/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   rlImGui * basic ImGui integration
*
*   LICENSE: ZLIB
*
*   Copyright (c) 2020 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/
#include "rlImGui.h"

#include "imgui/imgui.h"
#include <raylib.h>
#include <rlgl.h>

#include <math.h>
#include <vector>
#include <map>

#define ENUM_MOUSE_CURSOR_MAP \
    CASE(ImGuiMouseCursor_Arrow      , MOUSE_CURSOR_ARROW) \
    CASE(ImGuiMouseCursor_TextInput  , MOUSE_CURSOR_IBEAM) \
    CASE(ImGuiMouseCursor_Hand       , MOUSE_CURSOR_POINTING_HAND) \
    CASE(ImGuiMouseCursor_ResizeAll  , MOUSE_CURSOR_RESIZE_ALL) \
    CASE(ImGuiMouseCursor_ResizeEW   , MOUSE_CURSOR_RESIZE_EW) \
    CASE(ImGuiMouseCursor_ResizeNESW , MOUSE_CURSOR_RESIZE_NESW) \
    CASE(ImGuiMouseCursor_ResizeNS   , MOUSE_CURSOR_RESIZE_NS) \
    CASE(ImGuiMouseCursor_ResizeNWSE , MOUSE_CURSOR_RESIZE_NWSE) \
    CASE(ImGuiMouseCursor_NotAllowed , MOUSE_CURSOR_NOT_ALLOWED)

