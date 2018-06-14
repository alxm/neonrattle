/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Neon Rattle is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Neon Rattle is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Neon Rattle.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "platform.h"
#include "util_light.h"

#include "util_fix.h"
#include "util_fps.h"

static const struct {
    ZColorId color;
    ZFixu counterSpeed[2];
} g_patterns[Z_LIGHT_NUM] = {
    [Z_LIGHT_APPLE_EAT] = {
        Z_COLOR_BG_GREEN_03,
        {Z_FIX_DEG_090 / 4, Z_FIX_DEG_090 / 2}
    },
};

static struct {
    ZColorId bgColor;
    ZLightId pulseId;
    ZFixu counter;
} g_light;

static struct {
    ZColorId bgColor;
    ZColorId pulseColor;
    int alpha;
} g_last;

void z_light_reset(void)
{
    g_light.bgColor = Z_COLOR_INVALID;
    g_light.pulseId = Z_LIGHT_INVALID;
    g_light.counter = 0;

    g_last.bgColor = Z_COLOR_INVALID;
    g_last.pulseColor = Z_COLOR_INVALID;
    g_last.alpha = 0;
}

void z_light_tick(void)
{
    ZColorId color = Z_COLOR_INVALID;
    int alpha = 0;

    if(g_light.pulseId != Z_LIGHT_INVALID) {
        bool goingDown = g_light.counter >= Z_FIX_DEG_090;
        g_light.counter += g_patterns[g_light.pulseId].counterSpeed[goingDown];

        if(g_light.counter >= Z_FIX_DEG_180) {
            g_light.pulseId = Z_LIGHT_INVALID;
        } else {
            color = g_patterns[g_light.pulseId].color;
            alpha = z_fix_toInt(z_fix_sinf(g_light.counter) * 256);
        }
    }

    #if Z_PLATFORM_META
        if(g_light.bgColor != g_last.bgColor
            || color != g_last.pulseColor || alpha != g_last.alpha) {

            g_last.bgColor = g_light.bgColor;
            g_last.pulseColor = color;
            g_last.alpha = alpha;

            z_platform_meta_fillLights(g_light.bgColor, color, alpha);
        }
    #else
        Z_UNUSED(color);
        Z_UNUSED(alpha);
    #endif
}

void z_light_setPulse(ZLightId Light)
{
    g_light.pulseId = Light;
    g_light.counter = 0;
}

void z_light_setBackground(ZColorId ColorId)
{
    g_light.bgColor = ColorId;
}
