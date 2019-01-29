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

#pragma once

#include "platform.h"

Z_EXTERN_C_START

#include "util_fix.h"

typedef enum {
    Z_COLOR_INVALID = -1,

    Z_COLOR_ALXM_BG,
    Z_COLOR_ALXM_Y1,
    Z_COLOR_ALXM_Y2,
    Z_COLOR_ALXM_G1,
    Z_COLOR_ALXM_G2,
    Z_COLOR_ALXM_B1,
    Z_COLOR_ALXM_B2,
    Z_COLOR_ALXM_R1,
    Z_COLOR_ALXM_R2,

    Z_COLOR_MAP_WHITE,
    Z_COLOR_MAP_GREEN,
    Z_COLOR_MAP_RED,

    Z_COLOR_APPLE_01,
    Z_COLOR_APPLE_02,
    Z_COLOR_APPLE_03,
    Z_COLOR_APPLE_04,
    Z_COLOR_APPLE_05,
    Z_COLOR_APPLE_06_04,
    Z_COLOR_APPLE_07_03,
    Z_COLOR_APPLE_08_02,

    Z_COLOR_BG_GREEN_01,
    Z_COLOR_BG_GREEN_02,
    Z_COLOR_BG_GREEN_03,
    Z_COLOR_BG_GREEN_04,
    Z_COLOR_BG_GREEN_05,

    Z_COLOR_SNAKE_01,
    Z_COLOR_SNAKE_02,
    Z_COLOR_SNAKE_03,
    Z_COLOR_SNAKE_04_02,

    Z_COLOR_NUM
} ZColorId;

#define Z_COLOR_APPLE_NUM 8
#define Z_COLOR_SNAKE_NUM 4

static inline ZColorId z_color_appleGet(void)
{
    return (ZColorId)(Z_COLOR_APPLE_01 + z_random_int(Z_COLOR_APPLE_NUM));
}

static inline ZColorId z_color_snakeGet(void)
{
    return (ZColorId)(Z_COLOR_SNAKE_01 + z_random_int(Z_COLOR_SNAKE_NUM));
}

typedef enum {
    Z_SPRITE_INVALID = -1,
    Z_SPRITE_PALETTE,

    Z_SPRITE_ALXM,
    Z_SPRITE_ALXM2,
    Z_SPRITE_ALXM_WING,
    Z_SPRITE_NEONRATTLE,
    Z_SPRITE_NEONRATTLE_GLOW,

    Z_SPRITE_MAPS,
    Z_SPRITE_TILES,

    Z_SPRITE_SNAKE_MASK,
    Z_SPRITE_SNAKE_MINIMAP,

    Z_SPRITE_APPLE_MASK,
    Z_SPRITE_APPLE_HALO,

    Z_SPRITE_ICON_APPLE,
    Z_SPRITE_ICON_CHECK,
    Z_SPRITE_ICON_HEART,
    Z_SPRITE_ICON_LOCK,
    Z_SPRITE_ICON_LVL,

    Z_SPRITE_FONT_LCDNUM,

    Z_SPRITE_NUM
} ZSpriteId;

typedef struct ZColor {
    ZPixel pixel;
    ZRgb rgb;
} ZColor;

extern ZColor z_colors[Z_COLOR_NUM];

extern void z_graphics_setup(void);

extern ZPixel* z_screen_pixelsGet(void);

extern ZPixel z_sprite_transparentColorGet(void);
extern const ZPixel* z_sprite_pixelsGet(ZSpriteId Sprite, unsigned Frame);
extern ZPixel z_sprite_pixelGet(ZSpriteId Sprite, unsigned Frame, int X, int Y);
extern void z_sprite_blit(ZSpriteId Sprite, int X, int Y, unsigned Frame);
extern void z_sprite_blitCentered(ZSpriteId Sprite, int X, int Y, unsigned Frame);
extern void z_sprite_blitAlphaMask(ZSpriteId AlphaMask, int X, int Y, unsigned Frame, ZColorId Fill, int Alpha);
extern void z_sprite_blitAlphaMaskRGBA(ZSpriteId AlphaMask, int X, int Y, unsigned Frame, const ZRgb* Rgb, int Alpha);
extern int z_sprite_widthGet(ZSpriteId Sprite);
extern int z_sprite_heightGet(ZSpriteId Sprite);
extern uint8_t z_sprite_framesNumGet(ZSpriteId Sprite);

extern void z_draw_fill(ZColorId Color);
extern void z_draw_rectangle(int X, int Y, int W, int H, ZColorId Color);
extern void z_draw_rectangleAlpha(int X, int Y, int W, int H, ZColorId Color, int Alpha);
extern void z_draw_pixel(int X, int Y, ZColorId Color);
extern void z_draw_hline(int X1, int X2, int Y, ZColorId Color);
extern void z_draw_vline(int X, int Y1, int Y2, ZColorId Color);
extern void z_draw_circle(int X, int Y, int Radius, ZColorId Color);

static inline void z_draw_pixelBufferRgbAlpha(ZPixel* Dst, const ZRgb* Rgb, int Alpha)
{
    ZRgb rgb = z_pixel_toRgb(*Dst);

    *Dst = z_pixel_fromRgb(rgb.r + (((Rgb->r - rgb.r) * Alpha) >> 8),
                           rgb.g + (((Rgb->g - rgb.g) * Alpha) >> 8),
                           rgb.b + (((Rgb->b - rgb.b) * Alpha) >> 8));
}

static inline void z_draw_pixelCoordsIdAlpha(int X, int Y, ZColorId Color, int Alpha)
{
    z_draw_pixelBufferRgbAlpha(
        z_screen_pixelsGet() + Y * Z_SCREEN_W + X, &z_colors[Color].rgb, Alpha);
}

static inline void z_draw_pixelCoordsRgbAlpha(int X, int Y, const ZRgb* Rgb, int Alpha)
{
    z_draw_pixelBufferRgbAlpha(
        z_screen_pixelsGet() + Y * Z_SCREEN_W + X, Rgb, Alpha);
}

Z_EXTERN_C_END
