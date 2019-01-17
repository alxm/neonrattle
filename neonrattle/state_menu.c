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

#include "obj_camera.h"
#include "obj_game.h"
#include "util_coords.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_sound.h"
#include "util_swipe.h"

#define Z_LEVELS_NUM 32
#define Z_GRID_W 8
#define Z_GRID_H 4
#define Z_CELL_DIM 8
#define Z_SELECT_DELAY_DS 2

static unsigned g_cursor;
static unsigned g_lastUnlocked;

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

    bool changed = false;

    if(z_button_pressGetDelay(Z_BUTTON_UP, Z_SELECT_DELAY_DS)) {
        g_cursor -= Z_GRID_W;
        changed = true;
    }

    if(z_button_pressGetDelay(Z_BUTTON_DOWN, Z_SELECT_DELAY_DS)) {
        g_cursor += Z_GRID_W;
        changed = true;
    }

    if(z_button_pressGetDelay(Z_BUTTON_LEFT, Z_SELECT_DELAY_DS)) {
        g_cursor--;
        changed = true;
    }

    if(z_button_pressGetDelay(Z_BUTTON_RIGHT, Z_SELECT_DELAY_DS)) {
        g_cursor++;
        changed = true;
    }

    if(changed) {
        g_cursor &= Z_LEVELS_NUM - 1;
        z_sfx_play(Z_SFX_MENU_BROWSE);
    }

    if(z_button_pressGetOnce(Z_BUTTON_A) || z_button_pressGetOnce(Z_BUTTON_B)) {
        if(g_cursor > g_lastUnlocked) {
            o_camera_shakeSet(1);
            z_sfx_play(Z_SFX_MENU_REJECT);
        } else {
            o_game_setup(g_cursor);

            z_state_set(Z_STATE_START);
            z_swipe_start(Z_SWIPE_HIDE);
            z_sfx_play(Z_SFX_MENU_SELECT);
        }
    }

    o_camera_tick((ZVectorFix){0, 0});
}

void s_menu_draw(void)
{
    ZVectorInt shake = o_camera_shakeGet();
    int offsetX = -shake.x - Z_COORDS_UNIT_PIXELS / 8;
    int offsetY = -shake.y - Z_COORDS_UNIT_PIXELS / 2;

    for(int y = 0; y < Z_SCREEN_H / Z_COORDS_UNIT_PIXELS + 1; y++) {
        for(int x = 0; x < Z_SCREEN_W / Z_COORDS_UNIT_PIXELS + 1; x++) {
            z_sprite_blit(Z_SPRITE_TILES,
                          offsetX + x * Z_COORDS_UNIT_PIXELS,
                          offsetY + y * Z_COORDS_UNIT_PIXELS,
                          8 + (unsigned)(!(y & 1) * 2 + (x & 1)));
        }
    }

    z_sprite_blitAlphaMask(
        Z_SPRITE_NEONRATTLE,
        shake.x + 8 + z_sprite_widthGet(Z_SPRITE_NEONRATTLE) / 2,
        shake.y + 10 + z_sprite_heightGet(Z_SPRITE_NEONRATTLE) / 2,
        0,
        Z_COLOR_APPLE_02,
        256);

    for(unsigned l = 0; l < Z_LEVELS_NUM; l++) {
        int alpha;
        ZColorId colorBg, colorSprite;
        ZSpriteId sprite = l > g_lastUnlocked
                            ? Z_SPRITE_ICON_LOCK : Z_SPRITE_ICON_CHECK;

        if(l == g_cursor) {
            alpha = 224;
            colorBg = Z_COLOR_SNAKE_01;
            colorSprite = Z_COLOR_APPLE_02;
        } else {
            if(l > g_lastUnlocked) {
                alpha = 128;
                colorBg = Z_COLOR_BG_GREEN_03;
                colorSprite = Z_COLOR_BG_GREEN_04;
            } else {
                alpha = 192;
                colorBg = Z_COLOR_APPLE_02;
                colorSprite = Z_COLOR_SNAKE_01;
            }
        }

        int drawX = shake.x + (int)(8 + (l & (Z_GRID_W - 1)) * Z_CELL_DIM);
        int drawY = shake.y + (int)(19 + (l / Z_GRID_W) * Z_CELL_DIM);

        z_draw_rectangleAlpha(
            drawX, drawY, Z_CELL_DIM - 1, Z_CELL_DIM - 1, colorBg, alpha);

        z_sprite_blitAlphaMask(sprite,
                               drawX + (Z_CELL_DIM - 1) / 2,
                               drawY + (Z_CELL_DIM - 1) / 2,
                               0,
                               colorSprite,
                               256);
    }
}

void s_menu_free(void)
{
    //
}

void s_menu_selectNext(void)
{
    g_cursor = (g_cursor + 1) & (Z_LEVELS_NUM - 1);

    if(g_cursor > g_lastUnlocked) {
        g_lastUnlocked++;
    }
}
