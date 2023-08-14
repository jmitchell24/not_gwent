#include "random.hpp"

using namespace ut;

#include <random>
using namespace std;

RandomManager::RandomManager()
{
    size_t n_colors = 32;

    float hue = 0, hue_inc = 360.0f / n_colors;

    m_colors.reserve(n_colors);
    for (size_t i = 0; i < n_colors; ++i)
    {
        m_colors.push_back(color(color::hsluv{hue, 80.0f, 80.0f, 1.0f}));
        hue += hue_inc;
    }

    //srand(time(0));
    //random_shuffle(m_colors.begin(), m_colors.end());

    random_device rd;
    mt19937 g(rd());

    shuffle(m_colors.begin(), m_colors.end(), g);
}

RandomManager& RandomManager::instance()
{
    static RandomManager rm;
    return rm;
}

RandomManager::color RandomManager::nextColor()
{
    return m_colors[ m_color_counter++ % m_colors.size() ];
}

RandomManager::color RandomManager::nextColor(size_t seed)
{
    return m_colors[ seed % m_colors.size() ];
}
