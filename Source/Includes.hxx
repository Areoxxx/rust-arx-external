#pragma once
#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <thread>
#include <TlHelp32.h>
#include <Psapi.h>
#include <winternl.h>
#include <sstream>
#include <locale>
#include <tchar.h>
#include <map>
#include <mutex>
#include <array>
#include <unordered_map>
#include <vector>
#include <dwmapi.h>
#include <filesystem>
#include <string>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "Security/Xorstr.hpp"
#include "Security/Lazy_Importer.hpp"
#include "Security/Callstack_Spoofer.hpp"

#include "driver.h"

#include "Config/Config.hxx"

#include "Utils/Utils.hxx"
#include "Utils/Offsets.hxx"
#include "Utils/Decryption.hxx"
#include "Utils/Maths.hxx"

#include "SDK/Unity/Structs.hxx"
#include "SDK/Unity/Camera.hxx"
#include "SDK/Unity/Transform.hxx"
#include "SDK/Unity/Unity.hxx"

#include "SDK/Classes/PlayerModel.hxx"
#include "SDK/Classes/PlayerInput.hxx"
#include "SDK/Classes/PlayerEyes.hxx"
#include "SDK/Classes/BaseMovement.hxx"
#include "SDK/Classes/HeldItem.hxx"
#include "SDK/Classes/BasePlayer.hxx"
#include "SDK/Classes/Players.hxx"
#include "SDK/Classes/World.hxx"
#include "SDK/Classes/Cache.hxx"

#include "SDK/Features/Movement.hxx"
#include "SDK/Features/Environment.hxx"

#include "Threads/Update.hxx"
#include "Threads/Chams.hxx"
#include "Threads/Misc.hxx"
#include "Threads/Aimbot.hxx"

#include "Overlay/Overlay.hxx"
