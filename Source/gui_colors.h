#pragma once
#include <imgui.h>

namespace ui
{
	namespace colors
	{
		inline ImVec4 background = ImColor(11, 11, 14, 255);
		inline ImVec4 background_dark = ImColor(7, 7, 9, 255);
		inline ImVec4 background_light = ImColor(18, 18, 24, 255);

		inline ImVec4 top_color = ImColor(15, 16, 20, 255);
		inline ImVec4 bottom_color = ImColor(10, 11, 14, 255);

		inline ImVec4 sidebar_top = ImColor(18, 19, 26, 255);
		inline ImVec4 sidebar_bottom = ImColor(11, 12, 17, 255);

		inline ImVec4 outline = ImColor(255, 255, 255, 12);
		inline ImVec4 main = ImVec4(0.47f, 0.37f, 1.0f, 1.0f);
		inline ImVec4 text = ImColor(252, 252, 254, 255);
		inline ImVec4 text_disabled = ImColor(105, 110, 130, 255);

		namespace child
		{
			inline ImVec4 top_background = ImColor(24, 25, 34, 100);
			inline ImVec4 bottom_background = ImColor(14, 15, 20, 255);
			inline ImVec4 outline = ImColor(255, 255, 255, 10);
		};

		namespace checkbox
		{
			inline ImVec4 circle_inactive = ImColor(35, 36, 42, 255);
			inline ImVec4 background = ImColor(18, 18, 24, 255);
			inline ImVec4 outline_background = ImColor(255, 255, 255, 12);
			inline float rounding = 30;
		}

		namespace slider
		{
			inline ImVec4 background = ImColor(18, 18, 24, 255);
			inline ImVec4 outline_background = ImColor(255, 255, 255, 12);
			inline ImVec4 grabber = ImColor(252, 252, 254, 255);
		}

		namespace combo
		{
			inline ImVec4 background = ImColor(18, 18, 24, 255);
			inline ImVec4 outline_background = ImColor(255, 255, 255, 12);
		}

		namespace keybind
		{
			inline ImVec4 background = ImColor(18, 18, 24, 255);
			inline ImVec4 outline_background = ImColor(255, 255, 255, 12);
		}

		namespace button
		{
			inline ImVec4 background = ImColor(18, 18, 24, 255);
			inline ImVec4 background_light = ImColor(24, 25, 33, 255);
			inline ImVec4 outline_background = ImColor(255, 255, 255, 12);
		}

		namespace input
		{
			inline ImVec4 background = ImColor(18, 18, 24, 255);
			inline ImVec4 background_light = ImColor(24, 25, 33, 255);
			inline ImVec4 outline_background = ImColor(255, 255, 255, 12);
		}

		namespace picker
		{
			inline ImVec4 background = ImColor(14, 15, 20, 255);
			inline ImVec4 outline_background = ImColor(255, 255, 255, 12);
		}

		inline ImU32 MainAlpha(int alpha) { return ImGui::GetColorU32(ImVec4(main.x, main.y, main.z, alpha / 255.0f)); }
	};
};
