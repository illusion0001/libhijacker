#pragma once

#include "util.hpp"
#include <sys/_stdint.h>

extern "C" {

#include <ps5/kernel.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

}

template <typename T>
void kread(uintptr_t addr, T *dst) {
	kernel_copyout(addr, dst, sizeof(T));
}

template <typename T>
T kread(uintptr_t addr) {
	T dst{};
	kernel_copyout(addr, &dst, sizeof(T));
	return dst;
}

template <unsigned long length>
void kread(uintptr_t addr, uint8_t(&buf)[length]) {
	kernel_copyout(addr, buf, length);
}

template <typename T>
void kwrite(uintptr_t addr, const T& value) {
	kernel_copyin(const_cast<T *>(&value), addr, sizeof(T));
}

template <unsigned long length>
void kwrite(uintptr_t addr, const uint8_t(&buf)[length]) {
	kernel_copyin(const_cast<uint8_t *>(buf), addr, length);
}

template <typename ObjBase, unsigned long size>
class KernelObject;

template <class T>
concept SizedKernelObject = requires { T::length; };

template <class T>
concept DerivedKernelObject = requires (T t) {
	static_cast<KernelObject<T, T::length>>(t);
};

template <class T>
concept KernelObjectBase = SizedKernelObject<T> && DerivedKernelObject<T>;

template<KernelObjectBase T>
class KIterable;

String getKernelString(uintptr_t addr);

template <typename ObjBase, unsigned long size>
class KernelObject {

	uintptr_t addr;

	protected:
		template<KernelObjectBase Base>
		friend class KIterable;
		uint8_t buf[__restrict size];
		explicit KernelObject() {}
		KernelObject(uintptr_t addr) : addr(addr) {
			#ifdef DEBUG
			if (addr == 0) [[unlikely]] {
				fatalf("kernel nullpointer dereference attempted\n");
				volatile void **tmp = nullptr;
				*tmp = nullptr;
			}
			#endif
			kernel_copyout(addr, buf, size);
		}

		void reload() {
			kernel_copyout(addr, buf, size);
		}

		template<typename T, const unsigned long offset>
		T get() const {
			static_assert(offset < size, "offset >= size");
			return *(T *)(buf + offset);
		}

		template<const unsigned long offset, typename T>
		void set(T value) {
			static_assert(offset < size, "offset >= size");
			*(T *)(buf + offset) = value;
		}

		template<const unsigned long offset>
		String getString() const {
			uintptr_t addr = get<uintptr_t, offset>();
			return getKernelString(addr);
		}

	public:
		static constexpr size_t length = size;
		KernelObject(KernelObject &&rhs) = default;
		KernelObject &operator=(KernelObject &&rhs) = default;
		KernelObject(const KernelObject &rhs) = default;
		KernelObject &operator=(const KernelObject &rhs) = default;

		explicit operator bool() const {
			return addr;
		}

		const void *data() const {
			return buf;
		}

		uintptr_t address() const {
			return addr;
		}

		void flush() const {
			#ifdef DEBUG
			if (!addr) [[unlikely]] {
				fatalf("nullptr dereference\n");
			}
			#endif
			kernel_copyin(const_cast<uint8_t *>(buf), addr, size);
		}
};


template<typename T>
class KPointer {

	protected:
		const uintptr_t addr;

	public:
		KPointer(decltype(nullptr)) : addr(0) {}
		KPointer(uintptr_t addr) : addr(addr) {}
		T operator*() const {
			T t;
			kernel_copyout(addr, &t, sizeof(T));
			return t;
		}

		explicit operator bool() const {
			return addr;
		}

		bool operator==(const KPointer<T> &rhs) const { return addr == rhs.addr; }
};

template<KernelObjectBase T>
struct KIterator;

template<KernelObjectBase T>
class KIterable {

	friend struct KIterator<T>;

	protected:

		uintptr_t addr;

		KIterable(decltype(nullptr)) : addr() {}

	public:
		KIterable(uintptr_t addr) : addr(addr) {}

		UniquePtr<T> operator*() {
			return new T{addr};
		}

		bool operator==(decltype(nullptr)) const { return addr == 0; }
		bool operator!=(decltype(nullptr)) const { return addr != 0; }

		KIterable<T> &operator++() {
			#ifdef DEBUG
			if (addr == 0) [[unlikely]] {
				return *this;
			}
			#endif
			uintptr_t ptr = 0;
			kernel_copyout(addr, &ptr, sizeof(ptr));
			addr = ptr;
			return *this;
		}

		explicit operator bool() const { return addr; }
};

template<KernelObjectBase T>
struct KIterator {

	const uintptr_t addr;

	KIterator(uintptr_t addr) : addr(addr) {}
	KIterable<T> begin() const {
		return addr;
	}
	decltype(nullptr) end() const {
		return nullptr;
	}

};

class KUcred : public KernelObject<KUcred, 0x168> {

	public:
		KUcred(uintptr_t addr) : KernelObject(addr) {}

		uint64_t authid() const {
			return get<uint64_t, 0x58>();
		}

		void authid(uint64_t value) {
			set<0x58>(value);
		}
};
