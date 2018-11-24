/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

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

#pragma once

Z_EXTERN_C_START

#include "util_fix.h"
#include "util_screen.h"

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

    Z_COLOR_RED_100,
    Z_COLOR_RED_050,
    Z_COLOR_RED_025,
    Z_COLOR_GREEN_100,
    Z_COLOR_WHITE_100,

    Z_COLOR_APPLE_01,
    Z_COLOR_APPLE_02,
    Z_COLOR_APPLE_03,
    Z_COLOR_APPLE_04,
    Z_COLOR_APPLE_05,

    Z_COLOR_BG_GREEN_01,
    Z_COLOR_BG_GREEN_02,
    Z_COLOR_BG_GREEN_03,
    Z_COLOR_BG_GREEN_04,
    Z_COLOR_BG_GREEN_05,

    Z_COLOR_SNAKE_01,
    Z_COLOR_SNAKE_02,
    Z_COLOR_SNAKE_03,

    Z_COLOR_NUM
} ZColorId;

#define Z_COLOR_APPLE_NUM 5
#define Z_COLOR_SNAKE_NUM 3

static inline ZColorId z_color_getRandomApple(void)
{
    return (ZColorId)(Z_COLOR_APPLE_01 + z_random_int(Z_COLOR_APPLE_NUM));
}

static inline ZColorId z_color_getRandomSnake(void)
{
    return (ZColorId)(Z_COLOR_SNAKE_01 + z_random_int(Z_COLOR_SNAKE_NUM));
}

typedef enum {
    Z_SPRITE_PALETTE,

    // Maps
    Z_SPRITE_MAP0,

    // Tiles
    Z_SPRITE_TILES,

    // Snake
    Z_SPRITE_SNAKE_ALPHAMASK,

    // Apples
    Z_SPRITE_APPLE_ALPHAMASK,
    Z_SPRITE_APPLE_HALO,

    // Misc
    Z_SPRITE_ALXM,
    Z_SPRITE_ALXM_WING,

    Z_SPRITE_NUM
} ZSpriteId;

typedef struct ZColor {
    ZPixel pixel;
    int r, g, b;
} ZColor;

extern ZColor z_colors[Z_COLOR_NUM];

extern void z_graphics_setup(void);

extern ZPixel* z_screen_getPixels(void);

extern ZPixel z_sprite_getTransparentColor(void);
extern const ZPixel* z_sprite_getPixels(ZSpriteId Sprite, unsigned Frame);
extern ZPixel z_sprite_getPixel(ZSpriteId Sprite, unsigned Frame, int X, int Y);
extern void z_sprite_blit(ZSpriteId Sprite, int X, int Y, unsigned Frame);
extern void z_sprite_blitCentered(ZSpriteId Sprite, int X, int Y, unsigned Frame);
extern void z_sprite_blitAlphaMask(ZSpriteId AlphaMask, int X, int Y, unsigned Frame, ZColorId Fill, int Alpha);
extern void z_sprite_blitAlphaMaskRGBA(ZSpriteId AlphaMask, int X, int Y, unsigned Frame, int R, int G, int B, int Alpha);
extern int z_sprite_getWidth(ZSpriteId Sprite);
extern int z_sprite_getHeight(ZSpriteId Sprite);
extern uint8_t z_sprite_getNumFrames(ZSpriteId Sprite);

extern void z_draw_fill(ZColorId ColorId);
extern void z_draw_rectangle(int X, int Y, int W, int H, ZColorId ColorId);
extern void z_draw_rectangleAlpha(int X, int Y, int W, int H, ZColorId ColorId, int Alpha);
extern void z_draw_pixel(int X, int Y, ZColorId ColorId);
extern void z_draw_hline(int X1, int X2, int Y, ZColorId ColorId);
extern void z_draw_vline(int X, int Y1, int Y2, ZColorId ColorId);
extern void z_draw_circle(int X, int Y, int Radius, ZColorId ColorId);

static inline void z_pixel_drawAlpha(ZPixel* Dst, int Red, int Green, int Blue, int Alpha)
{
    int r, g, b;
    z_pixel_toRGB(*Dst, &r, &g, &b);

    *Dst = z_pixel_fromRGB(r + (((Red   - r) * Alpha) >> 8),
                           g + (((Green - g) * Alpha) >> 8),
                           b + (((Blue  - b) * Alpha) >> 8));
}

static inline void z_pixel_drawAlpha2(int X, int Y, ZColorId ColorId, int Alpha)
{
    z_pixel_drawAlpha(z_screen_getPixels() + Y * Z_SCREEN_W + X,
                      z_colors[ColorId].r,
                      z_colors[ColorId].g,
                      z_colors[ColorId].b,
                      Alpha);
}

Z_EXTERN_C_END
