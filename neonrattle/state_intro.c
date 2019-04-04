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

#include "state_intro.h"

#include "util_graphics.h"
#include "util_input.h"
#include "util_swipe.h"
#include "util_timer.h"

#define Z_LOGO_WAIT_MS 800

static const int8_t g_lines[] = {
    -1,
    8, 4, -1,
    8, 3, -1,
    7, 2, 6, 4, -1,
    7, 1, 6, 3, -1,
    6, 0, 5, 2, 4, 4, -1,
    5, 1, 4, 3, -1,
    4, 0, 3, 2, 2, 4, -1,
    3, 1, 2, 3, -1,
    2, 0, 1, 2, -1,
    1, 1, -1,
    0, 0, -1,
    -2
};

static int g_pc;

void s_intro_init(void)
{
    z_graphics_colorSetId(Z_COLOR_ALXM_BG);

    z_draw_fill();
}

void s_intro_tick(void)
{
    if(z_timer_isExpired(Z_TIMER_G1)
        || z_button_pressGetOnce(Z_BUTTON_A)
        || z_button_pressGetOnce(Z_BUTTON_B)) {

        z_state_set(Z_STATE_MENU);
        z_swipe_start(Z_SWIPE_FADE_HIDE);

        g_pc = Z_ARRAY_LEN(g_lines) - 1;
    }

    if(z_state_changed() || z_timer_isRunning(Z_TIMER_G1)) {
        return;
    }

    while(g_lines[g_pc] >= 0) {
        g_pc++;
    }

    if(g_lines[g_pc] == -1) {
        g_pc++;
    }

    if(g_lines[g_pc] == -2) {
        z_timer_start(Z_TIMER_G1, Z_LOGO_WAIT_MS, false);
    }
}

void s_intro_draw(void)
{
    z_sprite_align(Z_ALIGN_X_CENTER | Z_ALIGN_Y_CENTER);
    z_sprite_blit(Z_SPRITE_ALXM, 0, Z_SCREEN_W / 2, Z_SCREEN_H / 2);

    if(g_lines[g_pc] < 0) {
        return;
    }

    ZVectorInt logoSize = z_sprite_sizeGet(Z_SPRITE_ALXM);
    int startX1 = Z_SCREEN_W / 2 - logoSize.x / 2;
    int startX2 = Z_SCREEN_W / 2 + (logoSize.x + 1) / 2 - 2;
    int startY = Z_SCREEN_H / 2 - logoSize.y / 2;

    z_graphics_colorSetId(Z_COLOR_ALXM_BG);

    for(int pc = g_pc; g_lines[pc] != -2; pc++) {
        while(g_lines[pc] != -1) {
            int8_t x = g_lines[pc++];
            int8_t y = g_lines[pc++];

            z_draw_rectangle(startX1 + x, startY + y, 2, 1);
            z_draw_rectangle(startX2 - x, startY + y, 2, 1);
        }
    }
}
