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

#pragma once

Z_EXTERN_C_START

typedef enum Z_ENUM_PACK {
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

    Z_COLOR_BG_GREEN_01,
    Z_COLOR_BG_GREEN_02,
    Z_COLOR_BG_GREEN_03,
    Z_COLOR_BG_GREEN_04,
    Z_COLOR_BG_GREEN_05,

    Z_COLOR_SNAKE_BLUE_01,

    Z_COLOR_NUM
} ZColorId;

typedef enum Z_ENUM_PACK {
    Z_SPRITE_PALETTE,

    // Maps
    Z_SPRITE_MAP0,

    // Tiles
    Z_SPRITE_TILES,

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
extern ZPixel* z_sprite_getPixels(ZSpriteId Sprite, unsigned Frame);
extern ZPixel z_sprite_getPixel(ZSpriteId Sprite, unsigned Frame, int X, int Y);
extern void z_sprite_blit(ZSpriteId Sprite, int X, int Y, unsigned Frame);
extern void z_sprite_blitCentered(ZSpriteId Sprite, int X, int Y, unsigned Frame);
extern int z_sprite_getWidth(ZSpriteId Sprite);
extern int z_sprite_getHeight(ZSpriteId Sprite);
extern uint8_t z_sprite_getNumFrames(ZSpriteId Sprite);

extern void z_draw_fill(ZColorId ColorId);
extern void z_draw_rectangle(int X, int Y, int W, int H, ZColorId ColorId);
extern void z_draw_pixel(int X, int Y, ZColorId ColorId);
extern void z_draw_hline(int X1, int X2, int Y, ZColorId ColorId);
extern void z_draw_vline(int X, int Y1, int Y2, ZColorId ColorId);
extern void z_draw_circle(int X, int Y, int Radius, ZColorId ColorId);

Z_EXTERN_C_END
