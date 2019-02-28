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

#include "obj_hud.h"

#include "obj_camera.h"
#include "obj_game.h"
#include "obj_map.h"
#include "util_graphics.h"
#include "util_save.h"
#include "util_timer.h"

#define Z_HUD_APPLE_BLINK_MS 100
#define Z_HUD_APPLE_COLOR_NORMAL Z_COLOR_SNAKE_01
#define Z_HUD_APPLE_COLOR_ALERT Z_COLOR_SNAKE_03

#define Z_HUD_LIFE_BLINK_MS 200
#define Z_HUD_LIFE_COLOR_NORMAL Z_COLOR_SNAKE_01
#define Z_HUD_LIFE_COLOR_ALERT Z_COLOR_APPLE_03

typedef struct {
    ZColorId appleColor;
    ZColorId lifeColor;
} NHud;

static NHud g_hud;

void n_hud_new(void)
{
    g_hud.lifeColor = Z_HUD_LIFE_COLOR_NORMAL;
    g_hud.appleColor = Z_HUD_APPLE_COLOR_NORMAL;
}

void n_hud_tick(const OSnake* Snake)
{
    if(o_snake_flagsTest(Snake, O_SNAKE_FLAG_HURT)) {
        if(!z_timer_running(Z_TIMER_HUD_LIFE)) {
            z_timer_start(Z_TIMER_HUD_LIFE_2, Z_HUD_LIFE_BLINK_MS, true);
            g_hud.lifeColor = Z_HUD_LIFE_COLOR_ALERT;
        }

        z_timer_start(Z_TIMER_HUD_LIFE, 4 * Z_HUD_LIFE_BLINK_MS, false);
    }

    if(z_timer_expired(Z_TIMER_HUD_LIFE)) {
        z_timer_stop(Z_TIMER_HUD_LIFE_2);
        g_hud.lifeColor = Z_HUD_LIFE_COLOR_NORMAL;
    } else if(z_timer_expired(Z_TIMER_HUD_LIFE_2)) {
        g_hud.lifeColor = g_hud.lifeColor == Z_HUD_LIFE_COLOR_NORMAL
                            ? Z_HUD_LIFE_COLOR_ALERT
                            : Z_HUD_LIFE_COLOR_NORMAL;
    }

    if(o_snake_flagsTest(Snake, O_SNAKE_FLAG_ATE)) {
        if(!z_timer_running(Z_TIMER_HUD_APPLES)) {
            z_timer_start(Z_TIMER_HUD_APPLES, Z_HUD_APPLE_BLINK_MS, false);
            g_hud.appleColor = Z_HUD_APPLE_COLOR_ALERT;
        }
    } else if(z_timer_expired(Z_TIMER_HUD_APPLES)) {
        g_hud.appleColor = Z_HUD_APPLE_COLOR_NORMAL;
    }
}

static void drawIcon(int X, int Y, ZSpriteId Sprite, unsigned Frame, ZColorId Color, int Alpha)
{
    z_graphics_stateColorSetId(Color);
    z_graphics_stateAlphaSet(Alpha);

    z_sprite_blitAlphaMask(Sprite, Frame, X, Y);
}

static void drawBar(int X, int Y, int Value, int Total, int Width, int Height, ZColorId ColorProg, ZColorId ColorBg, int Alpha)
{
    int progWidth = Width * Value / Total;
    int borderAlpha = Alpha >> 2;

    // Main bar

    z_graphics_stateAlphaSet(Alpha);
    z_graphics_stateColorSetId(ColorProg);

    z_draw_rectangleAlpha(X, Y, progWidth, Height);

    z_graphics_stateColorSetId(ColorBg);

    z_draw_rectangleAlpha(X + progWidth, Y, Width - progWidth, Height);

    // Glow border

    z_graphics_stateAlphaSet(borderAlpha);
    z_graphics_stateColorSetId(Value > 0 ? ColorProg : ColorBg);

    z_draw_rectangleAlpha(X - 1, Y, 1, Height);

    z_graphics_stateColorSetId(ColorProg);
    z_draw_rectangleAlpha(X, Y - 1, progWidth, 1);
    z_draw_rectangleAlpha(X, Y + Height, progWidth, 1);

    z_graphics_stateColorSetId(ColorBg);

    z_draw_rectangleAlpha(X + progWidth, Y - 1, Width - progWidth, 1);
    z_draw_rectangleAlpha(X + progWidth, Y + Height, Width - progWidth, 1);

    z_graphics_stateColorSetId(Value >= Total ? ColorProg : ColorBg);

    z_draw_rectangleAlpha(X + Width, Y, 1, Height);
}

