/*
    Copyright 2018, 2019 Alex Margarit <alex@alxm.org>

    Neonrattle is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Neonrattle is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Neonrattle.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <a2x.h>

#if Z_DEBUG_STATS && A_BUILD_SYSTEM_DESKTOP
    #define Z_DEBUG_INSTRUMENT 1
#endif

typedef APixel ZPixel;

static inline ZRgb z_pixel_toRgb(ZPixel Pixel)
{
    ZRgb rgb;
    a_pixel_toRgb(Pixel, &rgb.r, &rgb.g, &rgb.b);

    return rgb;
}

static inline int z_pixel_toAnyChannel(ZPixel Pixel)
{
    int blue;
    a_pixel_toRgb(Pixel, NULL, NULL, &blue);

    return blue;
}

static inline ZPixel z_pixel_fromRgb(int Red, int Green, int Blue)
{
    return a_pixel_fromRgb(Red, Green, Blue);
}

static inline ZPixel z_pixel_fromHex(uint32_t Hexcode)
{
    return a_pixel_fromHex(Hexcode);
}

#define Z_SCREEN_W 80
#define Z_SCREEN_H 64

#define z_sprite_load(Index, Id) \
    z_platform__loadSprite(Index, "assets/gfx/" #Id ".png");

#define z_sprite_loadGrid(Index, Id, W, H) \
    z_platform__loadSpriteGrid(Index, "assets/gfx/" #Id ".png", W, H);

#define z_sfx_load(Index, Id) \
    z_platform__loadSfx(Index, "assets/sfx/" #Id ".wav");

extern void z_platform__loadSprite(int Sprite, const char* Path);
extern void z_platform__loadSpriteGrid(int Sprite, const char* Path, int W, int H);
extern void z_platform__loadSfx(int Sfx, const char* Path);
