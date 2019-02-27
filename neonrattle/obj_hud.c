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

#include "obj_game.h"
#include "obj_map.h"
#include "util_graphics.h"
#include "util_save.h"
#include "util_timer.h"

#define Z_HUD_APPLES_BLINK_DS 1
#define Z_HUD_LIFE_BLINK_DS 2

typedef struct {
    int lifeColorIndex;
} NHud;

static NHud g_hud;

void n_hud_tick(const OSnake* Snake)
{
    if(o_snake_flagsTest(Snake, O_SNAKE_FLAG_HURT)) {
        if(!z_timer_running(Z_TIMER_HUD_LIFE)) {
            z_timer_start(Z_TIMER_HUD_LIFE, Z_HUD_LIFE_BLINK_DS, true);
            g_hud.lifeColorIndex = 1;
        } else if(z_timer_expired(Z_TIMER_HUD_LIFE)) {
            g_hud.lifeColorIndex = !g_hud.lifeColorIndex;
        }
    } else {
        z_timer_stop(Z_TIMER_HUD_LIFE);
        g_hud.lifeColorIndex = 0;
    }

    if(o_snake_flagsTest(Snake, O_SNAKE_FLAG_ATE)) {
        if(!z_timer_running(Z_TIMER_HUD_APPLES)) {
            z_timer_start(Z_TIMER_HUD_APPLES, Z_HUD_APPLES_BLINK_DS, false);
        }
    }
}

static void drawIcon(ZVectorInt* Coords, ZSpriteId Sprite, unsigned Frame, ZColorId Color, int Alpha)
{
    ZVectorInt size = z_sprite_sizeGet(Sprite);

    z_graphics_stateColorSetId(Color);
    z_graphics_stateAlphaSet(Alpha);

    z_sprite_blitAlphaMask(
        Sprite, Coords->x, Coords->y - 2 + (size.y & 1), Frame);

    Coords->x += size.x + 2;
}

static void drawBar(ZVectorInt* Coords, int Value, int Total, int Width, int Height, ZColorId ProgressColor, ZColorId BackgroundColor, int Alpha)
{
    int progressWidth = Width * Value / Total;
    int borderAlpha = Alpha >> 2;

    z_graphics_stateAlphaSet(borderAlpha);
    z_graphics_stateColorSetId(Value > 0 ? ProgressColor : BackgroundColor);

    z_draw_rectangleAlpha(Coords->x - 1,
                          Coords->y,
                          1,
                          Height);

    z_graphics_stateColorSetId(ProgressColor);

    z_draw_rectangleAlpha(Coords->x,
                          Coords->y - 1,
                          progressWidth,
                          1);
    z_draw_rectangleAlpha(Coords->x,
                          Coords->y + Height,
                          progressWidth,
                          1);

    z_graphics_stateAlphaSet(Alpha);

    z_draw_rectangleAlpha(Coords->x,
                          Coords->y,
                          progressWidth,
                          Height);

    z_graphics_stateAlphaSet(borderAlpha);
    z_graphics_stateColorSetId(BackgroundColor);

    z_draw_rectangleAlpha(Coords->x + progressWidth,
                          Coords->y - 1,
                          Width - progressWidth,
                          1);

    z_draw_rectangleAlpha(Coords->x + progressWidth,
                          Coords->y + Height,
                          Width - progressWidth,
                          1);

    z_graphics_stateAlphaSet(Alpha);

    z_draw_rectangleAlpha(Coords->x + progressWidth,
                          Coords->y,
                          Width - progressWidth,
                          Height);

    z_graphics_stateColorSetId(
        Value >= Total ? ProgressColor : BackgroundColor);

    z_draw_rectangleAlpha(Coords->x + Width,
                          Coords->y,
                          1,
                          Height);

    Coords->x += Width + 2;
}

static void drawNumber(int X, int Y, unsigned Number, int NumDigits, ZSpriteId Font)
{
    int charSize = z_sprite_sizeGetWidth(Font) + 1;

    X += charSize * (NumDigits - 1);

    z_graphics_stateAlphaSet(256);

    for(int d = NumDigits; d--; X -= charSize, Number /= 10) {
        z_sprite_blitAlphaMask(Font, X, Y, Number % 10);
    }
}

void n_hud_draw(const OSnake* Snake)
{
    static const ZColorId aColors[] = {Z_COLOR_SNAKE_01, Z_COLOR_SNAKE_03};
    static const ZColorId lColors[] = {Z_COLOR_SNAKE_01, Z_COLOR_APPLE_03};

    ZColorId aColor = aColors[z_timer_running(Z_TIMER_HUD_APPLES)];
    ZColorId lColor = lColors[g_hud.lifeColorIndex];

    ZVectorInt pos = {1, 3};

    drawIcon(&pos, Z_SPRITE_APPLE_MASK, 0, aColor, 256);
    drawBar(&pos,
            o_snake_eatenNumGet(Snake),
            n_map_applesNumGet(),
            Z_SCREEN_W / 2 - pos.x,
            4,
            aColor,
            Z_COLOR_BG_GREEN_03,
            192);

    drawIcon(&pos, Z_SPRITE_ICON_HEART, 0, lColor, 256);
    drawBar(&pos,
            o_snake_lifeGet(Snake),
            O_SNAKE_LIFE_MAX,
            Z_SCREEN_W - pos.x - 2,
            4,
            lColor,
            Z_COLOR_BG_GREEN_03,
            192);

    unsigned score = n_game_scoreGet();
    unsigned hiscore = z_save_hiscoreGet(n_game_levelGet());
    ZColorId color = Z_COLOR_SNAKE_01 + (score > hiscore);

    z_graphics_stateColorSetId(Z_COLOR_SNAKE_01);

    drawNumber(2,
               Z_SCREEN_H - 15,
               n_game_scoreGet(),
               4,
               Z_SPRITE_FONT_LCDNUM);

    z_graphics_stateColorSetId(color);

    drawNumber(2,
               Z_SCREEN_H - 7,
               z_math_maxu(score, hiscore),
               5,
               Z_SPRITE_FONT_SMALLNUM);

    z_graphics_stateAlphaSet(192);
    z_sprite_blitAlphaMask(Z_SPRITE_ICON_HI, 22, Z_SCREEN_H - 7, 0);

    z_graphics_stateColorSetId(Z_COLOR_SNAKE_02);
    z_graphics_stateAlphaSet(192);

    z_sprite_blitAlphaMask(
        Z_SPRITE_ICON_LVL, Z_SCREEN_W - 31, Z_SCREEN_H - 7, 0);
    drawNumber(Z_SCREEN_W - 28,
               Z_SCREEN_H - 15,
               n_game_levelGet() + 1,
               2,
               Z_SPRITE_FONT_LCDNUM);
}
