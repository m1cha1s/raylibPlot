cmake_minimum_required(VERSION 3.16)

project(raylibPlot)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(raylibPlot
	main.c

	serial_linux.c
)

add_subdirectory(raylib)

target_link_libraries(raylibPlot PRIVATE raylib)

target_include_directories(raylibPlot PRIVATE raygui/src)
