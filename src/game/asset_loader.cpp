//
// Created by james on 8/29/23.
//

#include "game/asset_loader.hpp"
using namespace game;
using namespace game::asset_loaders;

//
// Texture2DLoader -> Implementation
//

Texture2DLoader::value_type Texture2DLoader::load(key_type const& k)
{
    return LoadTexture(k.c_str());
}

bool Texture2DLoader::isReady(value_type const& v)
{
    return IsTextureReady(v);
}

Texture2DLoader::value_type Texture2DLoader::errorValue()
{
    static Texture2D error_texture;

    if (!IsTextureReady(error_texture))
    {
        int w=32, h=32;

        Image img = GenImageColor(w,h,WHITE);

        ImageDrawLine(&img, 0,0, w,h, RED);
        ImageDrawLine(&img, w,0, 0,h, RED);

        ImageDrawLine(&img, 0,0, w,0, RED);
        ImageDrawLine(&img, 0,0, 0,h, RED);

        ImageDrawLine(&img, w,h, w,0, RED);
        ImageDrawLine(&img, w,h, 0,h, RED);

        error_texture = LoadTextureFromImage(img);
        UnloadImage(img);
    }

    return error_texture;
}

//
// FontLoader -> Implementation
//

FontLoader::value_type FontLoader::load(key_type const& k)
{
    return LoadFontEx(k.filename.c_str(), (int)k.font_size, nullptr, 0);
}

bool FontLoader::isReady(value_type const& v)
{
    return IsFontReady(v);
}

FontLoader::value_type FontLoader::errorValue()
{
    return { };
}