#define ENUM_KEY_MAP \
    CASE(ImGuiKey_Apostrophe    , KEY_APOSTROPHE    ) \
    CASE(ImGuiKey_Comma         , KEY_COMMA         ) \
    CASE(ImGuiKey_Minus         , KEY_MINUS         ) \
    CASE(ImGuiKey_Period        , KEY_PERIOD        ) \
    CASE(ImGuiKey_Slash         , KEY_SLASH         ) \
    CASE(ImGuiKey_0             , KEY_ZERO          ) \
    CASE(ImGuiKey_1             , KEY_ONE           ) \
    CASE(ImGuiKey_2             , KEY_TWO           ) \
    CASE(ImGuiKey_3             , KEY_THREE         ) \
    CASE(ImGuiKey_4             , KEY_FOUR          ) \
    CASE(ImGuiKey_5             , KEY_FIVE          ) \
    CASE(ImGuiKey_6             , KEY_SIX           ) \
    CASE(ImGuiKey_7             , KEY_SEVEN         ) \
    CASE(ImGuiKey_8             , KEY_EIGHT         ) \
    CASE(ImGuiKey_9             , KEY_NINE          ) \
    CASE(ImGuiKey_Semicolon     , KEY_SEMICOLON     ) \
    CASE(ImGuiKey_Equal         , KEY_EQUAL         ) \
    CASE(ImGuiKey_A             , KEY_A             ) \
    CASE(ImGuiKey_B             , KEY_B             ) \
    CASE(ImGuiKey_C             , KEY_C             ) \
    CASE(ImGuiKey_D             , KEY_D             ) \
    CASE(ImGuiKey_E             , KEY_E             ) \
    CASE(ImGuiKey_F             , KEY_F             ) \
    CASE(ImGuiKey_G             , KEY_G             ) \
    CASE(ImGuiKey_H             , KEY_H             ) \
    CASE(ImGuiKey_I             , KEY_I             ) \
    CASE(ImGuiKey_J             , KEY_J             ) \
    CASE(ImGuiKey_K             , KEY_K             ) \
    CASE(ImGuiKey_L             , KEY_L             ) \
    CASE(ImGuiKey_M             , KEY_M             ) \
    CASE(ImGuiKey_N             , KEY_N             ) \
    CASE(ImGuiKey_O             , KEY_O             ) \
    CASE(ImGuiKey_P             , KEY_P             ) \
    CASE(ImGuiKey_Q             , KEY_Q             ) \
    CASE(ImGuiKey_R             , KEY_R             ) \
    CASE(ImGuiKey_S             , KEY_S             ) \
    CASE(ImGuiKey_T             , KEY_T             ) \
    CASE(ImGuiKey_U             , KEY_U             ) \
    CASE(ImGuiKey_V             , KEY_V             ) \
    CASE(ImGuiKey_W             , KEY_W             ) \
    CASE(ImGuiKey_X             , KEY_X             ) \
    CASE(ImGuiKey_Y             , KEY_Y             ) \
    CASE(ImGuiKey_Z             , KEY_Z             ) \
    CASE(ImGuiKey_Space         , KEY_SPACE         ) \
    CASE(ImGuiKey_Escape        , KEY_ESCAPE        ) \
    CASE(ImGuiKey_Enter         , KEY_ENTER         ) \
    CASE(ImGuiKey_Tab           , KEY_TAB           ) \
    CASE(ImGuiKey_Backspace     , KEY_BACKSPACE     ) \
    CASE(ImGuiKey_Insert        , KEY_INSERT        ) \
    CASE(ImGuiKey_Delete        , KEY_DELETE        ) \
    CASE(ImGuiKey_RightArrow    , KEY_RIGHT         ) \
    CASE(ImGuiKey_LeftArrow     , KEY_LEFT          ) \
    CASE(ImGuiKey_DownArrow     , KEY_DOWN          ) \
    CASE(ImGuiKey_UpArrow       , KEY_UP            ) \
    CASE(ImGuiKey_PageUp        , KEY_PAGE_UP       ) \
    CASE(ImGuiKey_PageDown      , KEY_PAGE_DOWN     ) \
    CASE(ImGuiKey_Home          , KEY_HOME          ) \
    CASE(ImGuiKey_End           , KEY_END           ) \
    CASE(ImGuiKey_CapsLock      , KEY_CAPS_LOCK     ) \
    CASE(ImGuiKey_ScrollLock    , KEY_SCROLL_LOCK   ) \
    CASE(ImGuiKey_NumLock       , KEY_NUM_LOCK      ) \
    CASE(ImGuiKey_PrintScreen   , KEY_PRINT_SCREEN  ) \
    CASE(ImGuiKey_Pause         , KEY_PAUSE         ) \
    CASE(ImGuiKey_F1            , KEY_F1            ) \
    CASE(ImGuiKey_F2            , KEY_F2            ) \
    CASE(ImGuiKey_F3            , KEY_F3            ) \
    CASE(ImGuiKey_F4            , KEY_F4            ) \
    CASE(ImGuiKey_F5            , KEY_F5            ) \
    CASE(ImGuiKey_F6            , KEY_F6            ) \
    CASE(ImGuiKey_F7            , KEY_F7            ) \
    CASE(ImGuiKey_F8            , KEY_F8            ) \
    CASE(ImGuiKey_F9            , KEY_F9            ) \
    CASE(ImGuiKey_F10           , KEY_F10           ) \
    CASE(ImGuiKey_F11           , KEY_F11           ) \
    CASE(ImGuiKey_F12           , KEY_F12           ) \
    CASE(ImGuiKey_LeftShift     , KEY_LEFT_SHIFT    ) \
    CASE(ImGuiKey_LeftCtrl      , KEY_LEFT_CONTROL  ) \
    CASE(ImGuiKey_LeftAlt       , KEY_LEFT_ALT      ) \
    CASE(ImGuiKey_LeftSuper     , KEY_LEFT_SUPER    ) \
    CASE(ImGuiKey_RightShift    , KEY_RIGHT_SHIFT   ) \
    CASE(ImGuiKey_RightCtrl     , KEY_RIGHT_CONTROL ) \
    CASE(ImGuiKey_RightAlt      , KEY_RIGHT_ALT     ) \
    CASE(ImGuiKey_RightSuper    , KEY_RIGHT_SUPER   ) \
    CASE(ImGuiKey_Menu          , KEY_KB_MENU       ) \
    CASE(ImGuiKey_LeftBracket   , KEY_LEFT_BRACKET  ) \
    CASE(ImGuiKey_Backslash     , KEY_BACKSLASH     ) \
    CASE(ImGuiKey_RightBracket  , KEY_RIGHT_BRACKET ) \
    CASE(ImGuiKey_GraveAccent   , KEY_GRAVE         ) \
    CASE(ImGuiKey_Keypad0       , KEY_KP_0          ) \
    CASE(ImGuiKey_Keypad1       , KEY_KP_1          ) \
    CASE(ImGuiKey_Keypad2       , KEY_KP_2          ) \
    CASE(ImGuiKey_Keypad3       , KEY_KP_3          ) \
    CASE(ImGuiKey_Keypad4       , KEY_KP_4          ) \
    CASE(ImGuiKey_Keypad5       , KEY_KP_5          ) \
    CASE(ImGuiKey_Keypad6       , KEY_KP_6          ) \
    CASE(ImGuiKey_Keypad7       , KEY_KP_7          ) \
    CASE(ImGuiKey_Keypad8       , KEY_KP_8          ) \
    CASE(ImGuiKey_Keypad9       , KEY_KP_9          ) \
    CASE(ImGuiKey_KeypadDecimal , KEY_KP_DECIMAL    ) \
    CASE(ImGuiKey_KeypadDivide  , KEY_KP_DIVIDE     ) \
    CASE(ImGuiKey_KeypadMultiply, KEY_KP_MULTIPLY   ) \
    CASE(ImGuiKey_KeypadSubtract, KEY_KP_SUBTRACT   ) \
    CASE(ImGuiKey_KeypadAdd     , KEY_KP_ADD        ) \
    CASE(ImGuiKey_KeypadEnter   , KEY_KP_ENTER      ) \
    CASE(ImGuiKey_KeypadEqual   , KEY_KP_EQUAL      )

