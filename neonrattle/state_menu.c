/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
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

#include "state_menu.h"

#include "obj_camera.h"
#include "obj_map.h"
#include "obj_game.h"
#include "util_coords.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_light.h"
#include "util_save.h"
#include "util_sound.h"
#include "util_swipe.h"

#define Z_GRID_W 8
#define Z_GRID_H (Z_LEVELS_NUM / Z_GRID_W)
#define Z_CELL_DIM (Z_COORDS_PIXELS_PER_UNIT / 2)
#define Z_SELECT_DELAY_MS 200
#define Z_MOVE_SPEED (Z_FIX_ONE * 2)
#define Z_GLOW_SPEED (Z_DEG_001_FIX * 4)

static unsigned g_cursor = UINT_MAX;
static ZVectorFix g_origin;
static ZVectorFix g_velocity;
static ZFixu g_angle;

void s_menu_init(void)
{
    if(g_cursor == UINT_MAX) {
        g_cursor = z_save_unlockedGet();
    }

    g_velocity.x = 8 * Z_MOVE_SPEED * (z_random_int(2) ? 1 : -1);
    g_velocity.y = 8 * Z_MOVE_SPEED * (z_random_int(2) ? 1 : -1);

    z_input_reset();

    z_swipe_start(Z_SWIPE_FADE_SHOW);
}

void s_menu_tick(void)
{
    if(z_state_changed()) {
        return;
    }

    ZVectorFix velocity = {0, 0};

    if(z_button_pressGetDelay(Z_BUTTON_UP, Z_SELECT_DELAY_MS)) {
        g_cursor -= Z_GRID_W;
        velocity.y = Z_MOVE_SPEED;
    }

    if(z_button_pressGetDelay(Z_BUTTON_DOWN, Z_SELECT_DELAY_MS)) {
        g_cursor += Z_GRID_W;
        velocity.y = -Z_MOVE_SPEED;
    }

    if(z_button_pressGetDelay(Z_BUTTON_LEFT, Z_SELECT_DELAY_MS)) {
        g_cursor--;
        velocity.x = Z_MOVE_SPEED;
    }

    if(z_button_pressGetDelay(Z_BUTTON_RIGHT, Z_SELECT_DELAY_MS)) {
        g_cursor++;
        velocity.x = -Z_MOVE_SPEED;
    }

    if(velocity.x != 0 || velocity.y != 0) {
        g_cursor &= Z_LEVELS_NUM - 1;
        g_velocity = velocity;

        z_sfx_play(Z_SFX_MENU_BROWSE);
    } else {
        g_velocity.x -= (g_velocity.x >> 2);
        g_velocity.y -= (g_velocity.y >> 2);
    }

    #define Z_OFFSET_MASK (z_fix_fromInt(2 * Z_COORDS_PIXELS_PER_UNIT) - 1)
    g_origin.x = (g_origin.x + g_velocity.x) & Z_OFFSET_MASK;
    g_origin.y = (g_origin.y + g_velocity.y) & Z_OFFSET_MASK;

    if(z_button_pressGetOnce(Z_BUTTON_A) || z_button_pressGetOnce(Z_BUTTON_B)) {
        if(g_cursor > z_save_unlockedGet()) {
            n_camera_shakeSet(200);
            z_sfx_play(Z_SFX_MENU_REJECT);
        } else {
            n_game_new(g_cursor);

            z_state_set(Z_STATE_START);
            z_swipe_start(Z_SWIPE_FADE_HIDE);
            z_sfx_play(Z_SFX_MENU_SELECT);
            z_light_pulseSet(Z_LIGHT_GAME_START);
        }
    }

    n_camera_tick(n_camera_originGet());

    g_angle += Z_GLOW_SPEED;
}

static void minimapDraw(unsigned Level, int X, int Y)
{
    const ZPixel* src = z_sprite_pixelsGet(Z_SPRITE_MAPS, Level);
    ZPixel* dstStart = z_screen_pixelsGet() + Y * Z_SCREEN_W + X;
    ZPixel white = z_colors[Z_COLOR_MAP_WHITE].pixel;
    const ZRgb* rgb[] = {
        &z_colors[Z_COLOR_APPLE_02].rgb,
        &z_colors[Z_COLOR_SNAKE_02].rgb,
    };
    int alpha[] = {192, 224};

    for(int y = N_MAP_H; y--; dstStart += Z_SCREEN_W) {
        ZPixel* dst = dstStart;

        for(int x = N_MAP_W; x--; dst++, src++) {
            bool isWall = *src == white;
            z_draw_pixelBufferRgbAlpha(dst, rgb[isWall], alpha[isWall]);
        }
    }

    z_graphics_stateColorSetId(Z_COLOR_APPLE_02);
    z_graphics_stateAlphaSet(92);

    z_draw_rectangleAlpha(X, Y - 1, N_MAP_W, 1);
    z_draw_rectangleAlpha(X, Y + N_MAP_H, N_MAP_W, 1);
    z_draw_rectangleAlpha(X - 1, Y, 1, N_MAP_H);
    z_draw_rectangleAlpha(X + N_MAP_W, Y, 1, N_MAP_H);
}

