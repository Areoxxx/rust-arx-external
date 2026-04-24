#pragma once
#ifndef DRIVER_HPP
#define DRIVER_HPP

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <iostream>

#pragma comment(lib, "Psapi.lib")

class Driver
{
public:
	int32_t        m_pid = 0;
	HANDLE         m_handle = nullptr;
	uintptr_t      BaseAddress = 0;

	bool initialize_handle() { return true; }

	bool attach(int pid)
	{
		if (!pid) return false;
		m_pid = pid;
		m_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, static_cast<DWORD>(pid));
		if (!m_handle || m_handle == INVALID_HANDLE_VALUE)
		{
			m_handle = nullptr;
			return false;
		}
		return true;
	}

	uint32_t get_process_pid(const std::wstring& name)
	{
		PROCESSENTRY32W pe{};
		pe.dwSize = sizeof(pe);
		HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snap == INVALID_HANDLE_VALUE) return 0;

		if (Process32FirstW(snap, &pe))
		{
			do
			{
				if (!_wcsicmp(pe.szExeFile, name.c_str()))
				{
					CloseHandle(snap);
					return pe.th32ProcessID;
				}
			} while (Process32NextW(snap, &pe));
		}
		CloseHandle(snap);
		return 0;
	}

	uintptr_t get_module(const wchar_t* name)
	{
		HMODULE modules[512];
		DWORD needed = 0;
		if (!EnumProcessModulesEx(m_handle, modules, sizeof(modules), &needed, LIST_MODULES_ALL))
			return 0;

		int count = needed / sizeof(HMODULE);
		for (int i = 0; i < count; i++)
		{
			wchar_t buf[MAX_PATH]{};
			if (GetModuleBaseNameW(m_handle, modules[i], buf, MAX_PATH))
				if (!_wcsicmp(buf, name))
					return reinterpret_cast<uintptr_t>(modules[i]);
		}
		return 0;
	}

	bool read(uintptr_t address, void* buffer, size_t size)
	{
		SIZE_T bytes = 0;
		BOOL ok = ReadProcessMemory(m_handle, reinterpret_cast<LPCVOID>(address), buffer, size, &bytes);
		return ok && bytes == size;
	}

	bool write(uintptr_t address, const void* buffer, size_t size)
	{
		SIZE_T bytes = 0;
		BOOL ok = WriteProcessMemory(m_handle, reinterpret_cast<LPVOID>(address), buffer, size, &bytes);
		return ok && bytes == size;
	}

	template<typename T>
	T Read(uintptr_t address)
	{
		T val{};
		read(address, &val, sizeof(T));
		return val;
	}

	template<typename T>
	bool Write(uintptr_t address, const T& val)
	{
		return write(address, &val, sizeof(T));
	}

	template<typename T>
	T ReadChain(uint64_t address, const std::vector<uint64_t>& offsets)
	{
		uint64_t cur = address;
		for (size_t i = 0; i < offsets.size() - 1; i++)
		{
			cur = Read<uint64_t>(cur + offsets[i]);
			if (!is_valid(cur)) return T{};
		}
		return Read<T>(cur + offsets.back());
	}

	std::string ReadChar(uint64_t addr)
	{
		char buf[128]{};
		if (!read(addr, buf, sizeof(buf))) return "";
		buf[127] = '\0';
		return std::string(buf);
	}

	std::string ReadString(uint64_t address)
	{
		char buf[128]{};
		if (!read(address, buf, sizeof(buf))) return "";
		buf[127] = '\0';
		return std::string(buf);
	}

	std::wstring ReadWString(uint64_t address, size_t size)
	{
		if (!size || size > 2048) return L"";
		std::vector<wchar_t> buf(size);
		if (!read(address, buf.data(), size * sizeof(wchar_t))) return L"";
		return std::wstring(buf.data());
	}

	static bool is_valid(uint64_t address)
	{
		return address > 0x10000 && address < 0x7FFFFFFFFFFF && address != 0xCCCCCCCCCCCCCCCC;
	}
};

static Driver* driver = new Driver();

inline bool is_valid(uint64_t address)
{
	return Driver::is_valid(address);
}

#endif
