//
// Created by james on 8/29/23.
//

#include "res/res_loader.hpp"
using namespace res;
using namespace res::loaders;

//
// ut
//
#include <ut/check.hpp>
using namespace ut;


//
// std
//
#include <optional>
using namespace std;

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
// CardTextureLoader -> Implementation
//

CardTextureLoader::value_type CardTextureLoader::load(key_type const& k)
{
    if (Image img = LoadImage(k.c_str()); IsImageReady(img))
    {
        ImageAlphaMask(&img, paperMaskImage());

        Texture2D tex = LoadTextureFromImage(img);
        UnloadImage(img);
        return tex;
    }

    return { 0 };
}

bool CardTextureLoader::isReady(value_type const& v)
{
    return IsTextureReady(v);
}

CardTextureLoader::value_type CardTextureLoader::errorValue()
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

Image CardTextureLoader::paperImage()
{
    static optional<Image> img;

    if (!img)
        img = LoadImage("data/board/card_blank_sm.png");
    return *img;
}

Image CardTextureLoader::paperMaskImage()
{
    static optional<Image> img;


    if (!img)
    {
        Image tmp = LoadImage("data/board/card_blank_mask.png");
        ImageColorGrayscale(&tmp);
        img = tmp;

    }
    return *img;
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

#define NANOSVGRAST_IMPLEMENTATION
#include "rlNanoVG/nanosvgrast.h"

//
// SvgLoader -> Implementation
//

SvgLoader::value_type SvgLoader::load(key_type const& k)
{


    // Load SVG
    if (auto svg = nsvgParseFromFile(k.filename.c_str(), "px", 96))
    {
        if (auto rast = nsvgCreateRasterizer())
        {
            int w = (int)k.w;
            int h = (int)k.h;

            if (Image img = GenImageColor(w,h, BLANK); IsImageReady(img))
            {

                if (img.format == PIXELFORMAT_UNCOMPRESSED_R8G8B8A8)
                {
                    // Rasterize
                    auto dst = (unsigned char*)img.data;
                    nsvgRasterize(rast, svg, 0,0,1, dst, w, h, w*4);
                }


                Texture2D tex = LoadTextureFromImage(img);
                UnloadImage(img);
                return tex;
            }
            nsvgDeleteRasterizer(rast);
        }
        nsvgDelete(svg);
    }

    return { 0 };
}

bool SvgLoader::isReady(value_type const& v)
{
    return IsTextureReady(v);
}

SvgLoader::value_type SvgLoader::errorValue()
{
    return { };
}





