static void drawNumber(int X, int Y, unsigned Number, int NumDigits, ZSpriteId Font)
{
    int charSize = z_sprite_sizeGetWidth(Font) + 1;

    X += charSize * (NumDigits - 1);

    z_graphics_stateAlphaSet(256);

    for(int d = NumDigits; d--; X -= charSize, Number /= 10) {
        z_sprite_blitAlphaMask(Font, Number % 10, X, Y);
    }
}

static void hudDrawApples(int X, int Y, const OSnake* Snake)
{
    drawIcon(X, Y, Z_SPRITE_APPLE_MASK, 0, g_hud.appleColor, 256);

    X += 10;
    Y += 2;

    drawBar(X,
            Y,
            o_snake_eatenNumGet(Snake),
            n_map_applesNumGet(),
            Z_SCREEN_W / 2 - 2 - X,
            4,
            g_hud.appleColor,
            Z_COLOR_BG_GREEN_03,
            192);
}

static void hudDrawLife(int X, int Y, const OSnake* Snake)
{
    drawIcon(X, Y, Z_SPRITE_ICON_HEART, 0, g_hud.lifeColor, 256);

    X += 9;
    Y += 1;

    drawBar(X,
            Y,
            o_snake_lifeGet(Snake),
            O_SNAKE_LIFE_MAX,
            Z_SCREEN_W - 2 - X,
            4,
            g_hud.lifeColor,
            Z_COLOR_BG_GREEN_03,
            192);
}

static void hudDrawScore(int X, int Y)
{
    unsigned score = n_game_scoreGet();
    unsigned hiscore = z_save_hiscoreGet(n_game_levelGet());
    ZColorId color = Z_COLOR_SNAKE_01 + (score > hiscore);

    z_graphics_stateColorSetId(Z_COLOR_SNAKE_01);

    drawNumber(X, Y, n_game_scoreGet(), 4, Z_SPRITE_FONT_LCDNUM);

    Y += 8;

    z_graphics_stateColorSetId(color);

    drawNumber(X, Y, z_math_maxu(score, hiscore), 5, Z_SPRITE_FONT_SMALLNUM);

    X += 20;

    z_graphics_stateAlphaSet(192);

    z_sprite_blitAlphaMask(Z_SPRITE_ICON_HI, 0, X, Y);
}

static void hudDrawLevel(int X, int Y)
{
    z_graphics_stateColorSetId(Z_COLOR_SNAKE_02);
    z_graphics_stateAlphaSet(192);

    z_sprite_blitAlphaMask(Z_SPRITE_ICON_LVL, 0, X, Y + 8);

    drawNumber(X + 3, Y, n_game_levelGet() + 1, 2, Z_SPRITE_FONT_LCDNUM);
}

void n_hud_draw(const OSnake* Snake)
{
    ZVectorInt shake = n_camera_shakeGet();

    hudDrawApples(1 + shake.x, 1 + shake.y, Snake);
    hudDrawLife(Z_SCREEN_W / 2 + 2 + shake.x, 2 - shake.y, Snake);
    hudDrawScore(2 - shake.x, Z_SCREEN_H - 15 + shake.y);
    hudDrawLevel(Z_SCREEN_W - 31 - shake.x, Z_SCREEN_H - 15 - shake.y);

    n_map_drawMinimap(Z_SCREEN_W - 18 + shake.x,
                      Z_SCREEN_H - 18 + shake.y,
                      o_snake_coordsGet(Snake));
}
