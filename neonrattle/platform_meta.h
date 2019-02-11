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

#include <Arduino.h>

#ifdef __cplusplus
#include <Gamebuino-Meta.h>
#endif

Z_EXTERN_C_START

typedef uint16_t ZPixel;

static inline ZRgb z_pixel_toRgb(ZPixel Pixel)
{
    return (ZRgb){((Pixel >> 11)                 ) << 3,
                  ((Pixel >> 5)  & ((1 << 6) - 1)) << 2,
                  ((Pixel >> 0)  & ((1 << 5) - 1)) << 3};
}

static inline int z_pixel_toAnyChannel(ZPixel Pixel)
{
    return (Pixel << 3) & 0xff;
}

static inline ZPixel z_pixel_fromRgb(int Red, int Green, int Blue)
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

#define Z_SCREEN_W 80
#define Z_SCREEN_H 64

#define z_sprite_load(Index, Id)                             \
    z_platform__loadSprite(Index, z_data_gfx_##Id##_buffer);

#define z_sfx_load(Index, Id)                     \
    z_platform__loadSfx(Index,                    \
                        z_data_sfx_##Id##_buffer, \
                        z_data_sfx_##Id##_size);

extern void z_platform__loadSprite(int Sprite, const uint16_t* Buffer);
extern void z_platform__loadSfx(int Sfx, const uint8_t* Buffer, uint32_t Size);

extern void z_platform_meta_lightsFill(int BgColorId, int ColorId, int Alpha);
extern void z_platform_meta_lightsDraw(int ColorId, int Alpha, int X, int Y);

Z_EXTERN_C_END
