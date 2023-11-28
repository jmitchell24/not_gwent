#include "IconsFontAwesome5.h"

#include "FA5FreeSolidFontData.h"
#include "rlImGui/imgui/imgui.h"

void rlImGuiAddFontAwesomeIconFonts(float size)
{
    ImGuiIO& io = ImGui::GetIO();
    //io.Fonts->AddFontDefault();

    // merge in icons from FontkAwesome
    static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    io.Fonts->AddFontFromMemoryTTF(FA5Free_Solid_900_otf, FA5Free_Solid_900_otf_len, size, &icons_config, icons_ranges);
}
