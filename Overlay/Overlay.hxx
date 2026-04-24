#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <algorithm>
#include <iostream>

#include "imgui.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"

#include "font.h"
#include "imgui_freetype.h"
#include "gui_colors.h"
#include "Config/Config.hxx"
#include "gui.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

inline ID3D11Device* g_pd3dDevice = nullptr;
inline ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
inline IDXGISwapChain* g_pSwapChain = nullptr;
inline ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

namespace images {
	inline void* filled_box = nullptr;
	inline void* custom_sky = nullptr;
}

namespace n_discord
{
	struct OverlayData
	{
		HWND my_wnd{ nullptr };
		float m_screen_x = 1920.f;
		float m_screen_y = 1080.f;
		float middle_x = m_screen_x / 2.f;
		float middle_y = m_screen_y / 2.f;
		float fps = 0.f;
	} overlay;

	inline WNDPROC g_original_wndproc = nullptr;

	static LRESULT CALLBACK OverlayWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
			return 1;

		if (msg == WM_DESTROY) {
			PostQuitMessage(0);
			return 0;
		}

		if (g_original_wndproc)
			return CallWindowProcW(g_original_wndproc, hwnd, msg, wParam, lParam);

		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}

	static BOOL CALLBACK enum_windows_cb(HWND hwnd, LPARAM lParam)
	{
		wchar_t title[256]{};
		GetWindowTextW(hwnd, title, 256);
		if (wcsstr(title, L"Discord"))
		{
			*(HWND*)lParam = hwnd;
			return FALSE;
		}
		return TRUE;
	}

	inline HWND find_target_window()
	{
		HWND hwnd = nullptr;

		hwnd = FindWindowW(L"CEF-OSC-WIDGET", L"NVIDIA GeForce Overlay");
		if (hwnd) return hwnd;

		EnumWindows(enum_windows_cb, (LPARAM)&hwnd);
		if (hwnd) return hwnd;

		hwnd = FindWindowW(nullptr, L"Spotify Free");
		if (!hwnd) hwnd = FindWindowW(nullptr, L"Spotify Premium");
		if (hwnd) return hwnd;

		return nullptr;
	}

	inline bool create_overlay_window()
	{
		const int screenW = GetSystemMetrics(SM_CXSCREEN);
		const int screenH = GetSystemMetrics(SM_CYSCREEN);

		std::cout << "[~] Waiting for target window (Discord / NVIDIA Overlay / Spotify)...\n";
		while (true)
		{
			overlay.my_wnd = find_target_window();
			if (overlay.my_wnd) break;
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		std::cout << "[+] Window found!\n";

		DWORD wnd_pid = 0;
		GetWindowThreadProcessId(overlay.my_wnd, &wnd_pid);

		if (wnd_pid != GetCurrentProcessId())
		{
			LONG_PTR ex_style = GetWindowLongPtrW(overlay.my_wnd, GWL_EXSTYLE);
			ex_style |= WS_EX_LAYERED | WS_EX_TRANSPARENT;
			SetWindowLongPtrW(overlay.my_wnd, GWL_EXSTYLE, ex_style);

			SetWindowLongPtrW(overlay.my_wnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
			SetWindowPos(overlay.my_wnd, HWND_TOPMOST, 0, 0, screenW, screenH, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		}
		else
		{
			SetWindowLongPtrW(overlay.my_wnd, GWL_EXSTYLE, GetWindowLongPtrW(overlay.my_wnd, GWL_EXSTYLE) | WS_EX_LAYERED | WS_EX_TRANSPARENT);
			SetWindowLongPtrW(overlay.my_wnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);

			g_original_wndproc = (WNDPROC)SetWindowLongPtrW(overlay.my_wnd, GWLP_WNDPROC, (LONG_PTR)OverlayWndProc);

			SetWindowPos(overlay.my_wnd, HWND_TOPMOST, 0, 0, screenW, screenH, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		}

		MARGINS margin = { -1, -1, -1, -1 };
		DwmExtendFrameIntoClientArea(overlay.my_wnd, &margin);

		SetLayeredWindowAttributes(overlay.my_wnd, RGB(0, 0, 0), 255, LWA_ALPHA);
		ShowWindow(overlay.my_wnd, SW_SHOWDEFAULT);
		UpdateWindow(overlay.my_wnd);

		overlay.m_screen_x = static_cast<float>(screenW);
		overlay.m_screen_y = static_cast<float>(screenH);
		overlay.middle_x = overlay.m_screen_x * 0.5f;
		overlay.middle_y = overlay.m_screen_y * 0.5f;

		return true;
	}

	inline void cleanup_device()
	{
		if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
		if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
		if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
		if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
	}

	inline bool init_device()
	{
		DXGI_SWAP_CHAIN_DESC sd{};
		sd.BufferDesc.Width = static_cast<UINT>(overlay.m_screen_x);
		sd.BufferDesc.Height = static_cast<UINT>(overlay.m_screen_y);
		sd.BufferDesc.RefreshRate.Numerator = 60U;
		sd.BufferDesc.RefreshRate.Denominator = 1U;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1U;
		sd.SampleDesc.Quality = 0U;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2U;
		sd.OutputWindow = overlay.my_wnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		constexpr D3D_FEATURE_LEVEL levels[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

		HRESULT hr = D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0U,
			levels, 2U, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
			&g_pd3dDevice, nullptr, &g_pd3dDeviceContext
		);

		if (FAILED(hr)) return false;

		ID3D11Texture2D* back_buffer{ nullptr };
		g_pSwapChain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

		if (back_buffer) {
			g_pd3dDevice->CreateRenderTargetView(back_buffer, nullptr, &g_mainRenderTargetView);
			back_buffer->Release();
		}

		return true;
	}

	inline void init_imgui()
	{
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImFontConfig beta_cfg;
		beta_cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_LoadColor;

		io.Fonts->AddFontFromMemoryTTF(poppins, sizeof(poppins), 18.f, &beta_cfg, io.Fonts->GetGlyphRangesCyrillic());
		font.visualsFont = io.Fonts->AddFontFromMemoryTTF(minecraftia, sizeof(minecraftia), 16.f, &beta_cfg, io.Fonts->GetGlyphRangesCyrillic());
		font.visitorFont = io.Fonts->AddFontFromMemoryTTF(visitor_font, sizeof(visitor_font), 16.f, &beta_cfg, io.Fonts->GetGlyphRangesDefault());

		ImGui_ImplWin32_Init(overlay.my_wnd);
		ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
	}
}

#include "Visuals/Visuals.hxx"

namespace n_discord {
	inline void DrawCrosshair(ImDrawList* draw)
	{
		if (!draw) return;

		ImVec2 screenSize = ImGui::GetIO().DisplaySize;
		ImVec2 center(screenSize.x * 0.5f, screenSize.y * 0.5f);
		static float rotationDegree = 0.0f;
		float a = (screenSize.y / 2.0f) / 120.0f;
		float gamma = atan(a / a);

		if (rotationDegree > 89.0f)
			rotationDegree = 0.0f;

		for (int i = 0; i < 4; i++)
		{
			float angle = rotationDegree + (i * 90.0f);
			float p0 = a * sinf(IM_PI / 180.0f * angle);
			float p1 = a * cosf(IM_PI / 180.0f * angle);
			float p2 = (a / cosf(gamma)) * sinf(IM_PI / 180.0f * (angle + gamma * 180.0f / IM_PI));
			float p3 = (a / cosf(gamma)) * cosf(IM_PI / 180.0f * (angle + gamma * 180.0f / IM_PI));

			draw->AddLine(ImVec2(center.x, center.y), ImVec2(center.x + p0, center.y - p1), IM_COL32(255, 255, 255, 255), 1.2f);
			draw->AddLine(ImVec2(center.x + p0, center.y - p1), ImVec2(center.x + p2, center.y - p3), IM_COL32(255, 255, 255, 255), 1.2f);
		}

		rotationDegree += 1.0f;
	}

	inline void overlay_renderer()
	{
		if (!create_overlay_window()) return;
		if (!init_device()) return;
		init_imgui();

		bool last_streamproof = false;
		
		while (true)
		{
			const auto frame_start = std::chrono::high_resolution_clock::now();

			static bool insert_held = false;
			if (GetAsyncKeyState(VK_INSERT) & 0x8000)
			{
				if (!insert_held)
				{
					ui::menu_open = !ui::menu_open;
					LONG_PTR exStyle = GetWindowLongPtr(overlay.my_wnd, GWL_EXSTYLE);
					if (ui::menu_open)
					{
						exStyle &= ~WS_EX_TRANSPARENT;
						SetWindowLongPtr(overlay.my_wnd, GWL_EXSTYLE, exStyle);
						SetForegroundWindow(overlay.my_wnd);
					}
					else
					{
						exStyle |= WS_EX_TRANSPARENT;
						SetWindowLongPtr(overlay.my_wnd, GWL_EXSTYLE, exStyle);
					}
					insert_held = true;
				}
			}
			else
			{
				insert_held = false;
			}

			ui::colors::main.x = cfg.settings.gui_color[0];
			ui::colors::main.y = cfg.settings.gui_color[1];
			ui::colors::main.z = cfg.settings.gui_color[2];
			ui::colors::main.w = cfg.settings.gui_color[3];

			if (cfg.settings.streamproof != last_streamproof)
			{
				SetWindowDisplayAffinity(overlay.my_wnd, cfg.settings.streamproof ? WDA_EXCLUDEFROMCAPTURE : WDA_NONE);
				last_streamproof = cfg.settings.streamproof;
			}

			MSG msg;
			while (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (msg.message == WM_QUIT)
					return;
			}

			if (GetAsyncKeyState(VK_END) & 1)
				break;

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			{
				ImGuiIO& io = ImGui::GetIO();
				POINT cursor;
				GetCursorPos(&cursor);
				io.MousePos = ImVec2((float)cursor.x, (float)cursor.y);
				io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
				io.MouseDown[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
			}

			overlay_menu_open = ui::menu_open; 

			RenderVisuals();

			if (ui::menu_open)
				ui::render_menu();

			if (cfg.settings.crosshair)
				DrawCrosshair(ImGui::GetBackgroundDrawList());

			if (cfg.aimbot.showfov)
				ImGui::GetBackgroundDrawList()->AddCircle(
					{ (float)GetSystemMetrics(SM_CXSCREEN) / 2, (float)GetSystemMetrics(SM_CYSCREEN) / 2 },
					cfg.aimbot.fov,
					IM_COL32(
						(int)(cfg.aimbot.fov_color[0] * 255),
						(int)(cfg.aimbot.fov_color[1] * 255),
						(int)(cfg.aimbot.fov_color[2] * 255),
						(int)(cfg.aimbot.fov_color[3] * 255)
					)
				);

			if (cfg.aimbot.show_aim_line)
			{
				bool valid = false;
				Vector2 screen_pos;
				{
					std::lock_guard<std::mutex> lock(g_aim_visual_mutex);
					valid = g_aim_target_valid;
					if (valid) screen_pos = g_aim_target_screen;
				}
				if (valid)
				{
					float cx = (float)GetSystemMetrics(SM_CXSCREEN) / 2;
					float cy = (float)GetSystemMetrics(SM_CYSCREEN) / 2;
					ImU32 col = IM_COL32(
						(int)(cfg.aimbot.aim_line_color[0] * 255),
						(int)(cfg.aimbot.aim_line_color[1] * 255),
						(int)(cfg.aimbot.aim_line_color[2] * 255),
						(int)(cfg.aimbot.aim_line_color[3] * 255)
					);
					ImGui::GetBackgroundDrawList()->AddLine(ImVec2(cx, cy), ImVec2(screen_pos.x, screen_pos.y), col, 2.0f);
				}
			}

			ImGui::Render();

			constexpr float clear_color[4] = { 0.f, 0.f, 0.f, 0.f };
			g_pd3dDeviceContext->OMSetRenderTargets(1U, &g_mainRenderTargetView, nullptr);
			g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color);

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			g_pSwapChain->Present(0U, 0U);

			const auto frame_end = std::chrono::high_resolution_clock::now();
			const float frame_sec = std::chrono::duration<float>(frame_end - frame_start).count();
			if (frame_sec > 0.0001f)
			{
				const float instant_fps = 1.f / frame_sec;
				overlay.fps = overlay.fps * 0.9f + instant_fps * 0.1f;
			}

			if (cfg.settings.fps_limit > 0)
			{
				const std::chrono::duration<float> frame_time = frame_end - frame_start;
				const std::chrono::duration<float> target_time(1.0f / static_cast<float>(cfg.settings.fps_limit));
				if (frame_time < target_time)
					std::this_thread::sleep_for(target_time - frame_time);
			}
		}

		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
		cleanup_device();
	}
}
