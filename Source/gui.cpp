#include "gui.h"

int active_tab = 0;
int active_subtab[10] = { 0 };
float tab_anim = 1.0f;
float subtab_anim = 1.0f;

// --- ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ---

bool helpers::is_tab_clicked(int tab_index, ImVec2 tab_pos, ImVec2 tab_size)
{
    ImVec2 mp = ImGui::GetMousePos();
    bool hov = mp.x >= tab_pos.x && mp.x <= tab_pos.x + tab_size.x &&
        mp.y >= tab_pos.y && mp.y <= tab_pos.y + tab_size.y;
    return hov && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
}

void helpers::add_tab()
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 wp = ImGui::GetWindowPos();
    ImVec2 ws = ImGui::GetWindowSize();

    float tab_width = 80.0f;
    float tab_height = 20.f;
    float left_padding = 3.0f;
    float top_padding = 6.0f;
    float spacing = 5.0f;

    const char* tabs[] = { "aimbot", "players", "world", "weapons", "movement", "environment", "settings" };
    int tab_count = 7;

    float tab_start_x = wp.x + left_padding;
    float current_y = wp.y + top_padding;

    // Фон сайдбара
    dl->AddRectFilled(ImVec2(wp.x + 3, wp.y + 3), ImVec2(wp.x + 83, wp.y + ws.y - 3), IM_COL32(25, 25, 25, 255));

    if (tab_anim < 1.0f) tab_anim += ImGui::GetIO().DeltaTime * 3.0f;
    if (tab_anim > 1.0f) tab_anim = 1.0f;

    for (int i = 0; i < tab_count; i++)
    {
        ImVec2 text_size = ImGui::CalcTextSize(tabs[i]);
        bool is_active = (active_tab == i);

        // Цвет текста с анимацией
        float r = 210 + ((cfg.settings.gui_color[0] * 255) - 210) * (is_active ? tab_anim : 0);
        float g = 210 + ((cfg.settings.gui_color[1] * 255) - 210) * (is_active ? tab_anim : 0);
        float b = 211 + ((cfg.settings.gui_color[2] * 255) - 211) * (is_active ? tab_anim : 0);
        ImU32 text_color = IM_COL32((int)r, (int)g, (int)b, 255);

        if (is_tab_clicked(i, ImVec2(tab_start_x, current_y), ImVec2(tab_width, tab_height)))
        {
            active_tab = i;
            active_subtab[0] = 0; // Сброс подвкладки при смене таба
            tab_anim = 0.0f;
        }

        if (is_active)
        {
            int alpha = (int)(255 * tab_anim);
            dl->AddRectFilled(ImVec2(tab_start_x, current_y), ImVec2(tab_start_x + tab_width, current_y + tab_height), IM_COL32(30, 30, 30, alpha));
            dl->AddLine(ImVec2(tab_start_x, current_y), ImVec2(tab_start_x + tab_width, current_y), IM_COL32(0, 0, 0, alpha));
            dl->AddLine(ImVec2(tab_start_x, current_y + 1), ImVec2(tab_start_x + tab_width + 2, current_y + 1), IM_COL32(44, 44, 44, alpha));
            dl->AddLine(ImVec2(tab_start_x, current_y + tab_height - 1), ImVec2(tab_start_x + tab_width + 2, current_y + tab_height - 1), IM_COL32(44, 44, 44, alpha));
            dl->AddLine(ImVec2(tab_start_x, current_y + tab_height), ImVec2(tab_start_x + tab_width, current_y + tab_height), IM_COL32(0, 0, 0, alpha));
        }

        ImVec2 text_pos = ImVec2(tab_start_x + (tab_width - text_size.x) * 0.5f, current_y + (tab_height - text_size.y) * 0.5f);
        draw_text_shdw(text_pos, text_color, tabs[i]);

        current_y += tab_height + spacing;
    }
}

