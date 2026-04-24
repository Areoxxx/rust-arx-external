#pragma once

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui.h"
#include "imgui_internal.h"
#include "Config/Config.hxx"

#include <unordered_map>
#include <string>
#include <Windows.h>

#ifndef IM_PI
#define IM_PI 3.14159265358979323846f
#endif

namespace esp_layout
{
    inline int name_side     = 1;
    inline int health_side   = 0;
    inline int weapon_side   = 3;
    inline int distance_side = 2;
}

struct Fonts {
    ImFont* spacegrotesk_medium[3];
    ImFont* inter_semibold[3];
    ImFont* widget_icon[3];
    ImFont* tab_icon;
    ImFont* visualsFont;
    ImFont* visitorFont;
};
inline Fonts font;

// mallah-style tab system
extern int active_tab;
extern int active_subtab[10];
extern float tab_anim;
extern float subtab_anim;

struct helpers
{
    void add_tab();
    void add_subtab(const char* subtabs[], int subtab_count, int subtab_index);
    bool is_tab_clicked(int tab_index, ImVec2 tab_pos, ImVec2 tab_size);
    bool is_subtab_clicked(int subtab_index, ImVec2 subtab_pos, ImVec2 subtab_size);
    bool begin_child(const char* name, ImVec2 size);
    void render();
};

__forceinline void draw_text_shdw(ImVec2 pos, ImU32 color, const char* text)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    dl->AddText(ImVec2(pos.x + 1, pos.y + 1), IM_COL32_BLACK, text);
    dl->AddText(pos, color, text);
}

__forceinline void draw_title(const char* title)
{
    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    ImVec2 wp = ImGui::GetWindowPos();
    ImVec2 ws = ImGui::GetWindowSize();
    float title_height = 24.f;
    float start_x = wp.x;
    float start_y = wp.y - title_height - 2;
    float end_x = wp.x + ws.x;
    dl->AddRectFilled(ImVec2(start_x, start_y), ImVec2(end_x, start_y + title_height), IM_COL32(30, 30, 30, 255));
    dl->AddRect(ImVec2(start_x, start_y), ImVec2(end_x, start_y + title_height), IM_COL32(0, 0, 0, 255));
    dl->AddRect(ImVec2(start_x + 1, start_y + 1), ImVec2(end_x - 1, start_y + title_height - 1), IM_COL32(44, 44, 44, 255));
    dl->AddRect(ImVec2(start_x + 2, start_y + 2), ImVec2(end_x - 2, start_y + title_height - 2), IM_COL32(0, 0, 0, 255));
    ImVec2 text_size = ImGui::CalcTextSize(title);
    ImVec2 text_pos = ImVec2(start_x + 7, start_y + (title_height - text_size.y) * 0.5f);
    dl->AddText(ImVec2(text_pos.x + 1, text_pos.y + 1), IM_COL32_BLACK, title);
    dl->AddText(text_pos, IM_COL32(255, 255, 255, 255), title);
}

__forceinline void draw_watermark()
{
    ImDrawList* dl = ImGui::GetForegroundDrawList();
    float fps = ImGui::GetIO().Framerate;
    char watermark_text[128];
    snprintf(watermark_text, sizeof(watermark_text), "selling | %.0f fps", fps);

    ImVec2 text_size = ImGui::CalcTextSize(watermark_text);
    float padding_x = 10.f;
    float padding_y = 6.f;
    float margin = 10.f;
    ImVec2 display_size = ImGui::GetIO().DisplaySize;

    static ImVec2 wm_pos = ImVec2(display_size.x - text_size.x - padding_x * 2 - margin, margin);
    static bool dragging = false;
    static ImVec2 drag_offset = ImVec2(0, 0);

    float box_w = text_size.x + padding_x * 2;
    float box_h = text_size.y + padding_y * 2;
    ImVec2 mouse = ImGui::GetIO().MousePos;
    bool hovered = mouse.x >= wm_pos.x && mouse.x <= wm_pos.x + box_w &&
                   mouse.y >= wm_pos.y && mouse.y <= wm_pos.y + box_h;

    if (hovered && ImGui::GetIO().MouseClicked[0]) { dragging = true; drag_offset = ImVec2(mouse.x - wm_pos.x, mouse.y - wm_pos.y); }
    if (!ImGui::GetIO().MouseDown[0]) dragging = false;
    if (dragging) {
        wm_pos.x = ImClamp(mouse.x - drag_offset.x, 0.f, display_size.x - box_w);
        wm_pos.y = ImClamp(mouse.y - drag_offset.y, 0.f, display_size.y - box_h);
    }

    float sx = wm_pos.x, sy = wm_pos.y, ex = sx + box_w, ey = sy + box_h;
    dl->AddRectFilled(ImVec2(sx, sy), ImVec2(ex, ey), IM_COL32(30, 30, 30, 255));
    dl->AddRect(ImVec2(sx, sy), ImVec2(ex, ey), IM_COL32(0, 0, 0, 255));
    dl->AddRect(ImVec2(sx+1, sy+1), ImVec2(ex-1, ey-1), IM_COL32(44, 44, 44, 255));
    dl->AddRect(ImVec2(sx+2, sy+2), ImVec2(ex-2, ey-2), IM_COL32(0, 0, 0, 255));

    ImVec2 tp = ImVec2(sx + padding_x, sy + padding_y);
    float cx = tp.x;
    auto accent_col = IM_COL32((int)(cfg.settings.gui_color[0]*255), (int)(cfg.settings.gui_color[1]*255), (int)(cfg.settings.gui_color[2]*255), 255);
    dl->AddText(ImVec2(cx+1, tp.y+1), IM_COL32(0,0,0,255), "selling");
    dl->AddText(ImVec2(cx, tp.y), accent_col, "selling");
    cx += ImGui::CalcTextSize("selling").x;
    dl->AddText(ImVec2(cx+1, tp.y+1), IM_COL32(0,0,0,255), " | ");
    dl->AddText(ImVec2(cx, tp.y), IM_COL32(80,80,80,255), " | ");
    cx += ImGui::CalcTextSize(" | ").x;
    char fps_text[16]; snprintf(fps_text, sizeof(fps_text), "%.0f fps", fps);
    dl->AddText(ImVec2(cx+1, tp.y+1), IM_COL32(0,0,0,255), fps_text);
    dl->AddText(ImVec2(cx, tp.y), IM_COL32(255,255,255,255), fps_text);
}

namespace ui
{
    extern bool menu_open;
    void render_menu();
}
