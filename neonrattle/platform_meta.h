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

#include <Arduino.h>

#ifdef __cplusplus
#include <Gamebuino-Meta.h>
#endif

Z_EXTERN_C_START

typedef uint16_t ZPixel;

#include "util_graphics.h"
#include "util_sound.h"

#define Z_SCREEN_W 80
#define Z_SCREEN_H 64

#define z_sprite_load(Index, Id)                             \
    z_platform__loadSprite(Index, z_data_gfx_##Id##_buffer);

#define z_sfx_load(Index, Id)                     \
    z_platform__loadSfx(Index,                    \
                        z_data_sfx_##Id##_buffer, \
                        z_data_sfx_##Id##_size);

extern void z_platform__loadSprite(ZSpriteId Sprite, const uint16_t* Buffer);
extern void z_platform__loadSfx(ZSfxId Sfx, const uint8_t* Buffer, uint32_t Size);

extern void z_platform_meta_fillLights(ZColorId BgColorId, ZColorId ColorId, int Alpha);
extern void z_platform_meta_drawLights(ZColorId ColorId, int Alpha, int X, int Y);

static inline void z_pixel_toRGB(ZPixel Pixel, int* R, int* G, int* B)
{
    *R = ((Pixel >> 11) & ((1 << 5) - 1)) << 3;
    *G = ((Pixel >> 5)  & ((1 << 6) - 1)) << 2;
    *B = ((Pixel >> 0)  & ((1 << 5) - 1)) << 3;
}

static inline ZPixel z_pixel_fromRGB(int Red, int Green, int Blue)
{
    return (ZPixel)
        (((Red >> 3) << 11) | ((Green >> 2) << 5)| ((Blue >> 3) << 0));
}

static inline ZPixel z_pixel_fromHex(uint32_t Hexcode)
{
    return (ZPixel)
        (((((Hexcode >> 16)       ) >> 3) << 11)  |
         ((((Hexcode >> 8)  & 0xff) >> 2) << 5)   |
         ((((Hexcode)       & 0xff) >> 3) << 0));
}

Z_EXTERN_C_END
