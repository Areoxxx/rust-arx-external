#pragma once
#include <algorithm>
#include "Config/Config.hxx"
#include "gui_colors.h"
#include "../ItemIcons.hxx"
#include "gui.h"

namespace VisualConstants
{
	inline constexpr float kOutlineOffset = 1.2f;
	inline constexpr float kInnerOffset = 1.0f;
	inline constexpr float kCornerRatio = 0.1f;
	inline constexpr float kCornerThickness = 1.5f;
	inline constexpr float kOutlineThickness = 3.0f;
	inline constexpr float kSkeletonThickness = 1.5f;
	inline constexpr float kBoxThickness = 1.5f;
	inline constexpr float kTextOutlineOffset = 1.0f;
	inline constexpr float kFontSize = 16.0f;

	[[nodiscard]] static inline ImFont* GetESPFont() noexcept
	{
		switch (cfg.player_visuals.esp_font)
		{
		case 1: return font.spacegrotesk_medium[1] ? font.spacegrotesk_medium[1] : font.visualsFont;
		case 2: return font.inter_semibold[1] ? font.inter_semibold[1] : font.visualsFont;
		case 3: return ImGui::GetFont();
		case 4: return font.visitorFont ? font.visitorFont : font.visualsFont;
		default: return font.visualsFont ? font.visualsFont : ImGui::GetFont();
		}
	}
	[[nodiscard]] static inline float GetESPFontSize() noexcept
	{
		return std::clamp(cfg.player_visuals.esp_font_size, 10.0f, 24.0f);
	}
	inline constexpr float kHeadOffset = 0.3f;
	inline constexpr float kFootOffset = 0.3f;
	inline constexpr float kBoxWidthRatio = 0.45f;
	inline constexpr float k3DBoxHalfWidth = 0.35f;
	inline constexpr float k3DBoxHeadPad = 0.2f;
	inline constexpr float kWeaponSpacing = 2.0f;
	inline constexpr float kTextPad = 2.0f;
	inline constexpr float kHealthBarWidth = 3.0f;
	inline constexpr float kHealthBarOffset = 5.0f;
	inline constexpr float kFilledBoxAlpha = 0.25f;

	inline constexpr ImU32 kBlack = IM_COL32(0, 0, 0, 255);

	inline const std::vector<BoneList> kSkeletonBones = {
		pelvis,
		l_foot, l_knee, l_hip,
		r_foot, r_knee, r_hip,
		spine1, neck, head,
		l_upperarm, l_forearm, l_hand,
		r_upperarm, r_forearm, r_hand
	};

	inline const std::vector<std::pair<BoneList, BoneList>> kBoneConnections = {
		{ l_foot, l_knee },   { l_knee, l_hip },       { l_hip, pelvis },
		{ r_foot, r_knee },   { r_knee, r_hip },       { r_hip, pelvis },
		{ pelvis, spine1 },   { spine1, neck },         { neck, head },
		{ neck, l_upperarm }, { l_upperarm, l_forearm }, { l_forearm, l_hand },
		{ neck, r_upperarm }, { r_upperarm, r_forearm }, { r_forearm, r_hand }
	};
}

[[nodiscard]] static inline ImColor MakeColor(const float(&col)[4]) noexcept
{
	return ImColor(col[0], col[1], col[2], 1.0f);
}

[[nodiscard]] static inline ImVec2 ToImVec2(const Vector2& v) noexcept
{
	return ImVec2(v.x, v.y);
}

[[nodiscard]] static inline ImVec2 CenteredTextPos(const ImVec2& anchor, const ImVec2& text_size) noexcept
{
	return ImVec2(anchor.x - text_size.x * 0.5f, anchor.y - text_size.y - VisualConstants::kTextPad);
}

[[nodiscard]] static inline bool IsZero(const Vector3& v) noexcept
{
	return v.x == 0.0f && v.y == 0.0f && v.z == 0.0f;
}

[[nodiscard]] static inline Vector3 GetBoneFromSnapshot(const PlayerBones* pb, BoneList bone) noexcept
{
	if (!pb) return {};
	Vector3 out;
	return pb->get(static_cast<int>(bone), out) ? out : Vector3{};
}

[[nodiscard]] static inline Vector3 GetBoneCached(uintptr_t instance, BoneList bone) noexcept
{
	Vector3 out;
	return g_bone_cache.get_bone(instance, static_cast<int>(bone), out) ? out : Vector3{};
}

static ImVec2 radar_offset = ImVec2(20.0f, 20.0f);
static bool radar_dragging = false;
static ImVec2 radar_drag_start = ImVec2(0.0f, 0.0f);

static ImVec2 ammo_bar_offset = ImVec2(0.0f, -420.0f);
static bool ammo_bar_dragging = false;
static ImVec2 ammo_bar_drag_start = ImVec2(0.0f, 0.0f);

static ImVec2 hotbar_offset = ImVec2(0.0f, -80.0f);
static bool hotbar_dragging = false;
static ImVec2 hotbar_drag_start = ImVec2(0.0f, 0.0f);

static bool overlay_menu_open = false;

