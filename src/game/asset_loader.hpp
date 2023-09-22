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

namespace game::asset_loaders
{
    struct Texture2DLoader
    {
        using key_type = std::string;
        using value_type = Texture2D;
        static value_type load(key_type const& k);
        static bool isReady(value_type const& v);
        static value_type errorValue();
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

}

namespace std
{
    template<> struct hash<game::asset_loaders::FontKey>
    {
        size_t operator()(game::asset_loaders::FontKey const& key) const
        {
            ut::digest d;
            d.putRange(key.filename);
            d.putValue(key.font_size);
            return d.value();
        }
    };
}