void s_menu_draw(void)
{
    ZVectorInt shake = n_camera_shakeGet();

    int offsetX = z_fix_toInt(g_origin.x) + shake.x;
    int offsetY = z_fix_toInt(g_origin.y) + shake.y;

    if(offsetX > 0) {
        offsetX -= 2 * Z_COORDS_PIXELS_PER_UNIT;
    }

    if(offsetY > 0) {
        offsetY -= 2 * Z_COORDS_PIXELS_PER_UNIT;
    }

    #define Z_X_TILES \
        ((Z_SCREEN_W + (Z_COORDS_PIXELS_PER_UNIT - 1)) \
            / Z_COORDS_PIXELS_PER_UNIT + 2)

    #define Z_Y_TILES \
        ((Z_SCREEN_H + (Z_COORDS_PIXELS_PER_UNIT - 1)) \
            / Z_COORDS_PIXELS_PER_UNIT + 2)

    for(int y = 0; y < Z_Y_TILES; y++) {
        for(int x = 0; x < Z_X_TILES; x++) {
            z_sprite_blit(Z_SPRITE_TILES,
                          (unsigned)(8 + (y & 1) * 2 + (x & 1)),
                          offsetX + x * Z_COORDS_PIXELS_PER_UNIT,
                          offsetY + y * Z_COORDS_PIXELS_PER_UNIT);
        }
    }

    int startX = (Z_SCREEN_W - 64) / 2;
    int startY = (Z_SCREEN_H - 46) / 2;
    int drawX = startX + shake.y;
    int drawY = startY + shake.x;
    int glowAlpha = 128 + z_fix_toInt(z_fix_sinf(g_angle) * 128);
    ZVectorInt sizeTitle = z_sprite_sizeGet(Z_SPRITE_NEONRATTLE);
    ZVectorInt sizeFooter = z_sprite_sizeGet(Z_SPRITE_ALXM2);

    z_graphics_stateColorSetId(Z_COLOR_APPLE_01);
    z_graphics_stateAlphaSet(glowAlpha);
    z_graphics_stateAlignSet(Z_ALIGN_X_CENTER | Z_ALIGN_Y_CENTER);

    z_sprite_blitAlphaMask(Z_SPRITE_NEONRATTLE_GLOW,
                           0,
                           drawX + sizeTitle.x / 2,
                           drawY + sizeTitle.y / 2);

    z_graphics_stateColorSetId(Z_COLOR_APPLE_02);
    z_graphics_stateAlphaSet(256);

    z_sprite_blitAlphaMask(Z_SPRITE_ALXM2GLOW,
                           0,
                           startX - shake.y + sizeFooter.x / 2,
                           startY - shake.x + 42 + sizeFooter.y / 2);

    z_graphics_stateAlignReset();
    z_graphics_stateColorSetId(Z_COLOR_APPLE_02);
    z_graphics_stateAlphaSet(256);

    z_sprite_blitAlphaMask(Z_SPRITE_NEONRATTLE, 0, drawX, drawY);
    z_sprite_blit(Z_SPRITE_ALXM2, 0, startX - shake.y, startY - shake.x + 42);

    int minimapX = -1, minimapY = 0;
    unsigned now = z_fps_ticksGet() / 4;
    unsigned lastUnlocked = z_save_unlockedGet();

    for(unsigned l = 0; l < Z_LEVELS_NUM; l++) {
        int drawX = (int)
            ((unsigned)startX + (l & (Z_GRID_W - 1)) * Z_CELL_DIM) - shake.x;
        int drawY = (int)
            ((unsigned)startY + 9 + (l / Z_GRID_W) * Z_CELL_DIM) - shake.y;

        ZColorId color;
        ZSpriteId sprite;

        if(l <= lastUnlocked) {
            if(l == g_cursor) {
                minimapX = drawX;
                minimapY = drawY;

                continue;
            } else if(l == lastUnlocked) {
                color = z_color_snakeGet();
                sprite = Z_SPRITE_ICON_APPLE;
            } else {
                color = Z_COLOR_SNAKE_01
                            + ((now + (l * 2 / 3)) % Z_COLOR_SNAKE_NUM);
                sprite = Z_SPRITE_ICON_CHECK;
            }
        } else {
            if(l == g_cursor) {
                color = z_color_snakeGet();
            } else {
                color = Z_COLOR_APPLE_01
                            + ((now + (l * 2 / 3)) % Z_COLOR_APPLE_NUM);
            }

            sprite = Z_SPRITE_ICON_LOCK;
        }

        z_graphics_stateColorSetId(color);
        z_graphics_stateAlphaSet(256);

        z_sprite_blitAlphaMask(sprite, 0, drawX, drawY);
    }

    if(minimapX != -1) {
        minimapDraw(g_cursor, minimapX - N_MAP_W / 4, minimapY - N_MAP_H / 4);
    }
}

void s_menu_select(unsigned Level)
{
    g_cursor = Level;
}