static void DrawRadar(ImDrawList* draw) noexcept
{
	if (!cfg.player_visuals.radar)
		return;

	std::vector<RustPlayer> cached_players;
	{
		std::lock_guard<std::mutex> lock(g_global_mutex);
		cached_players = global.all_player_list;
	}
	const float size = cfg.player_visuals.radar_size;
	const float range = cfg.player_visuals.radar_range;
	const float half = size;
	const float radius = half;
	const float corner_radius = 10.0f;

	ImVec2 screen = ImGui::GetIO().DisplaySize;
	ImVec2 radar_pos = radar_offset;

	radar_pos.x = std::clamp(radar_pos.x, 0.0f, screen.x - half * 2.0f);
	radar_pos.y = std::clamp(radar_pos.y, 0.0f, screen.y - half * 2.0f);

	ImVec2 center(radar_pos.x + half, radar_pos.y + half);
	ImVec2 box_min = radar_pos;
	ImVec2 box_max(radar_pos.x + half * 2.0f, radar_pos.y + half * 2.0f);

	if (overlay_menu_open)
	{
		ImVec2 mouse = ImGui::GetIO().MousePos;
		bool mouse_down = ImGui::GetIO().MouseDown[0];
		bool hovering = mouse.x >= box_min.x && mouse.x <= box_max.x &&
			mouse.y >= box_min.y && mouse.y <= box_max.y;

		if (hovering && mouse_down && !radar_dragging && !ImGui::IsAnyItemActive())
		{
			radar_dragging = true;
			radar_drag_start = ImVec2(mouse.x - radar_pos.x, mouse.y - radar_pos.y);
		}

		if (radar_dragging)
		{
			if (mouse_down)
			{
				radar_offset.x = mouse.x - radar_drag_start.x;
				radar_offset.y = mouse.y - radar_drag_start.y;
			}
			else
			{
				radar_dragging = false;
			}
		}

		if (hovering || radar_dragging)
		{
			draw->AddRect(box_min, box_max,
				ui::colors::MainAlpha(120), corner_radius, 0, 1.5f);
		}
	}

	draw->AddShadowRect(box_min, box_max,
		IM_COL32(0, 0, 0, 120), 20.0f, ImVec2(0, 2), 0.0f, corner_radius);

	draw->AddRectFilled(box_min, box_max, IM_COL32(10, 10, 16, 230), corner_radius);

	draw->PushClipRect(box_min, box_max, true);
	draw->AddRectFilledMultiColor(
		ImVec2(box_min.x + 1.0f, box_min.y + 1.0f),
		ImVec2(box_max.x - 1.0f, box_min.y + (half * 2.0f) * 0.15f),
		IM_COL32(255, 255, 255, 8), IM_COL32(255, 255, 255, 8),
		IM_COL32(255, 255, 255, 0), IM_COL32(255, 255, 255, 0)
	);

	float time = (float)ImGui::GetTime();
	float sweep_y = box_min.y + fmodf(time * 40.0f, half * 2.0f);
	draw->AddRectFilledMultiColor(
		ImVec2(box_min.x, sweep_y - 20.0f),
		ImVec2(box_max.x, sweep_y),
		ui::colors::MainAlpha(0), ui::colors::MainAlpha(0),
		ui::colors::MainAlpha(8), ui::colors::MainAlpha(8)
	);
	draw->AddRectFilledMultiColor(
		ImVec2(box_min.x, sweep_y),
		ImVec2(box_max.x, sweep_y + 2.0f),
		ui::colors::MainAlpha(15), ui::colors::MainAlpha(15),
		ui::colors::MainAlpha(0), ui::colors::MainAlpha(0)
	);

	const int grid_lines = 6;
	for (int i = 1; i < grid_lines; ++i)
	{
		float frac = (float)i / (float)grid_lines;
		float x = box_min.x + frac * half * 2.0f;
		float y = box_min.y + frac * half * 2.0f;
		draw->AddLine(ImVec2(x, box_min.y), ImVec2(x, box_max.y), IM_COL32(255, 255, 255, 8), 1.0f);
		draw->AddLine(ImVec2(box_min.x, y), ImVec2(box_max.x, y), IM_COL32(255, 255, 255, 8), 1.0f);
	}

	draw->AddLine(ImVec2(center.x, box_min.y), ImVec2(center.x, box_max.y), ui::colors::MainAlpha(20), 1.0f);
	draw->AddLine(ImVec2(box_min.x, center.y), ImVec2(box_max.x, center.y), ui::colors::MainAlpha(20), 1.0f);

	draw->AddCircle(center, radius * 0.25f, IM_COL32(255, 255, 255, 10), 64, 1.0f);
	draw->AddCircle(center, radius * 0.5f, IM_COL32(255, 255, 255, 14), 64, 1.0f);
	draw->AddCircle(center, radius * 0.75f, IM_COL32(255, 255, 255, 10), 64, 1.0f);

	draw->AddCircle(center, radius - 2.0f, ui::colors::MainAlpha(12), 64, 2.0f);

	float cam_yaw = std::atan2(camera.ViewMatrix._41, camera.ViewMatrix._43);

	float cos_yaw = std::cos(-cam_yaw);
	float sin_yaw = std::sin(-cam_yaw);

	float cone_half_angle = 0.45f;
	float cone_len = radius * 0.85f;

	ImVec2 fwd_tip(center.x, center.y - cone_len);
	ImVec2 fwd_l(center.x - cone_len * sinf(cone_half_angle), center.y - cone_len * cosf(cone_half_angle));
	ImVec2 fwd_r(center.x + cone_len * sinf(cone_half_angle), center.y - cone_len * cosf(cone_half_angle));

	draw->AddTriangleFilled(fwd_tip, fwd_l, fwd_r, ui::colors::MainAlpha(10));
	draw->AddLine(center, fwd_l, ui::colors::MainAlpha(25), 1.0f);
	draw->AddLine(center, fwd_r, ui::colors::MainAlpha(25), 1.0f);

	draw->AddCircleFilled(center, 6.0f, ui::colors::MainAlpha(25));
	draw->AddCircleFilled(center, 3.5f, ui::colors::MainAlpha(255));
	draw->AddCircleFilled(center, 1.5f, IM_COL32(200, 195, 255, 200));

	const uint64_t local_team_id = players.local_player ? players.local_player->GetCurrentTeam() : 0;
	const float scale = radius / range;

	for (const auto& player : cached_players)
	{
		if (player.is_local) continue;
		if (cfg.player_visuals.ignore_npcs && player.is_npc) continue;
		if (cfg.player_visuals.ignore_sleepers && player.is_sleeping) continue;

		const bool is_teammate = (local_team_id != 0 && player.team_id != 0 && player.team_id == local_team_id);
		if (cfg.player_visuals.ignore_teammates && is_teammate) continue;

		auto* base_player = reinterpret_cast<BasePlayer*>(player.instance);
		if (!base_player) continue;

		Vector3 head_pos = GetBoneCached(player.instance, BoneList::head);
		if (IsZero(head_pos)) continue;

		float dx = head_pos.x - camera.CameraPos.x;
		float dz = head_pos.z - camera.CameraPos.z;

		float dist = sqrtf(dx * dx + dz * dz);
		if (dist > range) continue;

		float rotated_x = dx * cos_yaw - dz * sin_yaw;
		float rotated_z = dx * sin_yaw + dz * cos_yaw;

		float px = center.x + rotated_x * scale;
		float py = center.y - rotated_z * scale;

		float cdx = px - center.x;
		float cdy = py - center.y;
		float cd = sqrtf(cdx * cdx + cdy * cdy);
		bool clamped = false;
		if (cd > radius - 6.0f)
		{
			float norm = (radius - 6.0f) / cd;
			px = center.x + cdx * norm;
			py = center.y + cdy * norm;
			clamped = true;
		}

		ImU32 dot_color, glow_color;
		if (is_teammate)
		{
			dot_color = IM_COL32(80, 255, 120, 255);
			glow_color = IM_COL32(80, 255, 120, 35);
		}
		else if (player.is_npc)
		{
			dot_color = IM_COL32(255, 210, 60, 255);
			glow_color = IM_COL32(255, 210, 60, 30);
		}
		else
		{
			dot_color = IM_COL32(255, 55, 55, 255);
			glow_color = IM_COL32(255, 55, 55, 40);
		}

		float dot_size = clamped ? 2.5f : 3.5f;

		draw->AddCircleFilled(ImVec2(px, py), dot_size + 4.0f, glow_color);
		draw->AddCircleFilled(ImVec2(px + 0.5f, py + 0.5f), dot_size, IM_COL32(0, 0, 0, 120));
		draw->AddCircleFilled(ImVec2(px, py), dot_size, dot_color);
		draw->AddCircleFilled(ImVec2(px - 0.5f, py - 0.5f), dot_size * 0.4f, IM_COL32(255, 255, 255, 120));

		if (clamped)
		{
			draw->AddCircle(ImVec2(px, py), dot_size + 2.0f, dot_color, 0, 1.0f);
		}
	}

	draw->PopClipRect();

	draw->AddRect(box_min, box_max, IM_COL32(255, 255, 255, 12), corner_radius, 0, 1.0f);

	float header_h = 18.0f;
	ImVec2 header_min = box_min;
	ImVec2 header_max(box_max.x, box_min.y + header_h);

	draw->PushClipRect(box_min, box_max, true);
	draw->AddRectFilled(header_min, header_max, IM_COL32(14, 14, 20, 200), corner_radius, ImDrawFlags_RoundCornersTop);
	draw->AddLine(ImVec2(header_min.x, header_max.y), ImVec2(header_max.x, header_max.y),
		ui::colors::MainAlpha(30), 1.0f);
	draw->PopClipRect();

	ImFont* radar_font = VisualConstants::GetESPFont();
	draw->AddText(radar_font, 10.0f, ImVec2(header_min.x + 8.0f, header_min.y + 3.0f),
		ui::colors::MainAlpha(200), "RADAR");

	int visible_count = 0;
	for (const auto& p : cached_players)
	{
		if (p.is_local) continue;
		if (cfg.player_visuals.ignore_npcs && p.is_npc) continue;
		if (cfg.player_visuals.ignore_sleepers && p.is_sleeping) continue;
		auto* bp = reinterpret_cast<BasePlayer*>(p.instance);
		if (!bp) continue;
		Vector3 hp = GetBoneCached(p.instance, BoneList::head);
		if (IsZero(hp)) continue;
		float d = sqrtf((hp.x - camera.CameraPos.x) * (hp.x - camera.CameraPos.x) +
			(hp.z - camera.CameraPos.z) * (hp.z - camera.CameraPos.z));
		if (d <= range) visible_count++;
	}

	char count_text[16];
	snprintf(count_text, sizeof(count_text), "%d", visible_count);
	ImVec2 count_size = radar_font->CalcTextSizeA(10.0f, FLT_MAX, 0.0f, count_text);
	draw->AddText(radar_font, 10.0f, ImVec2(header_max.x - count_size.x - 8.0f, header_min.y + 3.0f),
		IM_COL32(255, 255, 255, 100), count_text);

	char range_text[32];
	snprintf(range_text, sizeof(range_text), "%.0fm", range);
	ImVec2 range_size = radar_font->CalcTextSizeA(9.0f, FLT_MAX, 0.0f, range_text);
	draw->AddText(radar_font, 9.0f,
		ImVec2(box_max.x - range_size.x - 6.0f, box_max.y - range_size.y - 4.0f),
		IM_COL32(255, 255, 255, 40), range_text);
}

