#include "Includes.hxx"
#include "driver.h"

#define STEELBLUE "\033[38;2;70;130;180m"
#define RESET     "\033[0m"

void TypewriterPrint(const std::string& prefix, const std::string& text, int delay_ms = 30)
{
	std::cout << STEELBLUE << prefix << RESET << std::flush;
	for (char c : text)
	{
		std::cout << c << std::flush;
		std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
	}
	std::cout << std::endl;
}

bool initialize_game()
{
	SPOOF_FUNC;

	if (!driver->initialize_handle())
	{
		std::cout << "[-] Failed to initialize driver handle." << std::endl;
		return false;
	}
	std::cout << "[+] Driver handle obtained." << std::endl;

	int wait = 0;
	while (!FindWindowA("UnityWndClass", "Rust"))
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		if (++wait % 1000 == 0)
			std::cout << "[~] Waiting for Rust..." << std::endl;
	}

	auto pid = driver->get_process_pid(L"RustClient.exe");
	if (!pid) return false;

	if (!driver->attach(pid)) return false;

	driver->BaseAddress = driver->get_module(L"RustClient.exe");
	if (!driver->BaseAddress) return false;

	globals.game_assembly = driver->get_module(L"GameAssembly.dll");
	if (!globals.game_assembly) return false;

	globals.unity_player = driver->get_module(L"UnityPlayer.dll");
	if (!globals.unity_player) return false;

	std::cout << "[+] Successfully initialized game pointers!" << std::endl;
	return true;
}

int main()
{
	SPOOF_FUNC;
	SetConsoleTitleA("Selling Console");
	TypewriterPrint("[~] ", "Starting Selling...");

	if (!initialize_game())
	{
		TypewriterPrint("[-] ", "Failed to initialize. Press any key to exit.");
		system("pause>nul");
		return 0;
	}

	TypewriterPrint("[+] ", "Starting threads...");
	LoadConfig("config.json");

	std::thread(UpdateCamera).detach();
	std::thread(UpdateLists).detach();
	std::thread(Aimbot).detach();
	std::thread(Misc).detach();

	TypewriterPrint("[+] ", "All threads running. Press END to stop.");
	n_discord::overlay_renderer();
	TypewriterPrint("[~] ", "Exiting...");

	return 0;
}
