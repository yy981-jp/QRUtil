cmake_minimum_required(VERSION 3.20)

project(
	QRU
	VERSION 2.0.0
	DESCRIPTION "QRCode Util"
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

set(EXE_NAME "qr")

# Build CLI11 as a static library instead of header-only
set(CLI11_PRECOMPILED ON CACHE BOOL "Build CLI11 as a precompiled static library")

add_subdirectory(
	${CMAKE_SOURCE_DIR}/external/CLI11
	${CMAKE_BINARY_DIR}/CLI11
)
add_subdirectory(external/zxing/core)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_executable(${EXE_NAME} ${SOURCES})

target_compile_options(${EXE_NAME} PRIVATE
	-fdiagnostics-color=always
)

target_include_directories(${EXE_NAME} PRIVATE
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/external
)

target_link_libraries(${EXE_NAME} PRIVATE
	CLI11::CLI11
	ZXing::ZXing
)