float reload_start_time = 0.0f;
float reload_duration = 0.0f;
bool was_reloading = false;

void RenderReloadBar(bool reloading, float time_ms)
{
	ImDrawList* draw = ImGui::GetBackgroundDrawList();

	if (reloading && !was_reloading)
	{
		reload_start_time = ImGui::GetTime() * 1000.0f;
		reload_duration = time_ms;
	}

	if (reloading)
	{
		float now = ImGui::GetTime() * 1000.0f;
		float elapsed = now - reload_start_time;
		float progress = std::clamp(elapsed / reload_duration, 0.0f, 1.0f);

		int r = static_cast<int>(180 * progress);
		int g = static_cast<int>(255 * (1.0f - (progress * 0.8f)));
		ImU32 fill_color = IM_COL32(r, g, 0, 255);

		ImVec2 screen = ImGui::GetIO().DisplaySize;
		float bar_width = 220.0f;
		float bar_height = 3.0f;
		ImVec2 bar_pos = ImVec2((screen.x - bar_width) / 2.0f, screen.y - 400.0f);

		float outline_thickness = 2.0f;
		ImU32 bg_color = IM_COL32(0, 0, 0, 180);

		draw->AddRectFilled(ImVec2(bar_pos.x - outline_thickness, bar_pos.y - outline_thickness), ImVec2(bar_pos.x + bar_width + outline_thickness, bar_pos.y + bar_height + outline_thickness), bg_color, 0.0f);

		float filled_width = bar_width * progress;
		draw->AddRectFilled(bar_pos, ImVec2(bar_pos.x + filled_width, bar_pos.y + bar_height), fill_color, 0.0f
		);
	}
	if (!reloading && was_reloading)
	{
		reload_duration = 0.0f;
		reload_start_time = 0.0f;
	}
	was_reloading = reloading;
}


