cmake_minimum_required(VERSION 3.16)

project(CLI_devdiag
	VERSION 1.0
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(devdiag
	src/main.cpp
	src/command/system_command.cpp
	src/command/network_command.cpp
	src/common/logger.cpp
	src/common/parser.cpp
	src/system/sys_info.cpp
	src/system/cpu_info.cpp
	src/system/mem_info.cpp
	src/system/disk_info.cpp
	src/network/network_info.cpp
)

target_include_directories(devdiag
	PRIVATE
		${PROJECT_SOURCE_DIR}/include
		${PROJECT_SOURCE_DIR}/include/command
		${PROJECT_SOURCE_DIR}/include/common
		${PROJECT_SOURCE_DIR}/include/system
		${PROJECT_SOURCE_DIR}/include/network
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(devdiag PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
endif()

include(GNUInstallDirs)
install(TARGETS devdiag RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