struct Globals
{
    std::vector<Texture> LoadedTextures;
    Texture2D FontTexture;

    ImGuiMouseCursor CurrentMouseCursor = ImGuiMouseCursor_COUNT;

    static Globals& instance()
    {
        static Globals x;
        return x;
    }
};

static Globals& GLOBALS = Globals::instance();



static const char* rlImGuiGetClipText(void*)
{
    return GetClipboardText();
}

static void rlImGuiSetClipText(void*, const char* text)
{
    SetClipboardText(text);
}

static void rlImGuiNewFrame()
{
    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize.x = float(GetScreenWidth());
    io.DisplaySize.y = float(GetScreenHeight());

    io.DeltaTime = GetFrameTime();

//    io.KeyCtrl  = IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
//    io.KeyShift = IsKeyDown(KEY_RIGHT_SHIFT)   || IsKeyDown(KEY_LEFT_SHIFT);
//    io.KeyAlt   = IsKeyDown(KEY_RIGHT_ALT)     || IsKeyDown(KEY_LEFT_ALT);
//    io.KeySuper = IsKeyDown(KEY_RIGHT_SUPER)   || IsKeyDown(KEY_LEFT_SUPER);

    if (io.WantSetMousePos)
    {
        SetMousePosition((int)io.MousePos.x, (int)io.MousePos.y);
    }
    else
    {
        io.MousePos.x = (float)GetMouseX();
        io.MousePos.y = (float)GetMouseY();
    }

    io.MouseDown[0] = IsMouseButtonDown(MOUSE_LEFT_BUTTON);
    io.MouseDown[1] = IsMouseButtonDown(MOUSE_RIGHT_BUTTON);
    io.MouseDown[2] = IsMouseButtonDown(MOUSE_MIDDLE_BUTTON);

    if (GetMouseWheelMove() > 0)
        io.MouseWheel += 1;
    else if (GetMouseWheelMove() < 0)
        io.MouseWheel -= 1;

    if ((io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange) == 0)
    {
        ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();

        if (GLOBALS.CurrentMouseCursor != imgui_cursor)
        {
            GLOBALS.CurrentMouseCursor = imgui_cursor;
            if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
            {
                HideCursor();
            }
            else
            {
                ShowCursor();

                switch (imgui_cursor)
                {
#define CASE(x_, y_) case x_: SetMouseCursor(y_); break;
ENUM_MOUSE_CURSOR_MAP
#undef CASE
                }
            }
        }


    }
}