void RenderAmmoBar(int current, int capacity)
{
	ImDrawList* draw = ImGui::GetBackgroundDrawList();
	ImVec2 screen = ImGui::GetIO().DisplaySize;

	float ratio = (capacity > 0) ? (static_cast<float>(current) / static_cast<float>(capacity)) : 0.0f;
	ratio = std::clamp(ratio, 0.0f, 1.0f);

	float bar_width = 200.0f;
	float bar_height = 5.0f;
	float padding_x = 14.0f;
	float padding_top = 10.0f;
	float padding_bottom = 8.0f;
	float corner_radius = 5.0f;
	float bar_radius = 2.5f;
	float text_spacing = 7.0f;

	ImFont* fnt = VisualConstants::GetESPFont();
	float font_size = 15.0f;

	char ammo_text[32];
	snprintf(ammo_text, sizeof(ammo_text), "%d", current);
	char cap_text[32];
	snprintf(cap_text, sizeof(cap_text), "/ %d", capacity);
	char label_text[] = "AMMO";

	ImVec2 ammo_size = fnt->CalcTextSizeA(font_size + 2.0f, FLT_MAX, 0.0f, ammo_text);
	ImVec2 cap_size = fnt->CalcTextSizeA(font_size - 1.0f, FLT_MAX, 0.0f, cap_text);
	ImVec2 label_size = fnt->CalcTextSizeA(font_size - 2.0f, FLT_MAX, 0.0f, label_text);

	float text_row_height = (ammo_size.y > cap_size.y ? ammo_size.y : cap_size.y);

	float bg_width = bar_width + padding_x * 2.0f;
	float bg_height = padding_top + bar_height + text_spacing + text_row_height + padding_bottom;

	ImVec2 bg_pos(
		(screen.x - bg_width) / 2.0f + ammo_bar_offset.x,
		screen.y + ammo_bar_offset.y
	);

	bg_pos.x = std::clamp(bg_pos.x, 0.0f, screen.x - bg_width);
	bg_pos.y = std::clamp(bg_pos.y, 0.0f, screen.y - bg_height);

	if (overlay_menu_open)
	{
		ImVec2 mouse = ImGui::GetIO().MousePos;
		bool mouse_down = ImGui::GetIO().MouseDown[0];
		bool hovering = mouse.x >= bg_pos.x && mouse.x <= bg_pos.x + bg_width &&
			mouse.y >= bg_pos.y && mouse.y <= bg_pos.y + bg_height;

		if (hovering && mouse_down && !ammo_bar_dragging && !ImGui::IsAnyItemActive())
		{
			ammo_bar_dragging = true;
			ammo_bar_drag_start = ImVec2(mouse.x - bg_pos.x, mouse.y - bg_pos.y);
		}

		if (ammo_bar_dragging)
		{
			if (mouse_down)
			{
				float new_x = mouse.x - ammo_bar_drag_start.x;
				float new_y = mouse.y - ammo_bar_drag_start.y;
				ammo_bar_offset.x = new_x - (screen.x - bg_width) / 2.0f;
				ammo_bar_offset.y = new_y - screen.y;
			}
			else
			{
				ammo_bar_dragging = false;
			}
		}

		if (hovering || ammo_bar_dragging)
		{
			draw->AddRect(bg_pos, ImVec2(bg_pos.x + bg_width, bg_pos.y + bg_height),
				ui::colors::MainAlpha(120), corner_radius, 0, 1.5f);
		}
	}

	draw->AddRectFilled(bg_pos, ImVec2(bg_pos.x + bg_width, bg_pos.y + bg_height),
		IM_COL32(8, 8, 12, 210), corner_radius);

	draw->PushClipRect(bg_pos, ImVec2(bg_pos.x + bg_width, bg_pos.y + bg_height), true);
	draw->AddRectFilledMultiColor(
		ImVec2(bg_pos.x + 1.0f, bg_pos.y + 1.0f),
		ImVec2(bg_pos.x + bg_width - 1.0f, bg_pos.y + bg_height * 0.35f),
		IM_COL32(255, 255, 255, 6), IM_COL32(255, 255, 255, 6),
		IM_COL32(255, 255, 255, 0), IM_COL32(255, 255, 255, 0)
	);
	draw->PopClipRect();

	draw->AddRect(bg_pos, ImVec2(bg_pos.x + bg_width, bg_pos.y + bg_height),
		IM_COL32(255, 255, 255, 10), corner_radius, 0, 1.0f);

	ImVec2 bar_pos(bg_pos.x + padding_x, bg_pos.y + padding_top);
	draw->AddRectFilled(bar_pos, ImVec2(bar_pos.x + bar_width, bar_pos.y + bar_height),
		IM_COL32(22, 22, 28, 255), bar_radius);

	for (int i = 1; i < 4; ++i)
	{
		float tick_x = bar_pos.x + bar_width * (i / 4.0f);
		draw->AddLine(
			ImVec2(tick_x, bar_pos.y),
			ImVec2(tick_x, bar_pos.y + bar_height),
			IM_COL32(255, 255, 255, 12), 1.0f
		);
	}

	float filled_width = bar_width * ratio;
	if (filled_width > 1.0f)
	{
		const float r = ui::colors::main.x, g = ui::colors::main.y, b = ui::colors::main.z;
		ImU32 col_l, col_r, glow_col;
		if (ratio > 0.5f)
		{
			col_l = IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 255);
			col_r = IM_COL32((int)(r * 0.9f * 255), (int)(g * 0.9f * 255), (int)(b * 255), 255);
			glow_col = IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 30);
		}
		else if (ratio > 0.25f)
		{
			col_l = IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 255);
			col_r = IM_COL32((int)(r * 0.85f * 255), (int)(g * 0.85f * 255), (int)(b * 255), 255);
			glow_col = IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 25);
		}
		else
		{
			col_l = IM_COL32((int)(r * 0.9f * 255), (int)(g * 0.9f * 255), (int)(b * 255), 255);
			col_r = IM_COL32((int)(r * 0.7f * 255), (int)(g * 0.7f * 255), (int)(b * 255), 255);
			glow_col = IM_COL32((int)(r * 255), (int)(g * 255), (int)(b * 255), 20);
		}

		ImVec2 fill_min = bar_pos;
		ImVec2 fill_max = ImVec2(bar_pos.x + filled_width, bar_pos.y + bar_height);

		draw->AddRectFilled(fill_min, fill_max, col_l, bar_radius);

		draw->PushClipRect(fill_min, fill_max, true);

		draw->AddRectFilledMultiColor(
			fill_min, fill_max,
			col_l, col_r, col_r, col_l
		);

		draw->AddRectFilledMultiColor(
			fill_min,
			ImVec2(fill_max.x, bar_pos.y + bar_height * 0.35f),
			IM_COL32(255, 255, 255, 55), IM_COL32(255, 255, 255, 35),
			IM_COL32(255, 255, 255, 0), IM_COL32(255, 255, 255, 0)
		);

		draw->PopClipRect();

		draw->AddRectFilledMultiColor(
			ImVec2(bar_pos.x, bar_pos.y + bar_height),
			ImVec2(bar_pos.x + filled_width, bar_pos.y + bar_height + 4.0f),
			glow_col, glow_col,
			IM_COL32(0, 0, 0, 0), IM_COL32(0, 0, 0, 0)
		);

		if (filled_width > 3.0f)
		{
			draw->AddRectFilled(
				ImVec2(bar_pos.x + filled_width - 1.5f, bar_pos.y),
				ImVec2(bar_pos.x + filled_width, bar_pos.y + bar_height),
				IM_COL32(255, 255, 255, 100), bar_radius
			);
		}
	}

	float text_y = bar_pos.y + bar_height + text_spacing;

	ImVec2 label_pos(bg_pos.x + padding_x, text_y + (text_row_height - label_size.y) * 0.5f);
	draw->AddText(fnt, font_size - 2.0f, label_pos, IM_COL32(100, 100, 115, 180), label_text);

	float right_x = bg_pos.x + bg_width - padding_x;
	ImVec2 cap_pos(right_x - cap_size.x, text_y + (text_row_height - cap_size.y) * 0.5f);
	draw->AddText(fnt, font_size - 1.0f, cap_pos, IM_COL32(120, 120, 135, 200), cap_text);

	ImU32 ammo_col;
	if (ratio > 0.25f)
		ammo_col = IM_COL32(230, 232, 240, 255);
	else if (current > 0)
		ammo_col = IM_COL32(255, 120, 80, 255);
	else
		ammo_col = IM_COL32(255, 55, 45, 255);

	ImVec2 ammo_pos(cap_pos.x - ammo_size.x - 3.0f, text_y + (text_row_height - ammo_size.y) * 0.5f);
	draw->AddText(fnt, font_size + 2.0f, ammo_pos, ammo_col, ammo_text);
}

