#pragma once

#include "../../Utilities/RandomNumberGenerator.h"
static constexpr uint8_t kMinColorValue = 55;

inline RandomNumberGenerator g_rng; //TODO: Fix redo this hackyness 

namespace Brokkr
{
    class Color
    {
    public:

        int m_red = 0;
        int m_green = 0;
        int m_blue = 0;
        int m_opacity = 0;

        //TODO: methods for randomizing colors
        void RandomColors()
        {
            m_red = static_cast<int>(kMinColorValue + g_rng.RandomRange(0, 255 - kMinColorValue));
            m_green = static_cast<int>(kMinColorValue + g_rng.RandomRange(0, 255 - kMinColorValue));
            m_blue = static_cast<int>(kMinColorValue + g_rng.RandomRange(0, 255 - kMinColorValue));
            m_opacity = static_cast<int>(g_rng.RandomRange(0, 255));
        }

    };

}
