#define NOBUILD_IMPLEMENTATION
#include "./nobuild.h"
#include "./nob.h"

#define CC "cc"
#define CFLAGS "-Wall", "-Wextra", "-std=c99", "-pedantic", "-I", PATH(RAYLIB_PATH, "src"), "-I", PATH(RAYGUI_PATH, "src")

#define LDFLAGS "-l", "m"

void build_tool(const char *tool)
{
	Cstr tool_path= PATH("tools", tool);
	// For now only consider UNIX
	CMD(CC, CFLAGS, "-o", NOEXT(tool_path), tool_path);
}

void ensure_raygui()
{
	if (!PATH_EXISTS(RAYGUI_PATH))
	{
		INFO("Raygui not found, cloning...");
		CMD("git", "clone", "https://github.com/raysan5/raygui.git", RAYGUI_PATH);
	}
	CMD("git", "-C", RAYGUI_PATH, "checkout", "4.0");
}

void ensure_raylib()
{
	if (!PATH_EXISTS(RAYLIB_PATH))
	{
		INFO("Raylib not found, cloning...");
		CMD("git", "clone", "https://github.com/raysan5/raylib.git", RAYLIB_PATH);
	}
	CMD("git", "-C", RAYLIB_PATH, "checkout", "5.0");
}

void build_raylib_static()
{
	ensure_raylib();
	INFO("Building Raylib[static]");
	CMD("make", "-C", PATH(RAYLIB_PATH, "src"), "-j", NPROC, "PLATFORM=PLATFORM_DESKTOP");
}

void build()
{
	build_raylib_static();	
	ensure_raygui();

	CMD(CC, CFLAGS, "./main.c", PATH(RAYLIB_PATH, "src/libraylib.a"), "-o", "rplot", LDFLAGS);
}

int main(int argc, char **argv)
{
	GO_REBUILD_URSELF(argc, argv);

	build();

	if (argc > 1) if (argv[1][0] == 'r') CMD("./rplot");
	if (argc > 1) if (argv[1][0] == 'c') RM("./rplot");

	return 0;
}