bool helpers::is_subtab_clicked(int subtab_index, ImVec2 subtab_pos, ImVec2 subtab_size)
{
    ImVec2 mp = ImGui::GetMousePos();
    bool hov = mp.x >= subtab_pos.x && mp.x <= subtab_pos.x + subtab_size.x &&
        mp.y >= subtab_pos.y && mp.y <= subtab_pos.y + subtab_size.y;
    return hov && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
}

void helpers::add_subtab(const char* subtabs[], int subtab_count, int subtab_index)
{
    ImDrawList* dl = ImGui::GetWindowDrawList();
    ImVec2 wp = ImGui::GetWindowPos();
    ImVec2 ws = ImGui::GetWindowSize();

    float placeholder_height = 24.0f;
    float start_x = 83 + 8; // Отступ от сайдбара
    float start_y = 9;      // Фиксированный Y для субтабов
    float end_x = ws.x - 9;

    dl->AddRectFilled(ImVec2(wp.x + start_x, wp.y + start_y), ImVec2(wp.x + end_x, wp.y + start_y + placeholder_height), IM_COL32(25, 25, 25, 255));
    dl->AddRect(ImVec2(wp.x + start_x, wp.y + start_y), ImVec2(wp.x + end_x, wp.y + start_y + placeholder_height), IM_COL32(0, 0, 0, 255));
    dl->AddRect(ImVec2(wp.x + start_x + 1, wp.y + start_y + 1), ImVec2(wp.x + end_x - 1, wp.y + start_y + placeholder_height - 1), IM_COL32(44, 44, 44, 255));

    float available_width = end_x - start_x;
    float subtab_width = available_width / subtab_count;

    if (subtab_anim < 1.0f) subtab_anim += ImGui::GetIO().DeltaTime * 3.0f;
    if (subtab_anim > 1.0f) subtab_anim = 1.0f;

    for (int i = 0; i < subtab_count; i++)
    {
        ImVec2 text_size = ImGui::CalcTextSize(subtabs[i]);
        float current_x = wp.x + start_x + (i * subtab_width);
        bool is_active = (active_subtab[subtab_index] == i);

        float r = 210 + ((cfg.settings.gui_color[0] * 255) - 210) * (is_active ? subtab_anim : 0);
        float g = 210 + ((cfg.settings.gui_color[1] * 255) - 210) * (is_active ? subtab_anim : 0);
        float b = 211 + ((cfg.settings.gui_color[2] * 255) - 211) * (is_active ? subtab_anim : 0);
        ImU32 text_color = IM_COL32((int)r, (int)g, (int)b, 255);

        if (is_subtab_clicked(i, ImVec2(current_x, wp.y + start_y), ImVec2(subtab_width, placeholder_height)))
        {
            active_subtab[subtab_index] = i;
            subtab_anim = 0.0f;
        }

        ImVec2 text_pos = ImVec2(current_x + (subtab_width - text_size.x) * 0.5f, wp.y + start_y + (placeholder_height - text_size.y) * 0.5f);
        draw_text_shdw(text_pos, text_color, subtabs[i]);
    }
}

bool helpers::begin_child(const char* name, ImVec2 size)
{
    ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(0, 0, 0, 255));
    ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(25, 25, 25, 255));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
    bool result = ImGui::BeginChild("", name, size, true, ImGuiWindowFlags_NoScrollbar);
    if (result)
    {
        ImDrawList* dl = ImGui::GetWindowDrawList();
        ImVec2 child_wp = ImGui::GetWindowPos();
        ImVec2 child_ws = ImGui::GetWindowSize();
        dl->AddRect(ImVec2(child_wp.x + 1, child_wp.y + 1), ImVec2(child_wp.x + child_ws.x - 1, child_wp.y + child_ws.y - 1), IM_COL32(44, 44, 44, 255));
    }
    return result;
}

// --- ОСНОВНОЙ РЕНДЕР ---

