#include "Config.hxx"
#include <fstream>
#include <Windows.h>
#include <cstdio>

Config cfg;

static void GetConfigPath(char* out, size_t out_size, const char* config_name)
{
	char exe_path[MAX_PATH];
	GetModuleFileNameA(NULL, exe_path, MAX_PATH);
	char* last_slash = strrchr(exe_path, '\\');
	if (last_slash)
		*last_slash = '\0';
	snprintf(out, static_cast<int>(out_size), "%s\\config_%s.bin", exe_path, config_name);
}

void SaveConfig(const char* config_name)
{
	char path[MAX_PATH];
	GetConfigPath(path, sizeof(path), config_name);
	std::ofstream file(path, std::ios::binary);
	if (file.is_open())
		file.write(reinterpret_cast<const char*>(&cfg), sizeof(cfg));
}

void LoadConfig(const char* config_name)
{
	char path[MAX_PATH];
	GetConfigPath(path, sizeof(path), config_name);
	std::ifstream file(path, std::ios::binary);
	if (file.is_open())
		file.read(reinterpret_cast<char*>(&cfg), sizeof(cfg));
}
