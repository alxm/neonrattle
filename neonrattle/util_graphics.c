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

#include "util_graphics.h"

#include "generated/data_gfx_palette.h"
#include "generated/data_gfx_alxm.h"
#include "generated/data_gfx_alxm2.h"
#include "generated/data_gfx_alxm2glow.h"
#include "generated/data_gfx_alxm_wing.h"
#include "generated/data_gfx_neonrattle.h"
#include "generated/data_gfx_neonrattle_glow.h"
#include "generated/data_gfx_maps_grid16x16.h"
#include "generated/data_gfx_tiles_grid16x16.h"
#include "generated/data_gfx_snake_mask_grid8x8.h"
#include "generated/data_gfx_snake_minimap.h"
#include "generated/data_gfx_apple_mask.h"
#include "generated/data_gfx_apple_halo.h"
#include "generated/data_gfx_icon_apple.h"
#include "generated/data_gfx_icon_check.h"
#include "generated/data_gfx_icon_heart.h"
#include "generated/data_gfx_icon_hi.h"
#include "generated/data_gfx_icon_lock.h"
#include "generated/data_gfx_icon_lvl.h"
#include "generated/data_gfx_font_lcdnum_grid4x7.h"
#include "generated/data_gfx_font_smallnum_grid3x5.h"

static ZRgb g_rgb;
static int g_alpha;
static ZAlign g_align;

void z_graphics_setup(void)
{
    z_sprite_load(Z_SPRITE_PALETTE, palette);
    z_sprite_load(Z_SPRITE_ALXM, alxm);
    z_sprite_load(Z_SPRITE_ALXM2, alxm2);
    z_sprite_load(Z_SPRITE_ALXM2GLOW, alxm2glow);
    z_sprite_load(Z_SPRITE_ALXM_WING, alxm_wing);
    z_sprite_load(Z_SPRITE_NEONRATTLE, neonrattle);
    z_sprite_load(Z_SPRITE_NEONRATTLE_GLOW, neonrattle_glow);
    z_sprite_load(Z_SPRITE_MAPS, maps_grid16x16);
    z_sprite_load(Z_SPRITE_TILES, tiles_grid16x16);
    z_sprite_load(Z_SPRITE_SNAKE_MASK, snake_mask_grid8x8);
    z_sprite_load(Z_SPRITE_SNAKE_MINIMAP, snake_minimap);
    z_sprite_load(Z_SPRITE_APPLE_MASK, apple_mask);
    z_sprite_load(Z_SPRITE_APPLE_HALO, apple_halo);
    z_sprite_load(Z_SPRITE_ICON_APPLE, icon_apple);
    z_sprite_load(Z_SPRITE_ICON_CHECK, icon_check);
    z_sprite_load(Z_SPRITE_ICON_HEART, icon_heart);
    z_sprite_load(Z_SPRITE_ICON_HI, icon_hi);
    z_sprite_load(Z_SPRITE_ICON_LOCK, icon_lock);
    z_sprite_load(Z_SPRITE_ICON_LVL, icon_lvl);
    z_sprite_load(Z_SPRITE_FONT_LCDNUM, font_lcdnum_grid4x7);
    z_sprite_load(Z_SPRITE_FONT_SMALLNUM, font_smallnum_grid3x5);

    int palWidth = z_sprite_sizeGetWidth(Z_SPRITE_PALETTE);
    int palHeight = z_sprite_sizeGetHeight(Z_SPRITE_PALETTE) - 1;
    const ZPixel* pixels = z_sprite_pixelsGet(Z_SPRITE_PALETTE, 0) + palWidth;
    int color = 0;

    for(int p = palWidth * palHeight; p--; ) {
        ZPixel pixel = *pixels++;

        if(pixel == 0) {
            continue;
        }

        z_colors[color].pixel = pixel;
        z_colors[color].rgb = z_pixel_toRgb(pixel);

        if(++color == Z_COLOR_NUM) {
            break;
        }
    }

    z_graphics_stateAlignReset();
}

void z_graphics_stateColorSetId(ZColorId Color)
{
    g_rgb = z_colors[Color].rgb;
}

void z_graphics_stateColorSetRgb(const ZRgb* Rgb)
{
    g_rgb = *Rgb;
}

void z_graphics_stateAlphaSet(int Alpha)
{
    g_alpha = Alpha;
}

void z_graphics_stateAlignSet(ZAlign Alignment)
{
    g_align = Alignment;
}

void z_graphics_stateAlignReset(void)
{
    g_align = Z_ALIGN_X_LEFT | Z_ALIGN_Y_TOP;
}

