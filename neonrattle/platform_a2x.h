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

#include <a2x.h>

typedef APixel ZPixel;

#include "util_graphics.h"
#include "util_sound.h"

#define Z_SCREEN_W 80
#define Z_SCREEN_H 64

#define z_sprite_load(Index, Id) \
    z_platform__loadSprite(Index, "assets/gfx/" #Id ".png");

#define z_sfx_load(Index, Id) \
    z_platform__loadSfx(Index, "assets/sfx/" #Id ".wav");

extern void z_platform__loadSprite(ZSpriteId Sprite, const char* Path);
extern void z_platform__loadSfx(ZSfxId Sfx, const char* Path);

static inline void z_pixel_toRGB(ZPixel Pixel, int* Red, int* Green, int* Blue)
{
    a_pixel_toRgb(Pixel, Red, Green, Blue);
}

static inline int z_pixel_toR(ZPixel Pixel)
{
    int red;
    a_pixel_toRgb(Pixel, &red, NULL, NULL);

    return red;
}

static inline ZPixel z_pixel_fromRGB(int Red, int Green, int Blue)
{
    return a_pixel_fromRgb(Red, Green, Blue);
}

static inline ZPixel z_pixel_fromHex(uint32_t Hexcode)
{
    return a_pixel_fromHex(Hexcode);
}