static void rlImGuiEvents()
{
    ImGuiIO& io = ImGui::GetIO();

    io.AddKeyEvent(ImGuiKey_ModCtrl,  IsKeyDown(KEY_RIGHT_CONTROL   ) || IsKeyDown(KEY_LEFT_CONTROL ));
    io.AddKeyEvent(ImGuiKey_ModShift, IsKeyDown(KEY_RIGHT_SHIFT     ) || IsKeyDown(KEY_LEFT_SHIFT   ));
    io.AddKeyEvent(ImGuiKey_ModAlt,   IsKeyDown(KEY_RIGHT_ALT       ) || IsKeyDown(KEY_LEFT_ALT     ));
    io.AddKeyEvent(ImGuiKey_ModSuper, IsKeyDown(KEY_RIGHT_SUPER     ) || IsKeyDown(KEY_LEFT_SUPER   ));

#define CASE(x_, y_) \
    if (IsKeyPressed(y_)) { io.AddKeyEvent(x_, true); } \
    if (IsKeyReleased(y_)) { io.AddKeyEvent(x_, false); }

ENUM_KEY_MAP
#undef CASE

    unsigned int pressed = GetCharPressed();
    if (pressed != 0)
    {
        io.AddInputCharacter(pressed);
    }
}

static void rlImGuiTriangleVert(ImDrawVert& idx_vert)
{
    Color* c;
    c = (Color*)&idx_vert.col;
    rlColor4ub(c->r, c->g, c->b, c->a);
    rlTexCoord2f(idx_vert.uv.x, idx_vert.uv.y);
    rlVertex2f(idx_vert.pos.x, idx_vert.pos.y);
}

static void rlImGuiRenderTriangles(unsigned int count, int indexStart, const ImVector<ImDrawIdx>& indexBuffer, const ImVector<ImDrawVert>& vertBuffer, void* texturePtr)
{
    if (count < 3)
        return;
	
    Texture* texture = (Texture*)texturePtr;

    unsigned int textureId = (texture == nullptr) ? 0 : texture->id;

    rlBegin(RL_TRIANGLES);
    rlSetTexture(textureId);

    for (unsigned int i = 0; i <= (count - 3); i += 3)
    {
        if(rlCheckRenderBatchLimit(3)) 
        {
            rlBegin(RL_TRIANGLES);
            rlSetTexture(textureId);
        }

        ImDrawIdx indexA = indexBuffer[indexStart + i];
        ImDrawIdx indexB = indexBuffer[indexStart + i + 1];
        ImDrawIdx indexC = indexBuffer[indexStart + i + 2];

        ImDrawVert vertexA = vertBuffer[indexA];
        ImDrawVert vertexB = vertBuffer[indexB];
        ImDrawVert vertexC = vertBuffer[indexC];

        rlImGuiTriangleVert(vertexA);
        rlImGuiTriangleVert(vertexB);
        rlImGuiTriangleVert(vertexC);
    }
    rlEnd();
}

static void EnableScissor(float x, float y, float width, float height)
{
    rlEnableScissorTest();
    rlScissor((int)x, GetScreenHeight() - (int)(y + height), (int)width, (int)height);
}

