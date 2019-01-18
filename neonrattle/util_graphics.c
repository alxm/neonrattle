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

#include "util_graphics.h"

#include "generated/data_gfx_palette.h"
#include "generated/data_gfx_alxm.h"
#include "generated/data_gfx_alxm_wing.h"
#include "generated/data_gfx_neonrattle.h"
#include "generated/data_gfx_neonrattle_glow.h"
#include "generated/data_gfx_maps_grid16x16.h"
#include "generated/data_gfx_tiles_grid16x16.h"
#include "generated/data_gfx_snake_mask_grid8x8.h"
#include "generated/data_gfx_snake_minimap.h"
#include "generated/data_gfx_apple_mask.h"
#include "generated/data_gfx_apple_halo.h"
#include "generated/data_gfx_icon_check.h"
#include "generated/data_gfx_icon_heart.h"
#include "generated/data_gfx_icon_lock.h"
#include "generated/data_gfx_font_lcdnum_grid4x7.h"

void z_graphics_setup(void)
{
    z_sprite_load(Z_SPRITE_PALETTE, palette);
    z_sprite_load(Z_SPRITE_ALXM, alxm);
    z_sprite_load(Z_SPRITE_ALXM_WING, alxm_wing);
    z_sprite_load(Z_SPRITE_NEONRATTLE, neonrattle);
    z_sprite_load(Z_SPRITE_NEONRATTLE_GLOW, neonrattle_glow);
    z_sprite_loadGrid(Z_SPRITE_MAPS, maps_grid16x16, 16, 16);
    z_sprite_loadGrid(Z_SPRITE_TILES, tiles_grid16x16, 16, 16);
    z_sprite_loadGrid(Z_SPRITE_SNAKE_MASK, snake_mask_grid8x8, 8, 8);
    z_sprite_load(Z_SPRITE_SNAKE_MINIMAP, snake_minimap);
    z_sprite_load(Z_SPRITE_APPLE_MASK, apple_mask);
    z_sprite_load(Z_SPRITE_APPLE_HALO, apple_halo);
    z_sprite_load(Z_SPRITE_ICON_CHECK, icon_check);
    z_sprite_load(Z_SPRITE_ICON_HEART, icon_heart);
    z_sprite_load(Z_SPRITE_ICON_LOCK, icon_lock);
    z_sprite_loadGrid(Z_SPRITE_FONT_LCDNUM, font_lcdnum_grid4x7, 4, 7);

    int row = 1;
    int col = 0;

    for(ZColorId c = 0; c < Z_COLOR_NUM; c++, col++) {
        ZPixel pixel = z_sprite_pixelGet(Z_SPRITE_PALETTE, 0, col, row);

        if(pixel == 0) {
            col = 0;

            do {
                pixel = z_sprite_pixelGet(Z_SPRITE_PALETTE, 0, col, ++row);
            } while(pixel == 0);
        }

        z_colors[c].pixel = pixel;
        z_colors[c].rgb = z_pixel_toRgb(pixel);
    }
}

void z_sprite_blitCentered(ZSpriteId Sprite, int X, int Y, unsigned Frame)
{
    z_sprite_blit(Sprite,
                  X - z_sprite_widthGet(Sprite) / 2,
                  Y - z_sprite_heightGet(Sprite) / 2,
                  Frame);
}

void z_sprite_blitAlphaMask(ZSpriteId AlphaMask, int X, int Y, unsigned Frame, ZColorId Fill, int Alpha)
{
    z_sprite_blitAlphaMaskRGBA(AlphaMask,
                               X,
                               Y,
                               Frame,
                               &z_colors[Fill].rgb,
                               Alpha);
}

void z_sprite_blitAlphaMaskRGBA(ZSpriteId AlphaMask, int X, int Y, unsigned Frame, const ZRgb* Rgb, int Alpha)
{
    if(Alpha == 0) {
        return;
    }

    const int spriteW = z_sprite_widthGet(AlphaMask);
    const int spriteH = z_sprite_heightGet(AlphaMask);

    X -= spriteW / 2;
    Y -= spriteH / 2;

    if(X >= Z_SCREEN_W || X + spriteW <= 0
        || Y >= Z_SCREEN_H || Y + spriteH <= 0) {

        return;
    }

    ZPixel* screenPixels = z_screen_pixelsGet();
    const ZPixel* spritePixels = z_sprite_pixelsGet(AlphaMask, Frame);

    int spriteDrawW = spriteW;
    int spriteDrawH = spriteH;

    if(Y < 0) {
        spritePixels += -Y * spriteW;
        spriteDrawH -= -Y;
        Y = 0;
    }

    if(Y + spriteDrawH > Z_SCREEN_H) {
        spriteDrawH -= Y + spriteDrawH - Z_SCREEN_H;
    }

    if(X < 0) {
        spritePixels += -X;
        spriteDrawW -= -X;
        X = 0;
    }

    if(X + spriteDrawW > Z_SCREEN_W) {
        spriteDrawW -= X + spriteDrawW - Z_SCREEN_W;
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
        spritePixels += spriteW;
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
        W -= X + W - Z_SCREEN_W;
    }

    if(Y < 0) {
        H -= -Y;
        Y = 0;
    }

    if(Y + H > Z_SCREEN_H) {
        H -= Y + H - Z_SCREEN_H;
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
