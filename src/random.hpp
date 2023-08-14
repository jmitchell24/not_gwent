#pragma once

#include <ut/color.hpp>

#include <vector>

class RandomManager
{
public:
    using color = ut::color;

    RandomManager();
    static RandomManager& instance();

    color nextColor();
    color nextColor(size_t seed);

private:
    size_t              m_color_counter=0;
    std::vector<color>  m_colors;
};

[[maybe_unused]] static RandomManager& RANDOM = RandomManager::instance();
