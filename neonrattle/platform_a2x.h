/*
    Copyright 2018-2019 Alex Margarit <alex@alxm.org>
    This file is part of Neonrattle, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <faur.h>

#if Z_DEBUG && F_BUILD_SYSTEM_DESKTOP
    #define Z_DEBUG_INSTRUMENT 1
#endif

typedef FPixel ZPixel;

static inline ZRgb z_pixel_toRgb(ZPixel Pixel)
{
    FRgb rgb = f_pixel_toRgb(Pixel);

    return (ZRgb){rgb.r, rgb.g, rgb.b};
}

static inline int z_pixel_toAnyChannel(ZPixel Pixel)
{
    return f_pixel_toRgb(Pixel).r;
}

static inline ZPixel z_pixel_fromRgb(int Red, int Green, int Blue)
{
    return f_pixel_fromRgb(Red, Green, Blue);
}

static inline ZPixel z_pixel_fromHex(uint32_t Hexcode)
{
    return f_pixel_fromHex(Hexcode);
}

#define z_sprite_load(Index, Id) \
    z_platform__loadSprite(Index, "assets/gfx/" #Id ".png");

#define z_sfx_load(Index, Id) \
    z_platform__loadSfx(Index, "assets/sfx/" #Id ".wav");

extern void z_platform__loadSprite(int Sprite, const char* Path);
extern void z_platform__loadSfx(int Sfx, const char* Path);