void RenderHotbarGUI() noexcept
{
	if (!players.local_player || !cfg.player_visuals.itemlist)
		return;

	std::vector<std::string> slot_names;
	for (uint64_t item : players.local_player->GetBeltEntities())
	{
		std::string sn = players.local_player->GetItemShortName(item);
		slot_names.push_back(sn.empty() || sn == "Empty" ? "" : sn);
	}
	while (slot_names.size() < 6)
		slot_names.push_back("");

	constexpr int kSlots = 6;
	const float slot_size = 44.0f;
	const float slot_gap = 4.0f;
	const float padding = 12.0f;
	const float corner_radius = 6.0f;

	float panel_width = padding * 2.0f + kSlots * slot_size + (kSlots - 1) * slot_gap;
	float panel_height = padding * 2.0f + slot_size;

	ImDrawList* draw = ImGui::GetBackgroundDrawList();
	ImVec2 screen = ImGui::GetIO().DisplaySize;

	ImVec2 bg_pos(
		(screen.x - panel_width) * 0.5f + hotbar_offset.x,
		screen.y + hotbar_offset.y
	);
	bg_pos.x = std::clamp(bg_pos.x, 0.0f, screen.x - panel_width);
	bg_pos.y = std::clamp(bg_pos.y, 0.0f, screen.y - panel_height);

	if (overlay_menu_open)
	{
		ImVec2 mouse = ImGui::GetIO().MousePos;
		bool mouse_down = ImGui::GetIO().MouseDown[0];
		bool hovering = mouse.x >= bg_pos.x && mouse.x <= bg_pos.x + panel_width &&
			mouse.y >= bg_pos.y && mouse.y <= bg_pos.y + panel_height;

		if (hovering && mouse_down && !hotbar_dragging && !ImGui::IsAnyItemActive())
		{
			hotbar_dragging = true;
			hotbar_drag_start = ImVec2(mouse.x - bg_pos.x, mouse.y - bg_pos.y);
		}
		if (hotbar_dragging)
		{
			if (mouse_down)
			{
				hotbar_offset.x = mouse.x - hotbar_drag_start.x - (screen.x - panel_width) * 0.5f;
				hotbar_offset.y = mouse.y - hotbar_drag_start.y - screen.y;
			}
			else
				hotbar_dragging = false;
		}
		if (hovering || hotbar_dragging)
			draw->AddRect(bg_pos, ImVec2(bg_pos.x + panel_width, bg_pos.y + panel_height),
				ui::colors::MainAlpha(120), corner_radius, 0, 1.5f);
	}

	draw->AddRectFilled(bg_pos, ImVec2(bg_pos.x + panel_width, bg_pos.y + panel_height),
		IM_COL32(8, 8, 12, 210), corner_radius);
	draw->AddRect(bg_pos, ImVec2(bg_pos.x + panel_width, bg_pos.y + panel_height),
		IM_COL32(255, 255, 255, 10), corner_radius, 0, 1.0f);

	ImFont* fnt = VisualConstants::GetESPFont();
	const float font_size = 11.0f;

	for (int i = 0; i < kSlots; ++i)
	{
		float sx = bg_pos.x + padding + i * (slot_size + slot_gap);
		float sy = bg_pos.y + padding;
		ImVec2 slot_min(sx, sy);
		ImVec2 slot_max(sx + slot_size, sy + slot_size);

		if (images::filled_box)
		{
			draw->AddImageRounded((ImTextureID)images::filled_box, slot_min, slot_max,
				ImVec2(0, 0), ImVec2(1, 1), IM_COL32(28, 28, 35, 255), 4.0f);
		}
		else
		{
			draw->AddRectFilled(slot_min, slot_max, IM_COL32(28, 28, 35, 255), 4.0f);
		}
		draw->AddRect(slot_min, slot_max, IM_COL32(255, 255, 255, 25), 4.0f, 0, 1.0f);

		if (i < static_cast<int>(slot_names.size()) && !slot_names[i].empty())
		{
			const std::string& shortname = slot_names[i];
			ID3D11ShaderResourceView* icon = ItemIcons::GetIcon(shortname);
			const float icon_inset = 4.0f;
			ImVec2 icon_min(sx + icon_inset, sy + icon_inset);
			ImVec2 icon_max(sx + slot_size - icon_inset, sy + slot_size - icon_inset);

			if (icon)
			{
				draw->AddImage((ImTextureID)icon, icon_min, icon_max, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
			}
			else
			{
				std::string display = shortname;
				ImVec2 ts = fnt->CalcTextSizeA(font_size, FLT_MAX, 0.0f, display.c_str());
				while (ts.x > slot_size - 8.0f && display.size() > 1)
				{
					display = display.substr(0, display.size() - 1);
					ts = fnt->CalcTextSizeA(font_size, FLT_MAX, 0.0f, (display + "...").c_str());
				}
				if (display.size() < shortname.size())
					display += "...";
				ts = fnt->CalcTextSizeA(font_size, FLT_MAX, 0.0f, display.c_str());
				ImVec2 text_pos(sx + (slot_size - ts.x) * 0.5f, sy + (slot_size - ts.y) * 0.5f);
				draw->AddText(fnt, font_size, text_pos, IM_COL32(230, 232, 240, 255), display.c_str());
			}
		}
	}
}

static void DrawOutlinedText(ImDrawList* draw, const ImVec2& pos, const std::string& text, ImColor color, float font_size = 0.0f, ImFont* p_font = nullptr) noexcept
{
	float sz = font_size > 0.0f ? font_size : VisualConstants::GetESPFontSize();
	ImFont* fnt = p_font ? p_font : VisualConstants::GetESPFont();
	const auto* str = text.c_str();
	const float px = std::round(pos.x);
	const float py = std::round(pos.y);

	const ImU32 shadow_col = IM_COL32(0, 0, 0, 200);
	draw->AddText(fnt, sz, ImVec2(px + 1.f, py + 1.f), shadow_col, str);
	draw->AddText(fnt, sz, ImVec2(px, py), color, str);
}

static void DrawBox2D(ImDrawList* draw, const ImVec2& min, const ImVec2& max, ImColor color) noexcept
{
	const float outer = VisualConstants::kOutlineOffset;
	const float inner = VisualConstants::kInnerOffset;

	draw->AddRect({ min.x - outer, min.y - outer }, { max.x + outer, max.y + outer }, VisualConstants::kBlack, 0.0f, 0, 1.0f);
	draw->AddRect(min, max, color, 0.0f, 0, VisualConstants::kBoxThickness);
	draw->AddRect({ min.x + inner, min.y + inner }, { max.x - inner, max.y - inner }, VisualConstants::kBlack, 0.0f, 0, 1.0f);
}

static void DrawFilledBox(ImDrawList* draw, const ImVec2& min, const ImVec2& max, ImColor color) noexcept
{
	if (images::filled_box)
	{
		draw->AddImage((ImTextureID)images::filled_box, min, max, ImVec2(0, 0), ImVec2(1, 1), IM_COL32(255, 255, 255, 255));
	}
	else
	{
		ImColor fill_color = ImColor(color.Value.x, color.Value.y, color.Value.z, VisualConstants::kFilledBoxAlpha);
		draw->AddRectFilled(min, max, fill_color);
	}
	draw->AddRect(min, max, color, 0.0f, 0, VisualConstants::kBoxThickness);
}

static void DrawCornerBox(ImDrawList* draw, const ImVec2& min, const ImVec2& max, ImColor color) noexcept
{
	const float len = (max.y - min.y) * VisualConstants::kCornerRatio;
	const float thin = VisualConstants::kCornerThickness;
	const float thick = VisualConstants::kOutlineThickness;

	const ImVec2 corners[4] = { min, { max.x, min.y }, { min.x, max.y }, max };
	const ImVec2 hDirs[4] = { {1,0}, {-1,0}, {1,0}, {-1,0} };
	const ImVec2 vDirs[4] = { {0,1}, {0,1}, {0,-1}, {0,-1} };

	for (int i = 0; i < 4; ++i)
	{
		const ImVec2 hEnd = { corners[i].x + hDirs[i].x * len, corners[i].y + hDirs[i].y * len };
		const ImVec2 vEnd = { corners[i].x + vDirs[i].x * len, corners[i].y + vDirs[i].y * len };

		draw->AddLine(corners[i], hEnd, VisualConstants::kBlack, thick);
		draw->AddLine(corners[i], vEnd, VisualConstants::kBlack, thick);
		draw->AddLine(corners[i], hEnd, color, thin);
		draw->AddLine(corners[i], vEnd, color, thin);
	}
}


static bool DrawBox3D(ImDrawList* draw, BasePlayer* player, const PlayerBones* pb, ImColor color) noexcept
{
	auto* eyes = player->GetPlayerEyes();
	if (!eyes) return false;

	Vector3 rotation = eyes->GetBodyRotation();
	Vector3 right = rotation.Multiply({ -VisualConstants::k3DBoxHalfWidth, 0.0f, 0.0f });

	Vector3 bottom_bone = GetBoneFromSnapshot(pb, BoneList::r_foot);
	Vector3 top_bone = GetBoneFromSnapshot(pb, BoneList::head) + Vector3(0, VisualConstants::k3DBoxHeadPad, 0);

	if (IsZero(bottom_bone) || IsZero(top_bone)) return false;

	Vector3 world_pts[4] = { bottom_bone - right, bottom_bone + right, top_bone - right, top_bone + right };

	Vector2 screen_pts[4];
	for (int i = 0; i < 4; ++i)
	{
		if (!camera.WorldToScreen(world_pts[i], screen_pts[i]))
			return false;
	}
	draw->AddQuad(ToImVec2(screen_pts[2]), ToImVec2(screen_pts[3]), ToImVec2(screen_pts[1]), ToImVec2(screen_pts[0]), color, VisualConstants::kBoxThickness);
	return true;
}

static void DrawSkeleton(ImDrawList* draw, const ImVec2* screen_pos, const bool* bone_valid, ImU32 color) noexcept
{
	const float thick = VisualConstants::kSkeletonThickness;

	for (const auto& [from, to] : VisualConstants::kBoneConnections)
	{
		const int fi = static_cast<int>(from);
		const int ti = static_cast<int>(to);
		if (!bone_valid[fi] || !bone_valid[ti]) continue;

		ImVec2 a = screen_pos[fi];
		ImVec2 b = screen_pos[ti];
		a.x = std::round(a.x); a.y = std::round(a.y);
		b.x = std::round(b.x); b.y = std::round(b.y);

		draw->AddLine(a, b, color, thick);
	}
}

struct PlayerBounds
{
	ImVec2 top_left;
	ImVec2 bottom_right;
	Vector2 head_screen;
};

[[nodiscard]] static bool CalculatePlayerBounds(const PlayerBones* pb, float max_distance, float& out_distance, PlayerBounds& out_bounds) noexcept
{
	Vector3 head_pos = GetBoneFromSnapshot(pb, BoneList::head) + Vector3(0, VisualConstants::kHeadOffset, 0);
	if (IsZero(head_pos)) return false;

	Vector2 head_2d;
	if (!camera.WorldToScreen(head_pos, head_2d)) return false;

	out_distance = camera.CameraPos.Distance(head_pos);
	if (out_distance > max_distance) return false;

	Vector3 lfoot_pos = GetBoneFromSnapshot(pb, BoneList::l_foot) - Vector3(0, VisualConstants::kFootOffset, 0);
	Vector3 rfoot_pos = GetBoneFromSnapshot(pb, BoneList::r_foot) - Vector3(0, VisualConstants::kFootOffset, 0);

	Vector2 lfoot_2d, rfoot_2d;
	if (!camera.WorldToScreen(lfoot_pos, lfoot_2d)) return false;
	if (!camera.WorldToScreen(rfoot_pos, rfoot_2d)) return false;

	const float h = fabsf(lfoot_2d.y - head_2d.y);
	const float w = h * VisualConstants::kBoxWidthRatio;
	const float center_x = (lfoot_2d.x + rfoot_2d.x) * 0.5f;

	out_bounds.top_left = ImVec2(center_x - w * 0.5f, head_2d.y);
	out_bounds.bottom_right = ImVec2(center_x + w * 0.5f, lfoot_2d.y);
	out_bounds.head_screen = head_2d;

	return true;
}

static void RenderBoxESP(ImDrawList* draw, BasePlayer* player, const PlayerBounds& bounds, const PlayerBones* pb) noexcept
{
	const ImColor color = MakeColor(cfg.player_visuals.box_color);

	switch (cfg.player_visuals.box_type)
	{
	case 0:
		DrawBox2D(draw, bounds.top_left, bounds.bottom_right, color);
		break;
	case 1:
		DrawCornerBox(draw, bounds.top_left, bounds.bottom_right, color);
		break;
	case 2:
		DrawFilledBox(draw, bounds.top_left, bounds.bottom_right, color);
		break;
	case 3:
		DrawBox3D(draw, player, pb, color);
		break;
	default:
		break;
	}
}

static void RenderSkeletonESP(ImDrawList* draw, const PlayerBones* pb) noexcept
{
	if (!pb) return;

	ImVec2 screen_pos[kBoneArraySize];
	bool   bone_valid[kBoneArraySize] = {};

	for (const BoneList bone_id : VisualConstants::kSkeletonBones)
	{
		const int idx = static_cast<int>(bone_id);
		Vector3 world_pos;
		if (!pb->get(idx, world_pos)) continue;

		Vector2 sp;
		if (!camera.WorldToScreen(world_pos, sp)) continue;

		screen_pos[idx] = ImVec2(sp.x, sp.y);
		bone_valid[idx] = true;
	}
	DrawSkeleton(draw, screen_pos, bone_valid, MakeColor(cfg.player_visuals.bone_esp_color));
}

static void RenderHealthBar(ImDrawList* draw, const RustPlayer& player, const PlayerBounds& bounds) noexcept
{
	if (!player.is_local)
	{
		if (player.health_status == HealthStatus::Wounded || player.health_status == HealthStatus::Incapacitated)
		{
			const char* status = (player.health_status == HealthStatus::Wounded) ? "Knocked" : "Downed";
			ImColor color = (player.health_status == HealthStatus::Wounded)
				? ImColor(1.0f, 0.5f, 0.0f, 1.0f)
				: ImColor(1.0f, 0.0f, 0.0f, 1.0f);

		ImFont* fnt = VisualConstants::GetESPFont();
		const ImVec2 text_size = fnt->CalcTextSizeA(VisualConstants::GetESPFontSize(), FLT_MAX, 0.0f, status);
		const float center_x = (bounds.top_left.x + bounds.bottom_right.x) * 0.5f;
		const ImVec2 text_pos = { center_x - text_size.x * 0.5f, bounds.top_left.y - text_size.y - 18.0f };
		DrawOutlinedText(draw, text_pos, status, color);
		}
		return;
	}

	float health = std::clamp(player.health, 0.0f, player.max_health);
	float max_hp = player.max_health > 0.0f ? player.max_health : 100.0f;
	float fraction = health / max_hp;

	float bar_x = bounds.top_left.x - VisualConstants::kHealthBarOffset;
	float bar_top = bounds.top_left.y;
	float bar_bottom = bounds.bottom_right.y;
	float bar_height = bar_bottom - bar_top;
	float filled = bar_height * fraction;

	draw->AddRectFilled(
		{ bar_x - VisualConstants::kHealthBarWidth - 1.0f, bar_top - 1.0f },
		{ bar_x + 1.0f, bar_bottom + 1.0f },
		VisualConstants::kBlack
	);

	ImColor bar_color = ImColor(1.0f - fraction, fraction, 0.0f, 1.0f);

	draw->AddRectFilled(
		{ bar_x - VisualConstants::kHealthBarWidth, bar_top + (bar_height - filled) },
		{ bar_x, bar_bottom },
		bar_color
	);
}

[[nodiscard]] static std::string BuildInfoString(const RustPlayer& player, bool is_teammate) noexcept
{
	std::string result;
	result.reserve(64);

	if (cfg.player_visuals.show_name)
		result.append(player.name);

	if (cfg.player_visuals.show_team_id && player.team_id != 0)
	{
		if (!result.empty()) result.append(_(" "));
		if (is_teammate)
			result.append(_("[TEAM]"));
		else
			result.append(_("[T:") + std::to_string(player.team_id) + _("]"));
	}
	return result;
}

[[nodiscard]] static std::string BuildDistanceString(float distance) noexcept
{
	return std::to_string(static_cast<int>(distance)) + _("m");
}

[[nodiscard]] static ImVec2 ComputeESPLabelPos(
	const PlayerBounds& bounds, const ImVec2& text_size, int side, float stack_offset = 0.f) noexcept
{
	constexpr float kMargin = 4.f;
	const float center_x = (bounds.top_left.x + bounds.bottom_right.x) * 0.5f;

	switch (side)
	{
	case 0:
		return { bounds.top_left.x - kMargin - text_size.x,
		         bounds.top_left.y + stack_offset };
	case 2:
		return { bounds.bottom_right.x + kMargin,
		         bounds.top_left.y + stack_offset };
	case 3:
		return { center_x - text_size.x * 0.5f,
		         bounds.bottom_right.y + kMargin + stack_offset };
	default:
		return { center_x - text_size.x * 0.5f,
		         bounds.top_left.y - text_size.y - kMargin - stack_offset };
	}
}

static void RenderInfoText(ImDrawList* draw, const PlayerBounds& bounds, const std::string& info, bool is_teammate) noexcept
{
	if (info.empty()) return;

	const ImColor color = is_teammate ? ImColor(0.0f, 1.0f, 0.0f, 1.0f) : MakeColor(cfg.player_visuals.text_color);

	ImFont* fnt = VisualConstants::GetESPFont();
	const ImVec2 text_size = fnt->CalcTextSizeA(VisualConstants::GetESPFontSize(), FLT_MAX, 0.0f, info.c_str());
	const ImVec2 text_pos  = ComputeESPLabelPos(bounds, text_size, esp_layout::name_side);

	DrawOutlinedText(draw, text_pos, info, color);
}

static void RenderDistanceText(ImDrawList* draw, const PlayerBounds& bounds, float distance) noexcept
{
	if (!cfg.player_visuals.show_distance) return;
	const ImColor color = MakeColor(cfg.player_visuals.distance_color);


	const std::string dist_str = BuildDistanceString(distance);

	ImFont* fnt = VisualConstants::GetESPFont();
	const ImVec2 text_size = fnt->CalcTextSizeA(VisualConstants::GetESPFontSize(), FLT_MAX, 0.0f, dist_str.c_str());
	const ImVec2 text_pos  = ComputeESPLabelPos(bounds, text_size, esp_layout::distance_side);

	DrawOutlinedText(draw, text_pos, dist_str, color);
}

[[nodiscard]] static inline ImColor GetWeaponTierColor(WeaponTier tier) noexcept
{
	switch (tier)
	{
	case WeaponTier::Rifle:     return ImColor(1.0f, 0.3f, 0.3f, 1.0f);
	case WeaponTier::Sniper:    return ImColor(1.0f, 0.1f, 0.1f, 1.0f);
	case WeaponTier::LMG:       return ImColor(1.0f, 0.5f, 0.0f, 1.0f);
	case WeaponTier::SMG:       return ImColor(1.0f, 0.8f, 0.2f, 1.0f);
	case WeaponTier::Shotgun:   return ImColor(1.0f, 0.6f, 0.2f, 1.0f);
	case WeaponTier::Pistol:    return ImColor(0.6f, 0.8f, 1.0f, 1.0f);
	case WeaponTier::Bow:       return ImColor(0.6f, 1.0f, 0.6f, 1.0f);
	case WeaponTier::Launcher:  return ImColor(1.0f, 0.0f, 0.5f, 1.0f);
	case WeaponTier::Throwable: return ImColor(1.0f, 0.4f, 0.0f, 1.0f);
	case WeaponTier::Melee:     return ImColor(0.7f, 0.7f, 0.7f, 1.0f);
	case WeaponTier::Medical:   return ImColor(0.3f, 1.0f, 0.3f, 1.0f);
	case WeaponTier::Tool:      return ImColor(0.6f, 0.6f, 0.6f, 1.0f);
	case WeaponTier::Misc:      return ImColor(0.5f, 0.5f, 0.5f, 1.0f);
	default:                    return ImColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
}

static void RenderWeaponText(ImDrawList* draw, const PlayerBounds& bounds, const RustPlayer& player) noexcept
{
	if (player.weapon.empty() || player.weapon == _("Empty")) return;

	const ImColor color = GetWeaponTierColor(player.weapon_tier);

	ImFont* fnt = VisualConstants::GetESPFont();
	const ImVec2 text_size = fnt->CalcTextSizeA(VisualConstants::GetESPFontSize(), FLT_MAX, 0.0f, player.weapon.c_str());
	const ImVec2 text_pos  = ComputeESPLabelPos(bounds, text_size, esp_layout::weapon_side);

	DrawOutlinedText(draw, text_pos, player.weapon, color);
}

static void DrawPlayers(ImDrawList* draw) noexcept
{
	std::vector<RustPlayer> cached_players;
	{
		std::lock_guard<std::mutex> lock(g_global_mutex);
		cached_players = global.all_player_list;
	}
	if (cached_players.empty()) return;

	std::unordered_map<uintptr_t, PlayerBones> bone_cache_copy;
	g_bone_cache.copy_for_render(bone_cache_copy);

	const uint64_t local_team_id = players.local_player ? players.local_player->GetCurrentTeam() : 0;

	for (const auto& player : cached_players)
	{
		if (cfg.player_visuals.ignore_npcs && player.is_npc) continue;
		if (cfg.player_visuals.ignore_sleepers && player.is_sleeping) continue;

		const bool is_teammate = (local_team_id != 0 && player.team_id != 0 && player.team_id == local_team_id);
		if (cfg.player_visuals.ignore_teammates && is_teammate) continue;

		auto* base_player = reinterpret_cast<BasePlayer*>(player.instance);
		if (!base_player) continue;

		const PlayerBones* pb = nullptr;
		{
			auto it = bone_cache_copy.find(player.instance);
			if (it != bone_cache_copy.end()) pb = &it->second;
		}

		float distance = 0.0f;
		PlayerBounds bounds{};
		if (!CalculatePlayerBounds(pb, cfg.player_visuals.max_distance, distance, bounds))
			continue;

		if (cfg.player_visuals.box)
			RenderBoxESP(draw, base_player, bounds, pb);

		if (cfg.player_visuals.show_health)
			RenderHealthBar(draw, player, bounds);

		if (cfg.player_visuals.bone_esp)
			RenderSkeletonESP(draw, pb);

		const std::string info = BuildInfoString(player, is_teammate);
		RenderInfoText(draw, bounds, info, is_teammate);
		RenderDistanceText(draw, bounds, distance);

		if (cfg.player_visuals.weapon)
			RenderWeaponText(draw, bounds, player);
	}
}

[[nodiscard]] static inline ImColor GetWorldEntityColor(const std::string& name) noexcept
{
	if (name == "Elite Crate")    return ImColor(1.0f, 0.84f, 0.0f, 1.0f);
	if (name == "Military Crate") return ImColor(0.0f, 0.8f, 0.0f, 1.0f);
	if (name == "Crate")          return ImColor(0.6f, 0.6f, 0.6f, 1.0f);
	if (name == "AirDrop")        return ImColor(1.0f, 0.0f, 0.0f, 1.0f);
	if (name == "Locked Crate")   return ImColor(0.8f, 0.0f, 0.8f, 1.0f);
	if (name == "Autoturret")     return ImColor(1.0f, 0.2f, 0.2f, 1.0f);
	if (name == "Flameturret")    return ImColor(1.0f, 0.4f, 0.0f, 1.0f);
	if (name == "Shotgun Trap")   return ImColor(1.0f, 0.3f, 0.1f, 1.0f);
	if (name == "Corpse")         return ImColor(0.8f, 0.8f, 0.2f, 1.0f);
	if (name == "Stone")          return ImColor(0.7f, 0.7f, 0.7f, 1.0f);
	if (name == "Metal")          return ImColor(0.6f, 0.4f, 0.2f, 1.0f);
	if (name == "Sulfur")         return ImColor(0.9f, 0.9f, 0.0f, 1.0f);
	if (name == "Hemp")           return ImColor(0.0f, 0.9f, 0.0f, 1.0f);
	if (name == "Cave")           return ImColor(0.6f, 0.4f, 0.1f, 1.0f);
	if (IsAnimal(name))           return ImColor(1.0f, 0.6f, 0.2f, 1.0f);
	return MakeColor(cfg.world_esp.color);
}

[[nodiscard]] static Vector3 GetLiveEntityPosition(uintptr_t entity) noexcept
{
	uintptr_t position_ptr = driver->ReadChain<uintptr_t>(entity, {
		Offsets::GameObject::Object,
		Offsets::GameObject::Transform,
		Offsets::TransformChain::VisualState,
		Offsets::TransformChain::StateData,
		Offsets::TransformChain::TransformArray
		});
	if (!position_ptr)
		return {};

	return driver->Read<Vector3>(position_ptr + Offsets::TransformChain::Position);
}

static void DrawWorldEntities(ImDrawList* draw) noexcept
{
	std::vector<WorldEntity> cached_entities;
	{
		std::lock_guard<std::mutex> lock(g_global_mutex);
		cached_entities = global.world_entity_list;
	}
	if (cached_entities.empty()) return;

	for (const auto& ent : cached_entities)
	{
		bool is_animal = IsAnimal(ent.display_name);

		Vector3 position;
		if (is_animal)
			position = GetLiveEntityPosition(ent.instance);
		else
			position = ent.position;

		if (IsZero(position)) continue;

		float max_dist = is_animal ? cfg.world_esp.animals_max_distance : cfg.world_esp.max_distance;
		float distance = camera.CameraPos.Distance(position);
		if (distance > max_dist) continue;

		Vector2 screen_pos;
		if (!camera.WorldToScreen(position, screen_pos)) continue;

		const ImColor color = GetWorldEntityColor(ent.display_name);
		float y_offset = 0.0f;

		ImFont* fnt = VisualConstants::GetESPFont();

		if (cfg.world_esp.draw_name)
		{
			const ImVec2 text_size = fnt->CalcTextSizeA(VisualConstants::GetESPFontSize(), FLT_MAX, 0.0f, ent.display_name.c_str());
			const ImVec2 text_pos = { screen_pos.x - text_size.x * 0.5f, screen_pos.y + y_offset };
			DrawOutlinedText(draw, text_pos, ent.display_name, color);
			y_offset += text_size.y + 2.0f;
		}

		if (cfg.world_esp.draw_distance)
		{
			std::string dist_str = _("[") + std::to_string(static_cast<int>(distance)) + _("m]");
			const ImVec2 text_size = fnt->CalcTextSizeA(VisualConstants::GetESPFontSize(), FLT_MAX, 0.0f, dist_str.c_str());
			const ImVec2 text_pos = { screen_pos.x - text_size.x * 0.5f, screen_pos.y + y_offset };
			DrawOutlinedText(draw, text_pos, dist_str, color);
		}
	}
}

static void DrawDroppedItems(ImDrawList* draw) noexcept
{
	if (!cfg.world_esp.dropped_items)
		return;

	std::vector<RustItem> cached_items;
	{
		std::lock_guard<std::mutex> lock(g_global_mutex);
		cached_items = global.item_list;
	}
	if (cached_items.empty()) return;

	const ImColor item_color = MakeColor(cfg.world_esp.dropped_items_color);

	for (const auto& item : cached_items)
	{
		if (IsZero(item.position)) continue;

		float distance = camera.CameraPos.Distance(item.position);
		if (distance > cfg.world_esp.dropped_items_max_distance) continue;

		Vector2 screen_pos;
		if (!camera.WorldToScreen(const_cast<Vector3&>(item.position), screen_pos)) continue;

		std::string display_str;
		display_str.reserve(64);

		if (cfg.world_esp.draw_name)
			display_str.append(item.name);

		if (cfg.world_esp.draw_distance)
		{
			if (!display_str.empty()) display_str.append(_(" "));
			display_str.append(_("[") + std::to_string(static_cast<int>(distance)) + _("m]"));
		}

		ImFont* fnt = VisualConstants::GetESPFont();
		const ImVec2 text_size = fnt->CalcTextSizeA(VisualConstants::GetESPFontSize(), FLT_MAX, 0.0f, display_str.c_str());
		const ImVec2 text_pos = { screen_pos.x - text_size.x * 0.5f, screen_pos.y };
		DrawOutlinedText(draw, text_pos, display_str, item_color);
	}
}


void RenderVisuals() noexcept
{
	camera.CameraPos = driver->Read<Vector3>(camera.cam + 0x454);
	camera.ViewMatrix = driver->Read<Matrix>(camera.cam + 0x30C);
	if (players.local_player->GetHeldItem().is_weapon2() && cfg.weapon.reloadbar)
		RenderReloadBar(players.local_player->GetHeldItem().is_reloading(), players.local_player->GetHeldItem().time());
	if (players.local_player->GetHeldItem().is_weapon2() && cfg.weapon.ammocounter)
		RenderAmmoBar(ammo.contents, ammo.capacity);

	if (cfg.player_visuals.itemlist)
		RenderHotbarGUI();

	DrawPlayers(ImGui::GetBackgroundDrawList());
	DrawWorldEntities(ImGui::GetBackgroundDrawList());
	DrawDroppedItems(ImGui::GetBackgroundDrawList());
	DrawRadar(ImGui::GetBackgroundDrawList());
}
