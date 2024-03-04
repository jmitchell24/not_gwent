//
// Created by james on 8/29/23.
//

#pragma once

#include "raylib.h"
#include "record.hpp"

//
// ut
//
#include <ut/string.hpp>
#include <ut/algo/hash.hpp>

//
// std
//
#include <string>

namespace res::loaders
{
    struct Texture2DLoader
    {
        using key_type = std::string;
        using value_type = Texture2D;
        static value_type load(key_type const& k);
        static bool isReady(value_type const& v);
        static value_type errorValue();
    };

    struct CardTextureLoader
    {
        using key_type = std::string;
        using value_type = Texture2D;
        static value_type load(key_type const& k);
        static bool isReady(value_type const& v);
        static value_type errorValue();

    private:
        static Image paperImage();
        static Image paperMaskImage();
    };

    struct FontKey
    {
        std::string filename; size_t font_size;
        RECORD_DECLS(FontKey, filename, font_size);
    };

    struct FontLoader
    {
        using key_type = FontKey;
        using value_type = Font;
        static value_type load(key_type const& k);
        static bool isReady(value_type const& v);
        static value_type errorValue();
    };

    struct SvgKey
    {
        std::string filename; size_t w,h;
        RECORD_DECLS(SvgKey, filename, w, h);
    };

    struct SvgLoader
    {
        using key_type = SvgKey;
        using value_type = Texture2D;
        static value_type load(key_type const& k);
        static bool isReady(value_type const& v);
        static value_type errorValue();

    };

}

namespace std
{
    template<> struct hash<res::loaders::FontKey>
    {
        size_t operator()(res::loaders::FontKey const& key) const
        {
            ut::digest d;
            d.putRange(key.filename);
            d.putValue(key.font_size);
            return d.value();
        }
    };

    template<> struct hash<res::loaders::SvgKey>
    {
        size_t operator()(res::loaders::SvgKey const& key) const
        {
            ut::digest d;
            d.putRange(key.filename);
            d.putValue(key.w);
            d.putValue(key.h);
            return d.value();
        }
    };
}