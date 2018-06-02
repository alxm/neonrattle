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
#include "util_swipe.h"

#include "util_fix.h"

typedef void (ZSwipeInit)(void);
typedef bool (ZSwipeTick)(void);
typedef void (ZSwipeDraw)(void);

#define Z_SLIDE_CLOSE_INC (64)
#define Z_SLIDE_OPEN_INC  (64)

static unsigned g_counter;
static ZSwipeId* g_swipePtr;

static void swipeHideInit(void)
{
    g_counter = 0;
}

static bool swipeHideTick(void)
{
    g_counter += Z_SLIDE_CLOSE_INC;

    return g_counter == Z_INT_DEG_090;
}

static void swipeShowInit(void)
{
    g_counter = Z_INT_DEG_090;
}

static bool swipeShowTick(void)
{
    g_counter -= Z_SLIDE_OPEN_INC;

    return g_counter == 0;
}

static void swipeDraw(void)
{
    int h = z_fix_toInt(z_fix_sin(g_counter) * (Z_SCREEN_H / 2));

    if(h > 0) {
        int rH = h - 1;

        z_draw_rectangle(0, 0, Z_SCREEN_W, rH, Z_COLOR_BG_GREEN_01);
        z_draw_hline(0, Z_SCREEN_W - 1, rH, Z_COLOR_BG_GREEN_03);

        z_draw_hline(0, Z_SCREEN_W - 1, Z_SCREEN_H - h, Z_COLOR_BG_GREEN_03);
        z_draw_rectangle(0, Z_SCREEN_H - rH, Z_SCREEN_W, rH, Z_COLOR_BG_GREEN_01);
    }
}

static const struct {
    ZSwipeInit* init;
    ZSwipeTick* tick;
    ZSwipeDraw* draw;
    ZSfxId sfx;
} g_callbacks[Z_SWIPE_NUM] = {
    [Z_SWIPE_HIDE] = {swipeHideInit, swipeHideTick, swipeDraw, Z_SFX_SWIPE_HIDE},
    [Z_SWIPE_SHOW] = {swipeShowInit, swipeShowTick, swipeDraw, Z_SFX_SWIPE_SHOW},
};

void z_swipe_init(ZSwipeId* Swipe)
{
    g_swipePtr = Swipe;

    if(*g_swipePtr != Z_SWIPE_INVALID) {
        g_callbacks[*g_swipePtr].init();

        if(g_callbacks[*g_swipePtr].sfx != Z_SFX_INVALID) {
            z_sfx_play(g_callbacks[*g_swipePtr].sfx);
        }
    }
}

void z_swipe_tick(void)
{
    if(g_swipePtr != NULL && *g_swipePtr != Z_SWIPE_INVALID) {
        if(g_callbacks[*g_swipePtr].tick()) {
            *g_swipePtr = Z_SWIPE_INVALID;
            g_swipePtr = NULL;
        }
    }
}

void z_swipe_draw(void)
{
    if(g_swipePtr != NULL && *g_swipePtr != Z_SWIPE_INVALID) {
        g_callbacks[*g_swipePtr].draw();
    }
}