static void rlRenderData(ImDrawData* data)
{
    rlDrawRenderBatchActive();
    rlDisableBackfaceCulling();

    for (int l = 0; l < data->CmdListsCount; ++l)
    {
        const ImDrawList* commandList = data->CmdLists[l];

        for (const auto& cmd : commandList->CmdBuffer)
        {
            EnableScissor(cmd.ClipRect.x - data->DisplayPos.x, cmd.ClipRect.y - data->DisplayPos.y, cmd.ClipRect.z - (cmd.ClipRect.x - data->DisplayPos.x), cmd.ClipRect.w - (cmd.ClipRect.y - data->DisplayPos.y));
            if (cmd.UserCallback != nullptr)
            {
                cmd.UserCallback(commandList, &cmd);
  
                continue;
            }

            rlImGuiRenderTriangles(cmd.ElemCount, cmd.IdxOffset, commandList->IdxBuffer, commandList->VtxBuffer, cmd.TextureId);
            rlDrawRenderBatchActive();
        }
    }

    rlSetTexture(0);
    rlDisableScissorTest();
    rlEnableBackfaceCulling();
}

void rlImGuiBeginInitImGui()
{
    ImGui::CreateContext(nullptr);
}

void rlImGuiEndInitImGui()
{
    ImGuiIO& io = ImGui::GetIO();
    io.BackendPlatformName      = "imgui_impl_raylib";
    io.BackendFlags             |= ImGuiBackendFlags_HasMouseCursors;
    io.MousePos                 = ImVec2(0, 0);
    io.SetClipboardTextFn       = rlImGuiSetClipText;
    io.GetClipboardTextFn       = rlImGuiGetClipText;
    io.ClipboardUserData        = nullptr;


    rlImGuiReloadFonts();
}

void rlImGuiSetup(bool dark)
{
    rlImGuiBeginInitImGui();

    if (dark)
        ImGui::StyleColorsDark();
    else
        ImGui::StyleColorsLight();

    rlImGuiEndInitImGui();
}

void rlImGuiReloadFonts()
{
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels = nullptr;

    int width;
    int height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height, nullptr);
    Image image = GenImageColor(width, height, BLANK);
    memcpy(image.data, pixels, width * height * 4);

    if (GLOBALS.FontTexture.id != 0)
        UnloadTexture(GLOBALS.FontTexture);

    GLOBALS.FontTexture = LoadTextureFromImage(image);
    UnloadImage(image);
    io.Fonts->TexID = &GLOBALS.FontTexture;
}

void rlImGuiBegin()
{
    rlImGuiNewFrame();
    rlImGuiEvents();
    ImGui::NewFrame();
}

void rlImGuiEnd()
{
    ImGui::Render();
    rlRenderData(ImGui::GetDrawData());
}

void rlImGuiShutdown()
{
    for (const auto& tx : GLOBALS.LoadedTextures)
        UnloadTexture(tx);

    UnloadTexture(GLOBALS.FontTexture);
    GLOBALS.LoadedTextures.clear();
}

void rlImGuiImage(const Texture *image)
{
    ImGui::Image((ImTextureID)image, ImVec2(float(image->width), float(image->height)));
}

void rlImGuiImageSize(const Texture *image, int width, int height)
{
    ImGui::Image((ImTextureID)image, ImVec2(float(width), float(height)));
}

void rlImGuiImageRect(const Texture* image, int destWidth, int destHeight, Rectangle sourceRect)
{
    ImVec2 uv0;
    ImVec2 uv1;

    if (sourceRect.width < 0)
    {
        uv0.x = -((float)sourceRect.x / image->width);
        uv1.x = (uv0.x - (float)(fabs(sourceRect.width) / image->width));
    }
    else
    {
        uv0.x = (float)sourceRect.x / image->width;
        uv1.x = uv0.x + (float)(sourceRect.width / image->width);
    }

    if (sourceRect.height < 0)
    {
        uv0.y = -((float)sourceRect.y / image->height);
        uv1.y = (uv0.y - (float)(fabs(sourceRect.height) / image->height));
    }
    else
    {
        uv0.y = (float)sourceRect.y / image->height;
        uv1.y = uv0.y + (float)(sourceRect.height / image->height);
    }

    ImGui::Image((ImTextureID)image, ImVec2(float(destWidth), float(destHeight)),uv0,uv1);
}
