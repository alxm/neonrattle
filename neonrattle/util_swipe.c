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

#include "util_swipe.h"

#include "util_graphics.h"
#include "util_sound.h"

#define Z_ANGLE_INC (Z_DEG_090_FIX / 16)

typedef void (ZSwipeInit)(void);
typedef bool (ZSwipeTick)(void);
typedef void (ZSwipeDraw)(void);

static ZFixu g_angle;
static ZSwipeId g_swipe = Z_SWIPE_INVALID;

static void drawFadeHide(void)
{
    z_graphics_colorSetId(Z_COLOR_BG_GREEN_01);
    z_graphics_alphaSet(z_fix_toInt(z_fix_sinf(g_angle) * 256));

    z_draw_rectangleAlpha(0, 0, Z_SCREEN_W, Z_SCREEN_H);
}

static void drawFadeShow(void)
{
    z_graphics_colorSetId(Z_COLOR_BG_GREEN_01);
    z_graphics_alphaSet(
        z_fix_toInt(z_fix_sinf(Z_DEG_090_FIX - g_angle) * 256));

    z_draw_rectangleAlpha(0, 0, Z_SCREEN_W, Z_SCREEN_H);
}

static void drawLines(ZFixu Angle)
{
    ZFix sine = z_fix_sinf(Angle);
    int h = z_fix_toInt(sine * (Z_SCREEN_H / 2));
    int alpha = z_fix_toInt(sine * 256);

    z_graphics_colorSetId(Z_COLOR_BG_GREEN_01);
    z_graphics_alphaSet(alpha);

    z_draw_rectangleAlpha(0, 0, Z_SCREEN_W, h);
    z_draw_rectangleAlpha(0, Z_SCREEN_H - h, Z_SCREEN_W, h);

    z_graphics_colorSetId(Z_COLOR_BG_GREEN_02);

    z_draw_hline(0, Z_SCREEN_W - 1, h);
    z_draw_hline(0, Z_SCREEN_W - 1, Z_SCREEN_H - h - 1);
}

static void drawLinesHide(void)
{
    drawLines(g_angle);
}

static void drawLinesShow(void)
{
    drawLines(Z_DEG_090_FIX - g_angle);
}

static const struct {
    ZSwipeDraw* draw;
    ZSfxId sfx;
} g_callbacks[Z_SWIPE_NUM] = {
    [Z_SWIPE_FADE_HIDE] = {drawFadeHide, Z_SFX_SWIPE_HIDE},
    [Z_SWIPE_FADE_SHOW] = {drawFadeShow, Z_SFX_SWIPE_SHOW},
    [Z_SWIPE_LINES_HIDE] = {drawLinesHide, Z_SFX_SWIPE_HIDE},
    [Z_SWIPE_LINES_SHOW] = {drawLinesShow, Z_SFX_SWIPE_SHOW},
};

void z_swipe_start(ZSwipeId Swipe)
{
    g_swipe = Swipe;

    if(g_swipe != Z_SWIPE_INVALID) {
        g_angle = 0;

        z_sfx_play(g_callbacks[g_swipe].sfx);
    }
}

void z_swipe_stop(void)
{
    g_swipe = Z_SWIPE_INVALID;
}

bool z_swipe_running(void)
{
    return g_swipe != Z_SWIPE_INVALID;
}

void z_swipe_tick(void)
{
    if(g_swipe != Z_SWIPE_INVALID) {
        g_angle += Z_ANGLE_INC;

        if(g_angle > Z_DEG_090_FIX) {
            g_swipe = Z_SWIPE_INVALID;
        }
    }
}

void z_swipe_draw(void)
{
    if(g_swipe != Z_SWIPE_INVALID) {
        g_callbacks[g_swipe].draw();
    }
}
