#define NOBUILD_IMPLEMENTATION
#include "./nobuild.h"
#include "./nob.h"

#define CC "cc"
#define CFLAGS "-Wall", "-Wextra", "-std=c99", "-pedantic"

void build_tool(const char *tool)
{
	Cstr tool_path= PATH("tools", tool);
	// For now only consider UNIX
	CMD(CC, CFLAGS, "-o", NOEXT(tool_path), tool_path);
}

void ensure_raylib()
{
	if (!PATH_EXISTS("./raylib"))
	{
		INFO("Raylib not found, cloning...");
		CMD("git", "clone", "https://github.com/raysan5/raylib.git");
	}
	CMD("git", "-C", "./raylib", "checkout", "5.0");
}

void build_raylib_static()
{
	ensure_raylib();
	INFO("Building Raylib[static]");
	CMD("make", "-C", "./raylib/src", "-j", NPROC, "PLATFORM=PLATFORM_DESKTOP");
}

void build()
{
	build_raylib_static();	

	CMD(CC, CFLAGS, "./main.c", "./raylib/src/libraylib.a", "-o", "rplot", "-I", "./raylib/src", "-l", "m");
}

int main(int argc, char **argv)
{
	GO_REBUILD_URSELF(argc, argv);

	build();

	if (argc > 1) if (argv[1][0] == 'r') CMD("./rplot");
	if (argc > 1) if (argv[1][0] == 'c') RM("./rplot");

	return 0;
}
