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

#include "util_swipe.h"

#include "util_graphics.h"
#include "util_sound.h"

typedef void (ZSwipeInit)(void);
typedef bool (ZSwipeTick)(void);
typedef void (ZSwipeDraw)(void);

static ZFixu g_angle;
static ZSwipeId g_swipe = Z_SWIPE_INVALID;

static void swipeInit(void)
{
    g_angle = 0;
}

static bool swipeTick(void)
{
    g_angle += Z_FIX_DEG_090 / 16;

    return g_angle >= Z_FIX_DEG_090;
}

static void swipeDraw(ZFixu Angle)
{
    ZFix sine = z_fix_sinf(Angle);
    int h = z_fix_toInt(sine * (Z_SCREEN_H / 2));
    int alpha = z_fix_toInt(sine * 256);

    z_draw_rectangleAlpha(
        0, 0, Z_SCREEN_W, h, Z_COLOR_BG_GREEN_01, alpha);
    z_draw_hline(
        0, Z_SCREEN_W - 1, h, Z_COLOR_BG_GREEN_02);

    z_draw_hline(
        0, Z_SCREEN_W - 1, Z_SCREEN_H - h - 1, Z_COLOR_BG_GREEN_02);
    z_draw_rectangleAlpha(
        0, Z_SCREEN_H - h, Z_SCREEN_W, h, Z_COLOR_BG_GREEN_01, alpha);
}

static void swipeDrawIntro(void)
{
    int alpha = z_fix_toInt(z_fix_sinf(g_angle) * 256);

    z_draw_rectangleAlpha(
        0, 0, Z_SCREEN_W, Z_SCREEN_H, Z_COLOR_BG_GREEN_01, alpha);
}

static void swipeDrawHide(void)
{
    swipeDraw(g_angle);
}

static void swipeDrawShow(void)
{
    swipeDraw(Z_FIX_DEG_090 - 1 - g_angle);
}

static const struct {
    ZSwipeInit* init;
    ZSwipeTick* tick;
    ZSwipeDraw* draw;
    ZSfxId sfx;
} g_callbacks[Z_SWIPE_NUM] = {
    [Z_SWIPE_INTRO] = {swipeInit, swipeTick, swipeDrawIntro, Z_SFX_INVALID},
    [Z_SWIPE_HIDE] = {swipeInit, swipeTick, swipeDrawHide, Z_SFX_SWIPE_HIDE},
    [Z_SWIPE_SHOW] = {swipeInit, swipeTick, swipeDrawShow, Z_SFX_SWIPE_SHOW},
};

void z_swipe_start(ZSwipeId Swipe)
{
    g_swipe = Swipe;

    if(g_swipe != Z_SWIPE_INVALID) {
        g_callbacks[g_swipe].init();

        if(g_callbacks[g_swipe].sfx != Z_SFX_INVALID) {
            z_sfx_play(g_callbacks[g_swipe].sfx);
        }
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
        if(g_callbacks[g_swipe].tick()) {
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