void z_sprite_blit(ZSpriteId Sprite, int X, int Y, unsigned Frame)
{
    ZVectorInt spriteSize = z_sprite_sizeGet(Sprite);

    if(g_align & Z_ALIGN_X_CENTER) {
        X -= spriteSize.x >> 1;
    } else if(g_align & Z_ALIGN_X_RIGHT) {
        X -= spriteSize.x;
    }

    if(g_align & Z_ALIGN_Y_CENTER) {
        Y -= spriteSize.y >> 1;
    } else if(g_align & Z_ALIGN_Y_BOTTOM) {
        Y -= spriteSize.y;
    }

    z_platform_spriteBlit(Sprite, X, Y, Frame);
}

void z_sprite_blitAlphaMask(ZSpriteId AlphaMask, int X, int Y, unsigned Frame)
{
    const ZRgb* Rgb = &g_rgb;
    int Alpha = g_alpha;

    if(Alpha == 0) {
        return;
    }

    ZVectorInt spriteSize = z_sprite_sizeGet(AlphaMask);

    if(g_align & Z_ALIGN_X_CENTER) {
        X -= spriteSize.x >> 1;
    } else if(g_align & Z_ALIGN_X_RIGHT) {
        X -= spriteSize.x;
    }

    if(g_align & Z_ALIGN_Y_CENTER) {
        Y -= spriteSize.y >> 1;
    } else if(g_align & Z_ALIGN_Y_BOTTOM) {
        Y -= spriteSize.y;
    }

    if(X >= Z_SCREEN_W || X + spriteSize.x <= 0
        || Y >= Z_SCREEN_H || Y + spriteSize.y <= 0) {

        return;
    }

    ZPixel* screenPixels = z_screen_pixelsGet();
    const ZPixel* spritePixels = z_sprite_pixelsGet(AlphaMask, Frame);

    int spriteDrawW = spriteSize.x;
    int spriteDrawH = spriteSize.y;

    if(X < 0) {
        spritePixels += -X;
        spriteDrawW -= -X;
        X = 0;
    }

    if(X + spriteDrawW > Z_SCREEN_W) {
        spriteDrawW = Z_SCREEN_W - X;
    }

    if(Y < 0) {
        spritePixels += -Y * spriteSize.x;
        spriteDrawH -= -Y;
        Y = 0;
    }

    if(Y + spriteDrawH > Z_SCREEN_H) {
        spriteDrawH = Z_SCREEN_H - Y;
    }

    screenPixels += Y * Z_SCREEN_W + X;

    for(int y = spriteDrawH; y--; ) {
        ZPixel* screenPixels2 = screenPixels;
        const ZPixel* spritePixels2 = spritePixels;

        for(int x = spriteDrawW; x--; ) {
            int a = (Alpha * z_pixel_toAnyChannel(*spritePixels2++)) >> 8;

            if(a != 0) {
                z_draw_pixelBufferRgbAlpha(screenPixels2, Rgb, a);
            }

            screenPixels2++;
        }

        screenPixels += Z_SCREEN_W;
        spritePixels += spriteSize.x;
    }
}

void z_draw_rectangleAlpha(int X, int Y, int W, int H, ZColorId Color, int Alpha)
{
    if(Alpha == 0
        || X >= Z_SCREEN_W || X + W <= 0 || Y >= Z_SCREEN_H || Y + H <= 0) {

        return;
    }

    if(X < 0) {
        W -= -X;
        X = 0;
    }

    if(X + W > Z_SCREEN_W) {
        W = Z_SCREEN_W - X;
    }

    if(Y < 0) {
        H -= -Y;
        Y = 0;
    }

    if(Y + H > Z_SCREEN_H) {
        H = Z_SCREEN_H - Y;
    }

    const ZRgb* rgb = &z_colors[Color].rgb;
    ZPixel* screenPixels = z_screen_pixelsGet() + Y * Z_SCREEN_W + X;

    for(int y = H; y--; ) {
        ZPixel* screenPixels2 = screenPixels;

        for(int x = W; x--; ) {
            z_draw_pixelBufferRgbAlpha(screenPixels2++, rgb, Alpha);
        }

        screenPixels += Z_SCREEN_W;
    }
}

void z_draw_hline(int X1, int X2, int Y, ZColorId Color)
{
    z_draw_rectangle(X1, Y, X2 - X1 + 1, 1, Color);
}

void z_draw_vline(int X, int Y1, int Y2, ZColorId Color)
{
    z_draw_rectangle(X, Y1, 1, Y2 - Y1 + 1, Color);
}