void helpers::render()
{
    ImVec2 ws = ImGui::GetWindowSize();

    // Настройки сетки (единые для всех вкладок)
    float start_x = 83.0f + 15.0f;     // Сайдбар + отступ
    float child_start_y = 44.0f;       // Высота под субтабами
    float child_width = 245.0f;        // Ширина одной колонки
    float child_height = 375.0f;       // Высота колонки
    float gap = 15.0f;                 // Зазор между колонками
    float item_w = child_width * 0.65f; // Ширина элементов внутри (ползунки и тд)

    switch (active_tab)
    {
    case 0: // AIMBOT
    {
        const char* aim_subtabs[] = { "general", "visuals" };
        add_subtab(aim_subtabs, 2, 0);

        switch (active_subtab[0])
        {
        case 0: // General
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##aim_gen_1", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("enabled", &cfg.aimbot.enable);
                ImGui::Checkbox("silent aim", &cfg.aimbot.silentaim);
                ImGui::Checkbox("prediction", &cfg.aimbot.prediction);
                ImGui::Spacing();
                const char* bones[] = { "head", "neck", "spine", "pelvis" };
                ImGui::Combo("target bone", &cfg.aimbot.selected_bone, bones, IM_ARRAYSIZE(bones));
                ImGui::SliderFloat("fov", &cfg.aimbot.fov, 10.0f, 500.0f, "%.1f");
                ImGui::SliderInt("hitchance", &cfg.aimbot.hitchance, 1, 100, "%d%%");
                ImGui::SliderFloat("max distance", &cfg.aimbot.max_distance, 0.0f, 400.0f, "%.0f");
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);

            ImGui::SetCursorPos(ImVec2(start_x + child_width + gap, child_start_y));
            if (begin_child("##aim_gen_2", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("smooth", &cfg.aimbot.smooth);
                if (cfg.aimbot.smooth)
                    ImGui::SliderFloat("smoothness", &cfg.aimbot.smoothness, 1.0f, 20.0f, "%.1f");
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 1: // Visuals
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##aim_vis", ImVec2(child_width, child_height))) {
                ImGui::Checkbox("draw fov circle", &cfg.aimbot.showfov);
                ImGui::SameLine(child_width - 30);
                ImGui::ColorEdit4("##fovcol", cfg.aimbot.fov_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                ImGui::Checkbox("show aim line", &cfg.aimbot.show_aim_line);
                ImGui::SameLine(child_width - 30);
                ImGui::ColorEdit4("##alinecol", cfg.aimbot.aim_line_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;
        }
        break;
    }

    case 1: // PLAYERS
    {
        const char* plr_subtabs[] = { "esp", "radar", "filter", "layers" };
        add_subtab(plr_subtabs, 4, 0);

        switch (active_subtab[0])
        {
        case 0: // ESP
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##plr_esp_1", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("box esp", &cfg.player_visuals.box);
                ImGui::SameLine(child_width - 30);
                ImGui::ColorEdit4("##bcol", cfg.player_visuals.box_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                const char* btypes[] = { "2d", "corner" };
                ImGui::Combo("box type", &cfg.player_visuals.box_type, btypes, IM_ARRAYSIZE(btypes));
                ImGui::Checkbox("name", &cfg.player_visuals.show_name);
                ImGui::Checkbox("distance", &cfg.player_visuals.show_distance);
                ImGui::SameLine(child_width - 30);
                ImGui::ColorEdit4("##dcol", cfg.player_visuals.distance_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                ImGui::Checkbox("health bar", &cfg.player_visuals.show_health);
                ImGui::Checkbox("team id", &cfg.player_visuals.show_team_id);
                ImGui::Checkbox("weapon", &cfg.player_visuals.weapon);
                ImGui::SameLine(child_width - 30);
                ImGui::ColorEdit4("##wcol", cfg.player_visuals.weapon_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);

            ImGui::SetCursorPos(ImVec2(start_x + child_width + gap, child_start_y));
            if (begin_child("##plr_esp_2", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("skeleton", &cfg.player_visuals.bone_esp);
                ImGui::SameLine(child_width - 30);
                ImGui::ColorEdit4("##skcol", cfg.player_visuals.bone_esp_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

                ImGui::SliderFloat("max distance", &cfg.player_visuals.max_distance, 50.0f, 500.0f, "%.0f");
                const char* fonts[] = { "minecraftia", "space grotesk", "inter", "default", "visitor" };
                ImGui::Combo("font", &cfg.player_visuals.esp_font, fonts, IM_ARRAYSIZE(fonts));
                ImGui::SliderFloat("font size", &cfg.player_visuals.esp_font_size, 10.0f, 24.0f, "%.0f");
                ImGui::Checkbox("hotbar list", &cfg.player_visuals.itemlist);
                ImGui::Checkbox("clothing bar", &cfg.player_visuals.clothing_bar);
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 1: // Radar
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##plr_radar", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("enable radar", &cfg.player_visuals.radar);
                ImGui::SliderFloat("range", &cfg.player_visuals.radar_range, 50.0f, 300.0f, "%.0f");
                ImGui::SliderFloat("size", &cfg.player_visuals.radar_size, 80.0f, 250.0f, "%.0f");
                ImGui::SliderFloat("zoom", &cfg.player_visuals.radar_zoom, 0.5f, 3.0f, "%.1f");
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 2: // Filter
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##plr_filter", ImVec2(child_width, child_height))) {
                ImGui::Checkbox("ignore sleepers", &cfg.player_visuals.ignore_sleepers);
                ImGui::Checkbox("ignore npcs", &cfg.player_visuals.ignore_npcs);
                ImGui::Checkbox("ignore teammates", &cfg.player_visuals.ignore_teammates);
                ImGui::Checkbox("ignore friends", &cfg.player_visuals.ignore_friends);
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 3: // Layers
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##plr_layers", ImVec2(child_width, child_height))) {
                ImGui::Checkbox("enable layer removal", &cfg.player_visuals.remove_layer);
                if (cfg.player_visuals.remove_layer) {
                    ImGui::Checkbox("construction", &cfg.player_visuals.remove_construction);
                    ImGui::Checkbox("terrain", &cfg.player_visuals.remove_terrain);
                    ImGui::Checkbox("trees", &cfg.player_visuals.remove_tree);
                    ImGui::Checkbox("clutter", &cfg.player_visuals.remove_clutter);
                    ImGui::Checkbox("debris", &cfg.player_visuals.remove_debris);
                    ImGui::Checkbox("deployables", &cfg.player_visuals.remove_deployed);
                    ImGui::Checkbox("transparent", &cfg.player_visuals.remove_transparent);
                    ImGui::Checkbox("vehicles", &cfg.player_visuals.remove_vehicle_large);
                    ImGui::Checkbox("building block", &cfg.player_visuals.remove_prevent_building);
                }
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;
        }
        break;
    }

    case 2: // WORLD
    {
        const char* world_subtabs[] = { "general", "crates", "traps", "resources" };
        add_subtab(world_subtabs, 4, 0);

        switch (active_subtab[0])
        {
        case 0: // General
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##world_gen_1", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("draw name", &cfg.world_esp.draw_name);
                ImGui::Checkbox("draw distance", &cfg.world_esp.draw_distance);
                ImGui::SliderFloat("max distance", &cfg.world_esp.max_distance, 0.0f, 500.0f, "%.0f");
                ImGui::ColorEdit4("default color", cfg.world_esp.color, ImGuiColorEditFlags_NoInputs);
                ImGui::Spacing();
                ImGui::Checkbox("corpses", &cfg.world_esp.corpse);
                ImGui::Checkbox("caves", &cfg.world_esp.caves);
                ImGui::Checkbox("animals", &cfg.world_esp.animals);
                if (cfg.world_esp.animals)
                    ImGui::SliderFloat("animals dist", &cfg.world_esp.animals_max_distance, 50.0f, 500.0f, "%.0f");
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);

            ImGui::SetCursorPos(ImVec2(start_x + child_width + gap, child_start_y));
            if (begin_child("##world_gen_2", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("dropped items", &cfg.world_esp.dropped_items);
                ImGui::SameLine(child_width - 30);
                ImGui::ColorEdit4("##dicol", cfg.world_esp.dropped_items_color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                ImGui::SliderFloat("items dist", &cfg.world_esp.dropped_items_max_distance, 10.0f, 200.0f, "%.0f");
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 1: // Crates
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##world_crates", ImVec2(child_width, child_height))) {
                ImGui::Checkbox("elite crate", &cfg.world_esp.elite_crate);
                ImGui::Checkbox("military crate", &cfg.world_esp.military_crate);
                ImGui::Checkbox("crate", &cfg.world_esp.crate);
                ImGui::Checkbox("locked crate", &cfg.world_esp.locked_crate);
                ImGui::Checkbox("airdrop", &cfg.world_esp.airdrop);
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 2: // Traps
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##world_traps", ImVec2(child_width, child_height))) {
                ImGui::Checkbox("auto turret", &cfg.world_esp.auto_turret);
                ImGui::Checkbox("flame turret", &cfg.world_esp.flame_turret);
                ImGui::Checkbox("shotgun trap", &cfg.world_esp.shotgun_trap);
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 3: // Resources
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##world_res", ImVec2(child_width, child_height))) {
                ImGui::Checkbox("stone", &cfg.world_esp.stone);
                ImGui::Checkbox("metal", &cfg.world_esp.metal);
                ImGui::Checkbox("sulfur", &cfg.world_esp.sulfur);
                ImGui::Checkbox("hemp", &cfg.world_esp.hemp);
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;
        }
        break;
    }

    case 3: // WEAPONS
    {
        const char* wpn_subtabs[] = { "mods", "chams", "hud" };
        add_subtab(wpn_subtabs, 3, 0);

        switch (active_subtab[0])
        {
        case 0: // Mods
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##wpn_mods", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("no recoil", &cfg.weapon.norecoil);
                ImGui::SliderInt("recoil scale", &cfg.weapon.recoil_scale, 0, 100, "%d%%");
                ImGui::Checkbox("no spread", &cfg.weapon.nospread);
                ImGui::SliderInt("spread scale", &cfg.weapon.spread, 0, 100, "%d%%");
                ImGui::Spacing();
                ImGui::Checkbox("fast bow", &cfg.weapon.fast_bow);
                ImGui::Checkbox("eoka instant", &cfg.weapon.eoka);
                ImGui::Checkbox("no pullback", &cfg.weapon.no_pullback);
                ImGui::Checkbox("long hand", &cfg.weapon.long_hand);
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 1: // Chams
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##wpn_chams", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("hand chams", &cfg.weapon.hand_chams);
                if (cfg.weapon.hand_chams)
                    ImGui::SliderInt("hand type", &cfg.weapon.hands_type, 0, 16);
                ImGui::Checkbox("weapon chams", &cfg.weapon.weapon_chams);
                if (cfg.weapon.weapon_chams)
                    ImGui::SliderInt("weapon type", &cfg.weapon.weapon_type, 0, 16);
                ImGui::Checkbox("player chams", &cfg.weapon.player_chams);
                if (cfg.weapon.player_chams)
                    ImGui::SliderInt("player type", &cfg.weapon.player_chams_type, 0, 20);
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 2: // HUD
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##wpn_hud", ImVec2(child_width, child_height))) {
                ImGui::Checkbox("ammo counter", &cfg.weapon.ammocounter);
                ImGui::Checkbox("reload bar", &cfg.weapon.reloadbar);
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;
        }
        break;
    }

    case 4: // MOVEMENT
    {
        const char* mov_subtabs[] = { "movement", "exploits", "freecam" };
        add_subtab(mov_subtabs, 3, 0);

        switch (active_subtab[0])
        {
        case 0: // General
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##mov_gen", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("speedhack", &cfg.movement.speed);
                ImGui::SliderFloat("speed", &cfg.movement.speed_value, 1.0f, 20.0f, "%.1f");
                ImGui::Checkbox("spider / highjump", &cfg.movement.highjump);
                ImGui::SliderFloat("jump angle", &cfg.movement.highjump_angle, 10.0f, 90.0f, "%.0f");
                ImGui::Checkbox("fly", &cfg.movement.fly);
                ImGui::SliderFloat("fly speed", &cfg.movement.fly_speed, 1.0f, 50.0f, "%.1f");
                ImGui::Checkbox("fly safe mode", &cfg.movement.fly_safe_mode);
                ImGui::Checkbox("shoot in air", &cfg.movement.shoot_in_air);
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 1: // Exploits
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##mov_exploits", ImVec2(child_width, child_height))) {
                ImGui::Checkbox("fake admin", &cfg.exploits.fake_admin);
                ImGui::Checkbox("third person", &cfg.exploits.thirdperson);
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 2: // Freecam
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##mov_freecam", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("freecam", &cfg.movement.freecam);
                ImGui::SliderFloat("freecam speed", &cfg.movement.freecam_speed, 1.0f, 50.0f, "%.1f");
                ImGui::SliderFloat("sensitivity", &cfg.movement.freecam_sensitivity, 0.01f, 1.0f, "%.2f");
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;
        }
        break;
    }

    case 5: // ENVIRONMENT
    {
        const char* env_subtabs[] = { "sky", "fx" };
        add_subtab(env_subtabs, 2, 0);

        switch (active_subtab[0])
        {
        case 0: // Sky
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##env_sky_1", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("custom time", &cfg.sky.tod_sky);
                ImGui::SliderFloat("time", &cfg.sky.time_of_day, 0.0f, 24.0f, "%.1f");
                ImGui::SliderFloat("night bright", &cfg.sky.night_brightness, 0.0f, 10.0f, "%.1f");
                ImGui::Checkbox("bright night", &cfg.sky.bright_night);
                ImGui::Spacing();
                ImGui::Checkbox("custom sky color", &cfg.sky.custom_sky_color);
                if (cfg.sky.custom_sky_color)
                    ImGui::ColorEdit4("sky color", cfg.sky.sky_color, ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox("custom ambient", &cfg.sky.custom_ambient);
                if (cfg.sky.custom_ambient)
                    ImGui::ColorEdit4("ambient", cfg.sky.ambient_color, ImGuiColorEditFlags_NoInputs);
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);

            ImGui::SetCursorPos(ImVec2(start_x + child_width + gap, child_start_y));
            if (begin_child("##env_sky_2", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("custom fog", &cfg.sky.custom_fog);
                if (cfg.sky.custom_fog)
                    ImGui::SliderFloat("fogginess", &cfg.sky.fogginess, 0.0f, 1.0f, "%.1f");
                ImGui::Checkbox("custom stars", &cfg.sky.custom_stars);
                if (cfg.sky.custom_stars) {
                    ImGui::SliderFloat("star size", &cfg.sky.star_size, 0.0f, 10.0f, "%.1f");
                    ImGui::SliderFloat("star bright", &cfg.sky.star_brightness, 0.0f, 10.0f, "%.1f");
                }
                ImGui::Checkbox("custom sky image", &cfg.sky.custom_sky_image);
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 1: // FX
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##env_fx", ImVec2(child_width, child_height))) {
                ImGui::TextDisabled("No effects available yet...");
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;
        }
        break;
    }

    case 6: // SETTINGS
    {
        const char* set_subtabs[] = { "general", "camera" };
        add_subtab(set_subtabs, 2, 0);

        switch (active_subtab[0])
        {
        case 0: // General
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##set_gen_1", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::ColorEdit4("accent color", cfg.settings.gui_color, ImGuiColorEditFlags_NoInputs);
                ImGui::Checkbox("crosshair", &cfg.settings.crosshair);
                ImGui::Checkbox("streamproof", &cfg.settings.streamproof);
                ImGui::Checkbox("watermark", &cfg.settings.watermark);
                ImGui::Checkbox("fps counter", &cfg.settings.fps_counter);
                ImGui::SliderInt("fps limit", &cfg.settings.fps_limit, 30, 500);
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);

            ImGui::SetCursorPos(ImVec2(start_x + child_width + gap, child_start_y));
            if (begin_child("##set_gen_2", ImVec2(child_width, child_height))) {
                if (ImGui::Button("save config", ImVec2(child_width - 16, 30)))
                    SaveConfig("config.json");
                ImGui::Spacing();
                if (ImGui::Button("load config", ImVec2(child_width - 16, 30)))
                    LoadConfig("config.json");
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;

        case 1: // Camera
            ImGui::SetCursorPos(ImVec2(start_x, child_start_y));
            if (begin_child("##set_cam", ImVec2(child_width, child_height))) {
                ImGui::PushItemWidth(item_w);
                ImGui::Checkbox("fov changer", &cfg.settings.fov_changer);
                if (cfg.settings.fov_changer)
                    ImGui::SliderFloat("game fov", &cfg.settings.game_fov, 60.0f, 150.0f, "%.0f");
                ImGui::Checkbox("zoom", &cfg.settings.zoom);
                ImGui::PopItemWidth();
            }
            ImGui::EndChild(); ImGui::PopStyleVar(); ImGui::PopStyleColor(2);
            break;
        }
        break;
    }
    }
}

// --- ОТРИСОВКА МЕНЮ ---

namespace ui
{
    bool menu_open = false;

    void render_menu()
    {
        if (!menu_open) return;

        ImGuiStyle* style = &ImGui::GetStyle();
        style->WindowRounding = 0.0f;
        style->FrameRounding = 2.0f;
        style->ChildRounding = 0.0f;
        style->WindowPadding = ImVec2(0, 0);

        ImVec4* colors = style->Colors;
        colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20, 255);
        colors[ImGuiCol_ChildBg] = ImColor(25, 25, 25, 255);
        colors[ImGuiCol_FrameBg] = ImColor(30, 30, 30, 255);

        ImVec4 accent = ImVec4(cfg.settings.gui_color[0], cfg.settings.gui_color[1], cfg.settings.gui_color[2], 1.0f);
        colors[ImGuiCol_CheckMark] = accent;
        colors[ImGuiCol_SliderGrab] = accent;
        colors[ImGuiCol_SliderGrabActive] = ImColor(accent.x * 0.8f, accent.y * 0.8f, accent.z * 0.8f, 1.0f);

        ImGui::SetNextWindowSize(ImVec2(620, 440), ImGuiCond_Always);
        ImGui::Begin("##Selling", &menu_open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);

        ImVec2 wp = ImGui::GetWindowPos();
        ImVec2 ws = ImGui::GetWindowSize();
        ImDrawList* dl = ImGui::GetWindowDrawList();

        // Отрисовка основной рамки и фона контента
        dl->AddRectFilled(ImVec2(wp.x + 83, wp.y + 3), ImVec2(wp.x + ws.x - 3, wp.y + ws.y - 3), IM_COL32(30, 30, 30, 255));
        dl->AddRect(wp, ImVec2(wp.x + ws.x, wp.y + ws.y), IM_COL32(0, 0, 0, 255), 0.0f, 0, 1.0f);

        helpers _helper;
        _helper.add_tab();

        // Линии разграничения сайдбара
        dl->AddLine(ImVec2(wp.x + 83, wp.y + 3), ImVec2(wp.x + 83, wp.y + ws.y - 3), IM_COL32(0, 0, 0, 255));
        dl->AddLine(ImVec2(wp.x + 84, wp.y + 3), ImVec2(wp.x + 84, wp.y + ws.y - 3), IM_COL32(44, 44, 44, 255));

        _helper.render();

        draw_title("SELLING");
        if (cfg.settings.watermark) draw_watermark();

        ImGui::End();
    }
}
