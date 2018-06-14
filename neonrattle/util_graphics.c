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

#include "platform.h"
#include "util_graphics.h"

#include "generated/data_gfx_palette.h"

#include "generated/data_gfx_map0.h"

#include "generated/data_gfx_tiles.h"

#include "generated/data_gfx_snake_alphamask1.h"

#include "generated/data_gfx_apple_alphamask1.h"
#include "generated/data_gfx_apple_halo.h"

#include "generated/data_gfx_alxm.h"
#include "generated/data_gfx_alxm_wing.h"

void z_graphics_setup(void)
{
    z_sprite_load(Z_SPRITE_PALETTE, palette);

    z_sprite_load(Z_SPRITE_MAP0, map0);

    z_sprite_load(Z_SPRITE_TILES, tiles);

    z_sprite_load(Z_SPRITE_SNAKE_ALPHAMASK, snake_alphamask1);

    z_sprite_load(Z_SPRITE_APPLE_ALPHAMASK, apple_alphamask1);
    z_sprite_load(Z_SPRITE_APPLE_HALO, apple_halo);

    z_sprite_load(Z_SPRITE_ALXM, alxm);
    z_sprite_load(Z_SPRITE_ALXM_WING, alxm_wing);

    int row = 1;
    int col = 0;

    for(ZColorId c = 0; c < Z_COLOR_NUM; c++, col++) {
        ZPixel pixel = z_sprite_getPixel(Z_SPRITE_PALETTE, 0, col, row);

        while(pixel == 0) {
            row++;
            col = 0;
            pixel = z_sprite_getPixel(Z_SPRITE_PALETTE, 0, col, row);
        }

        z_colors[c].pixel = pixel;
        z_pixel_toRGB(
            pixel, &z_colors[c].r, &z_colors[c].g, &z_colors[c].b);
    }
}

void z_sprite_blitCentered(ZSpriteId Sprite, int X, int Y, unsigned Frame)
{
    z_sprite_blit(Sprite,
                  X - z_sprite_getWidth(Sprite) / 2,
                  Y - z_sprite_getHeight(Sprite) / 2,
                  Frame);
}

void z_sprite_blitAlphaMask(ZSpriteId AlphaMask, int X, int Y, unsigned Frame, ZColorId Fill, int Alpha)
{
    z_sprite_blitAlphaMaskRGBA(AlphaMask,
                               X,
                               Y,
                               Frame,
                               z_colors[Fill].r,
                               z_colors[Fill].g,
                               z_colors[Fill].b,
                               Alpha);
}

void z_sprite_blitAlphaMaskRGBA(ZSpriteId AlphaMask, int X, int Y, unsigned Frame, int R, int G, int B, int Alpha)
{
    if(Alpha == 0) {
        return;
    }

    const int spriteW = z_sprite_getWidth(AlphaMask);
    const int spriteH = z_sprite_getHeight(AlphaMask);

    X -= spriteW / 2;
    Y -= spriteH / 2;

    if(X >= Z_SCREEN_W || X + spriteW <= 0
        || Y >= Z_SCREEN_H || Y + spriteH <= 0) {

        return;
    }

    ZPixel* screenPixels = z_screen_getPixels();
    const ZPixel* spritePixels = z_sprite_getPixels(AlphaMask, Frame);

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
            int a = (Alpha * z_pixel_toR(*spritePixels2++)) >> 8;

            if(a != 0) {
                z_pixel_drawAlpha(screenPixels2, R, G, B, a);
            }

            screenPixels2++;
        }

        screenPixels += Z_SCREEN_W;
        spritePixels += spriteW;
    }
}

void z_draw_hline(int X1, int X2, int Y, ZColorId ColorId)
{
    z_draw_rectangle(X1, Y, X2 - X1 + 1, 1, ColorId);
}

void z_draw_vline(int X, int Y1, int Y2, ZColorId ColorId)
{
    z_draw_rectangle(X, Y1, 1, Y2 - Y1 + 1, ColorId);
}
