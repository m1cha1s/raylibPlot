.PHONY: all

all: build
	cmake --build build -j$(shell nproc)

build: raylib raygui
	cmake -B build

raylib:
	git clone https://github.com/raysan5/raylib.git
	git -C raylib checkout 5.0.0

raygui:
	git clone https://github.com/raysan5/raygui.git
	git -C raygui checkout 4.0.0
