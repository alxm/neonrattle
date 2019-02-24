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

#include "util_hud.h"

#include "obj_game.h"
#include "obj_map.h"
#include "util_graphics.h"
#include "util_save.h"
#include "util_timer.h"

#define Z_HUD_APPLES_BLINK_DS 1
#define Z_HUD_LIFE_BLINK_DS 2

static struct {
    int colorIndex;
} g_life;

void z_hud_tick(const OSnake* Snake)
{
    if(o_snake_flagsTest(Snake, O_SNAKE_FLAG_HURT)) {
        if(!z_timer_running(Z_TIMER_HUD_LIFE)) {
            z_timer_start(Z_TIMER_HUD_LIFE, Z_HUD_LIFE_BLINK_DS);
            g_life.colorIndex = 1;
        } else if(z_timer_expired(Z_TIMER_HUD_LIFE)) {
            g_life.colorIndex = !g_life.colorIndex;
        }
    } else {
        z_timer_stop(Z_TIMER_HUD_LIFE);
        g_life.colorIndex = 0;
    }

    if(o_snake_flagsTest(Snake, O_SNAKE_FLAG_ATE)) {
        if(!z_timer_running(Z_TIMER_HUD_APPLES)) {
            z_timer_start(Z_TIMER_HUD_APPLES, Z_HUD_APPLES_BLINK_DS);
        }
    } else if(z_timer_expired(Z_TIMER_HUD_APPLES)) {
        z_timer_stop(Z_TIMER_HUD_APPLES);
    }
}

static void drawIcon(ZVectorInt* Coords, ZSpriteId Sprite, unsigned Frame, ZColorId Color, int Alpha)
{
    int w = z_sprite_sizeGetWidth(Sprite);

    z_sprite_blitAlphaMask(
        Sprite, Coords->x + w / 2, Coords->y, Frame, Color, Alpha);

    Coords->x += w + 2;
}

static void drawBar(ZVectorInt* Coords, int Value, int Total, int Width, int Height, ZColorId ProgressColor, ZColorId BackgroundColor, int Alpha)
{
    int progressWidth = Width * Value / Total;
    int borderAlpha = Alpha >> 2;
    int heightHalf = Height >> 1;
    int heightHalf2 = Height - heightHalf;

    z_draw_rectangleAlpha(Coords->x - 1,
                          Coords->y - heightHalf,
                          1,
                          Height,
                          Value > 0 ? ProgressColor : BackgroundColor,
                          borderAlpha);
    z_draw_rectangleAlpha(Coords->x,
                          Coords->y - heightHalf - 1,
                          progressWidth,
                          1,
                          ProgressColor,
                          borderAlpha);
    z_draw_rectangleAlpha(Coords->x,
                          Coords->y - heightHalf,
                          progressWidth,
                          Height,
                          ProgressColor,
                          Alpha);
    z_draw_rectangleAlpha(Coords->x,
                          Coords->y + heightHalf2,
                          progressWidth,
                          1,
                          ProgressColor,
                          borderAlpha);

    z_draw_rectangleAlpha(Coords->x + progressWidth,
                          Coords->y - heightHalf - 1,
                          Width - progressWidth,
                          1,
                          BackgroundColor,
                          borderAlpha);
    z_draw_rectangleAlpha(Coords->x + progressWidth,
                          Coords->y - heightHalf,
                          Width - progressWidth,
                          Height,
                          BackgroundColor,
                          Alpha);
    z_draw_rectangleAlpha(Coords->x + progressWidth,
                          Coords->y + heightHalf2,
                          Width - progressWidth,
                          1,
                          BackgroundColor,
                          borderAlpha);
    z_draw_rectangleAlpha(Coords->x + Width,
                          Coords->y - heightHalf,
                          1,
                          Height,
                          Value >= Total ? ProgressColor : BackgroundColor,
                          borderAlpha);

    Coords->x += Width + 2;
}

static void drawNumber(int X, int Y, unsigned Number, int NumDigits, ZSpriteId Font, ZColorId Color)
{
    ZVectorInt charSize = z_sprite_sizeGet(Font);

    X += charSize.x / 2 + (charSize.x + 1) * (NumDigits - 1);
    Y += charSize.y / 2;

    for(int d = NumDigits; d--; X -= charSize.x + 1, Number /= 10) {
        z_sprite_blitAlphaMask(Font, X, Y, Number % 10, Color, 256);
    }
}

void z_hud_draw(const OSnake* Snake)
{
    static const ZColorId aColors[] = {Z_COLOR_SNAKE_01, Z_COLOR_BG_GREEN_04};
    static const ZColorId lColors[] = {Z_COLOR_SNAKE_01, Z_COLOR_APPLE_03};

    ZColorId aColor = aColors[z_timer_running(Z_TIMER_HUD_APPLES)];
    ZColorId lColor = lColors[g_life.colorIndex];

    ZVectorInt pos = {1, 5};

    drawIcon(&pos, Z_SPRITE_APPLE_MASK, 0, aColor, 256);
    drawBar(&pos,
            o_snake_eatenNumGet(Snake),
            o_map_applesNumGet(),
            28,
            4,
            aColor,
            Z_COLOR_BG_GREEN_03,
            192);

    drawIcon(&pos, Z_SPRITE_ICON_HEART, 0, lColor, 256);
    drawBar(&pos,
            o_snake_lifeGet(Snake),
            O_SNAKE_LIFE_MAX,
            28,
            4,
            lColor,
            Z_COLOR_BG_GREEN_03,
            192);

    unsigned score = o_game_scoreGet();
    unsigned hiscore = z_save_hiscoreGet(o_game_levelGet());
    ZColorId color = Z_COLOR_SNAKE_01 + (score > hiscore);

    drawNumber(2,
               Z_SCREEN_H - 15,
               o_game_scoreGet(),
               4,
               Z_SPRITE_FONT_LCDNUM,
               Z_COLOR_SNAKE_01);
    drawNumber(2,
               Z_SCREEN_H - 7,
               z_math_maxu(score, hiscore),
               5,
               Z_SPRITE_FONT_SMALLNUM,
               color);
    z_sprite_blitAlphaMask(
        Z_SPRITE_ICON_HI, 26, Z_SCREEN_H - 5, 0, color, 192);

    z_sprite_blitAlphaMask(
        Z_SPRITE_ICON_LVL, 55, Z_SCREEN_H - 5, 0, Z_COLOR_SNAKE_02, 192);
    drawNumber(52,
               Z_SCREEN_H - 15,
               o_game_levelGet() + 1,
               2,
               Z_SPRITE_FONT_LCDNUM,
               Z_COLOR_SNAKE_02);
}
