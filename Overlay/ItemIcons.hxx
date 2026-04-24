#pragma once
#include <Windows.h>
#include <string>
#include <unordered_map>
#include <mutex>

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

extern ID3D11Device* g_pd3dDevice;

namespace ItemIcons
{
	inline const wchar_t* g_rust_path = L"C:\\Program Files (x86)\\Steam\\steamapps\\common\\Rust\\Bundles\\items";
	inline std::unordered_map<std::string, ID3D11ShaderResourceView*> g_cache;
	inline std::mutex g_cache_mutex;

	inline std::wstring ToWide(const std::string& str)
	{
		if (str.empty()) return {};
		int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
		std::wstring wstr(size, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), wstr.data(), size);
		return wstr;
	}

	inline ID3D11ShaderResourceView* GetIcon(const std::string& shortname)
	{
		if (shortname.empty() || shortname == "Empty" || !g_pd3dDevice) return nullptr;

		{
			std::lock_guard<std::mutex> lock(g_cache_mutex);
			auto it = g_cache.find(shortname);
			if (it != g_cache.end())
				return it->second;
		}

		std::wstring wname = ToWide(shortname);
		const wchar_t* exts[] = { L".png", L".jpg", L".jpeg", L".dds", L".tga" };

		ID3D11ShaderResourceView* srv = nullptr;
		for (const wchar_t* ext : exts)
		{
			wchar_t path[MAX_PATH];
			swprintf_s(path, L"%s\\%s%s", g_rust_path, wname.c_str(), ext);

			HRESULT hr = D3DX11CreateShaderResourceViewFromFileW(
				g_pd3dDevice, path, nullptr, nullptr, &srv, 0);

			if (SUCCEEDED(hr) && srv)
				break;
			if (srv) { srv->Release(); srv = nullptr; }
		}

		if (!srv) return nullptr;

		{
			std::lock_guard<std::mutex> lock(g_cache_mutex);
			g_cache[shortname] = srv;
		}
		return srv;
	}
}
