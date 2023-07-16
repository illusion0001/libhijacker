#pragma once

#include "thread.hpp"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <poll.h>
#include <string.h>
#include <stdio.h>
#include <sys/_stdint.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>

#ifndef SOCK_NONBLOCK
#define	SOCK_NONBLOCK	0x20000000
#endif

namespace {

static constexpr int STUPID_C_ERROR_VALUE = -1;
struct BaseTcpServer;

}

class TcpSocket {

	int fd;

	public:
		TcpSocket(int sock) noexcept : fd(sock) {}
		TcpSocket(const TcpSocket&) = delete;
		TcpSocket &operator=(const TcpSocket&) = delete;
		TcpSocket(TcpSocket &&rhs) noexcept : fd(rhs.fd) {
			rhs.fd = -1;
		}
		TcpSocket &operator=(TcpSocket &&rhs) noexcept {
			close();
			fd = rhs.fd;
			rhs.fd = -1;
			return *this;
		}
		~TcpSocket() noexcept {
			close();
		}

		bool isClosed() const noexcept {
			return fd == -1;
		}

		explicit operator bool() const noexcept {
			return fd != -1;
		}

		void close() noexcept {
			if (fd != -1) {
				::close(fd);
			}
		}

		int getFd() const noexcept {
			return fd;
		}

		bool read(void *buf, size_t buflen) const noexcept;

		bool write(const void *buf, size_t buflen) const noexcept;

		template <size_t N>
		bool write(const char (&buf)[N]) const noexcept {
			return write(buf, N-1);
		}

		template <size_t N>
		bool println(const char (&buf)[N]) const noexcept {
			if (!write(buf)) [[unlikely]] {
				return false;
			}
			return write("\n");
		}

		template <size_t N>
		int puts(const char (&buf)[N]) const noexcept {
			if (println(buf)) {
				return N;
			}
			return 0;
		}
};

class ServerSocket {

	int fd;

	protected:
		friend struct BaseTcpServer;

		TcpSocket accept() const noexcept {
			pollfd pfd = {.fd = fd, .events = POLLHUP | POLLRDNORM, .revents = 0};
			int res = poll(&pfd, 1, INFTIM);
			if (res == STUPID_C_ERROR_VALUE || res == 0 || pfd.revents & POLLHUP) {
				// error occured
				return -1;
			}

			// we are ready to accept

			struct sockaddr client_addr{};
			socklen_t addr_len = sizeof(client_addr);
			int conn = ::accept(fd, &client_addr, &addr_len);
			if (conn == STUPID_C_ERROR_VALUE) {
				int err = errno;
				printf("accept failed %d %s\n", err, strerror(err));
			}
			return conn;
		}

		bool bind(struct sockaddr_in *server_addr) noexcept {
			if (::bind(fd, reinterpret_cast<struct sockaddr*>(server_addr), sizeof(sockaddr_in)) == STUPID_C_ERROR_VALUE) {
				int err = errno;
				printf("bind failed %d %s\n", err, strerror(err));
				return false;
			}
			return true;
		}

		bool listen(int backlog) noexcept {
			if (::listen(fd, backlog) == STUPID_C_ERROR_VALUE) {
				int err = errno;
				printf("listen failed %d %s\n", err, strerror(err));
				return false;
			}
			return true;
		}

		bool setsockopt(int level, int optname, const void *optval, socklen_t optlen) noexcept {
			auto result = ::setsockopt(fd, level, optname, optval, optlen);
			if (result == STUPID_C_ERROR_VALUE) [[unlikely]] {
				int err = errno;
				printf("setsockopt failed %d %s\n", err, strerror(err));
				return false;
			}
			return true;
		}

	public:
		ServerSocket(int fd) noexcept : fd(fd) {}

		ServerSocket(const ServerSocket&) = delete;

		ServerSocket &operator=(const ServerSocket&) = delete;

		ServerSocket(ServerSocket &&rhs) noexcept : fd(rhs.fd) {
			rhs.fd = -1;
		}

		ServerSocket &operator=(ServerSocket &&rhs) noexcept {
			if (fd != -1) {
				::close(fd);
			}
			fd = rhs.fd;
			rhs.fd = -1;
			return *this;
		}

		~ServerSocket() noexcept {
			if (fd != -1) {
				::close(fd);
			}
		}

		void close() noexcept {
			if (fd != -1) {
				::close(fd);
				fd = -1;
			}
		}

		bool isClosed() const noexcept {
			return fd == -1;
		}

		explicit operator bool() const noexcept {
			return fd != -1;
		}
};

namespace {

struct BaseTcpServer {

	ServerSocket serverSock;

	BaseTcpServer(uint16_t port) noexcept : serverSock(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) {
		if (!serverSock) {
			return;
		}

		int value = 1;
		if (!serverSock.setsockopt(SOL_SOCKET, SO_REUSEADDR, &value, sizeof(int))) {
			return;
		}

		struct sockaddr_in server_addr{0, AF_INET, htons(port), {}, {}};

		if (!serverSock.bind(&server_addr)) {
			return;
		}

		if (!serverSock.listen(1)) {
			return;
		}
	}

	explicit operator bool() const noexcept {
		return (bool)serverSock;
	}

	TcpSocket accept() const noexcept {
		return serverSock.accept();
	}

	bool isClosed() const noexcept {
		return serverSock.isClosed();
	}

};

}

class TcpServer : private BaseTcpServer, public JThread {

	static int start(void *self) noexcept {
		static_cast<TcpServer *>(self)->start();
		return 0;
	}

	void start() noexcept {
		while (!isClosed()) {
			TcpSocket sock = accept();
			if (sock) {
				run(sock);
				if (sock.isClosed()) {
					// closing the socket is a signal to shutdown
					close();
				}
			}
		}
	}

	void cancel() noexcept {
		close();
	}

	virtual void run(TcpSocket &sock) = 0;

	protected:
		static constexpr int STUPID_C_ERROR_VALUE = -1;

	public:
		TcpServer(uint16_t port) noexcept : BaseTcpServer(port), JThread(start, this) {}

		TcpServer(const TcpServer&) = delete;

		TcpServer &operator=(const TcpServer&) = delete;

		TcpServer(TcpServer &&rhs) noexcept = default;

		TcpServer &operator=(TcpServer &&rhs) noexcept = default;

		virtual ~TcpServer() noexcept {
			close();
		}

		void stop() noexcept {
			close();
		}

		void close() noexcept {
			serverSock.close();
		}
};