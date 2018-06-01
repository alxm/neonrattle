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

#include "generated/data_gfx_alxm.h"
#include "generated/data_gfx_alxm_wing.h"

void z_graphics_setup(void)
{
    z_sprite_load(Z_SPRITE_ALXM, alxm);
    z_sprite_load(Z_SPRITE_ALXM_WING, alxm_wing);
}

void z_sprite_blitCentered(ZSpriteId Sprite, int X, int Y, uint8_t Frame)
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
