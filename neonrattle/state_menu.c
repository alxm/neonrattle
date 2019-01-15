/*
    Copyright 2019 Alex Margarit <alex@alxm.org>

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

#include "state_menu.h"

#include "obj_game.h"
#include "util_coords.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_swipe.h"

#define Z_LEVELS_NUM 32
#define Z_GRID_W 8
#define Z_GRID_H 4
#define Z_CELL_DIM 8
#define Z_SELECT_DELAY_DS 2

static unsigned g_cursor;

void s_menu_init(void)
{
    z_input_reset();

    z_swipe_start(Z_SWIPE_SHOW);
}

void s_menu_tick(void)
{
    if(z_state_changed()) {
        return;
    }

    if(z_button_pressGetDelay(Z_BUTTON_UP, Z_SELECT_DELAY_DS)) {
        g_cursor -= Z_GRID_W;
    }

    if(z_button_pressGetDelay(Z_BUTTON_DOWN, Z_SELECT_DELAY_DS)) {
        g_cursor += Z_GRID_W;
    }

    if(z_button_pressGetDelay(Z_BUTTON_LEFT, Z_SELECT_DELAY_DS)) {
        g_cursor--;
    }

    if(z_button_pressGetDelay(Z_BUTTON_RIGHT, Z_SELECT_DELAY_DS)) {
        g_cursor++;
    }

    g_cursor &= Z_LEVELS_NUM - 1;

    if(z_button_pressGet(Z_BUTTON_A) || z_button_pressGet(Z_BUTTON_B)) {
        o_game_setup(g_cursor);

        z_state_set(Z_STATE_START);
        z_swipe_start(Z_SWIPE_HIDE);
    }
}

void s_menu_draw(void)
{
    for(int y = 0; y < Z_SCREEN_H / Z_COORDS_UNIT_PIXELS + 1; y++) {
        for(int x = 0; x < Z_SCREEN_W / Z_COORDS_UNIT_PIXELS + 1; x++) {
            z_sprite_blit(Z_SPRITE_TILES,
                          x * Z_COORDS_UNIT_PIXELS - Z_COORDS_UNIT_PIXELS / 8,
                          y * Z_COORDS_UNIT_PIXELS - Z_COORDS_UNIT_PIXELS / 2,
                          8 + (unsigned)(!(y & 1) * 2 + (x & 1)));
        }
    }

    z_sprite_blitAlphaMask(Z_SPRITE_NEONRATTLE,
                           8 + z_sprite_widthGet(Z_SPRITE_NEONRATTLE) / 2,
                           10 + z_sprite_heightGet(Z_SPRITE_NEONRATTLE) / 2,
                           0,
                           Z_COLOR_APPLE_02,
                           256);

    for(unsigned l = 0; l < Z_LEVELS_NUM; l++) {
        ZColorId color;
        int alpha;

        if(l == g_cursor) {
            color = Z_COLOR_SNAKE_01;
            alpha = 224;
        } else {
            color = Z_COLOR_APPLE_02;
            alpha = 192;
        }

        z_draw_rectangleAlpha((int)(8 + (l & (Z_GRID_W - 1)) * Z_CELL_DIM),
                              (int)(19 + (l / Z_GRID_W) * Z_CELL_DIM),
                              Z_CELL_DIM - 1,
                              Z_CELL_DIM - 1,
                              color,
                              alpha);
    }
}

void s_menu_free(void)
{
    //
}

void s_menu_selectNext(void)
{
    g_cursor = (g_cursor + 1) & (Z_LEVELS_NUM - 1);
}
