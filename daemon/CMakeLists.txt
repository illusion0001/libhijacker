###################################################################################################
# PS5SDK - Example: pipe pirate
# Uses the read/write primitive to read and write some kernel data.
# @authors ChendoChap, Specter, Znullptr
###################################################################################################

cmake_minimum_required (VERSION 3.20)

set(basename "daemon")
project(${basename} C CXX ASM)

# Language Standard Defaults
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

# Check for sub-project as part of main build or external build
if (NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(IS_SUBPROJECT TRUE)
else()
	set(IS_SUBPROJECT FALSE)
endif()

message("IS_SUBPROJECT: ${IS_SUBPROJECT}")

set(D_CWD "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${D_CWD}/../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${D_CWD}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${D_CWD}/bin)	# static libs are archive

# Headers
include_directories	(SYSTEM "${D_PS5SDK}")
include_directories	(SYSTEM "${D_PS5SDK}/include")

add_executable(${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

# Must build with clang
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "[Cc]lang")
	set(IS_CLANG 1)
else()
	message(FATAL_ERROR "${PROJECT_NAME} is meant to be built with clang! CompilerID: ${CMAKE_CXX_COMPILER_ID}")
endif()

# Finalize main target sources
target_compile_options(${PROJECT_NAME} PUBLIC
	$<$<COMPILE_LANGUAGE:C>:${C_DEFS} ${C_FLAGS}>
	$<$<COMPILE_LANGUAGE:CXX>:${CXX_DEFS} ${CXX_FLAGS}>
	$<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>
)

message("========== build: ${PROJECT_NAME} ==========")

set(D_SRC ${D_CWD}/source)

file(GLOB SrcFiles ${D_SRC}/*.c ${D_SRC}/*.cpp ${D_SRC}/*.h ${D_SRC}/*.hpp ${D_SRC}/*.s ${D_SRC}/../../ftp/*.c ${D_SRC}/../../ftp/*.h ${D_SRC}/../../extern/pfd_sfo_tools/sfopatcher/src/*.c ${D_SRC}/../../extern/pfd_sfo_tools/sfopatcher/src/*.h ${D_SRC}/../../extern/tiny-json/*.c ${D_SRC}/../../extern/tiny-json/*.h ${D_SRC}/../../extern/mxml/*.c ${D_SRC}/../../extern/mxml/*.h)

set(CMAKE_C_FLAGS "--target=x86_64-freebsd-pc-elf -DPPR -DPS5 -DPS5_FW_VERSION=${V_FW} ")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_POSIX_SOURCE -D_POSIX_C_SOURCE=200112 -D__BSD_VISIBLE=1 -D__XSI_VISIBLE=500")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fshort-wchar -fno-builtin -nostdlib -Wall") #  -nostartfiles
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fPIE -march=znver2 -Wall -Werror")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -DDEBUG -gfull -gdwarf-2 -O0 -pedantic -pedantic-errors")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -gfull -gdwarf-2 -O0")

target_sources(${PROJECT_NAME} PRIVATE ${SrcFiles})
target_include_directories(${PROJECT_NAME} PRIVATE "${D_CWD}/../include")
target_link_directories	(${PROJECT_NAME} PUBLIC "${PROJECT_ROOT}/lib")
target_link_libraries	(${PROJECT_NAME} PUBLIC hijacker SceLibcInternal SceSystemService SceUserService SceSysmodule SceSysCore ScePad SceVideoOut kernel_sys)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld -Xlinker -pie -Xlinker -zmax-page-size=16384 -Xlinker -zcommon-page-size=16384 -Xlinker -T ${D_CWD}/linker.x -Wl,--build-id=none -Wl,-z,norelro")
