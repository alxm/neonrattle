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

#include "platform.h"
#include "util_graphics.h"

#include "generated/data_gfx_palette.h"

#include "generated/data_gfx_map0.h"

#include "generated/data_gfx_tiles.h"

#include "generated/data_gfx_alxm.h"
#include "generated/data_gfx_alxm_wing.h"

void z_graphics_setup(void)
{
    z_sprite_load(Z_SPRITE_PALETTE, palette);

    z_sprite_load(Z_SPRITE_MAP0, map0);

    z_sprite_load(Z_SPRITE_TILES, tiles);

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

void z_draw_hline(int X1, int X2, int Y, ZColorId ColorId)
{
    z_draw_rectangle(X1, Y, X2 - X1 + 1, 1, ColorId);
}

void z_draw_vline(int X, int Y1, int Y2, ZColorId ColorId)
{
    z_draw_rectangle(X, Y1, 1, Y2 - Y1 + 1, ColorId);
}
