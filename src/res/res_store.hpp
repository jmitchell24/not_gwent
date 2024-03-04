//
// Created by james on 8/28/23.
//

#pragma once

#include "res/res_loader.hpp"

//
// std
//
#include <cstddef>
#include <optional>
#include <unordered_map>

namespace res
{
    template <typename Loader>
    class AssetStoreX
    {
    public:
        using store_type    = AssetStoreX<Loader>;
        using loader_type   = Loader;
        using key_type      = typename Loader::key_type;
        using value_type    = typename Loader::value_type;
        using map_type      = std::unordered_map<key_type, value_type>;

        static AssetStoreX<Loader>& instance()
        {
            static store_type x;
            return x;
        }

        bool tryGet(key_type const& k, value_type& v)
        {
            if (auto it = m_assets.find(k); it != m_assets.end())
            {
                v = it->second;
                return true;
            }

            if (auto x = load(k); isReady(x))
            {
                v = (m_assets[k] = x);
                return true;
            }

            return false;
        }

        value_type get(key_type const& k)
        {
            if (auto it = m_assets.find(k); it != m_assets.end())
            {
                TraceLog(LOG_INFO, "ASSET STORE GET CACHE");
                return it->second;
            }

            if (auto x = load(k); isReady(x))
            {
                TraceLog(LOG_INFO, "ASSET STORE GET FRESH");
                return (m_assets[k] = x);
            }

            return errorValue();
        }

    private:
        map_type m_assets;

        static bool isReady(value_type const& v) { return static_cast<bool>(loader_type::isReady(v)); }
        static value_type load(key_type const& k) { return static_cast<value_type>(loader_type::load(k)); }
        static value_type errorValue() { return static_cast<value_type>(loader_type::errorValue()); }
    };



    [[maybe_unused]] static AssetStoreX<loaders::Texture2DLoader>& TEXTURES =
            AssetStoreX<loaders::Texture2DLoader>::instance();

    [[maybe_unused]] static AssetStoreX<loaders::CardTextureLoader>& CARD_TEXTURES =
            AssetStoreX<loaders::CardTextureLoader>::instance();

    [[maybe_unused]] static AssetStoreX<loaders::FontLoader>& FONTS =
            AssetStoreX<loaders::FontLoader>::instance();

    [[maybe_unused]] static AssetStoreX<loaders::SvgLoader>& SVG =
            AssetStoreX<loaders::SvgLoader>::instance();
}