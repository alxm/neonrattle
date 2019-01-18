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

#include "util_hud.h"

#include "obj_game.h"
#include "obj_map.h"
#include "util_graphics.h"
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
    int w = z_sprite_widthGet(Sprite);

    z_sprite_blitAlphaMask(
        Sprite, Coords->x + w / 2, Coords->y, Frame, Color, Alpha);

    Coords->x += w + 2;
}

static void drawBar(ZVectorInt* Coords, int Value, int Total, int Width, int Height, ZColorId ProgressColor, ZColorId BackgroundColor, int Alpha)
{
    int progressWidth = Width * Value / Total;
    int borderAlpha = Alpha / 4;

    z_draw_rectangleAlpha(Coords->x - 1,
                          Coords->y - Height / 2,
                          1,
                          Height,
                          Value > 0 ? ProgressColor : BackgroundColor,
                          borderAlpha);
    z_draw_rectangleAlpha(Coords->x,
                          Coords->y - Height / 2 - 1,
                          progressWidth,
                          1,
                          ProgressColor,
                          borderAlpha);
    z_draw_rectangleAlpha(Coords->x,
                          Coords->y - Height / 2,
                          progressWidth,
                          Height,
                          ProgressColor,
                          Alpha);
    z_draw_rectangleAlpha(Coords->x,
                          Coords->y + Height / 2,
                          progressWidth,
                          1,
                          ProgressColor,
                          borderAlpha);

    z_draw_rectangleAlpha(Coords->x + progressWidth,
                          Coords->y - Height / 2 - 1,
                          Width - progressWidth,
                          1,
                          BackgroundColor,
                          borderAlpha);
    z_draw_rectangleAlpha(Coords->x + progressWidth,
                          Coords->y - Height / 2,
                          Width - progressWidth,
                          Height,
                          BackgroundColor,
                          Alpha);
    z_draw_rectangleAlpha(Coords->x + progressWidth,
                          Coords->y + Height / 2,
                          Width - progressWidth,
                          1,
                          BackgroundColor,
                          borderAlpha);
    z_draw_rectangleAlpha(Coords->x + Width,
                          Coords->y - Height / 2,
                          1,
                          Height,
                          Value >= Total ? ProgressColor : BackgroundColor,
                          borderAlpha);

    Coords->x += Width + 2;
}

static void drawNumber(int X, int Y, unsigned Number, int NumDigits)
{
    int charWidth = z_sprite_widthGet(Z_SPRITE_FONT_LCDNUM);
    int charHeight = z_sprite_heightGet(Z_SPRITE_FONT_LCDNUM);

    X -= charWidth / 2;
    Y += charHeight / 2;

    for(int d = NumDigits; d--; X -= charWidth + 1, Number /= 10) {
        z_sprite_blitAlphaMask(
            Z_SPRITE_FONT_LCDNUM, X, Y, Number % 10, Z_COLOR_SNAKE_01, 256);
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
            22,
            4,
            aColor,
            Z_COLOR_BG_GREEN_03,
            192);

    drawIcon(&pos, Z_SPRITE_ICON_HEART, 0, lColor, 256);
    drawBar(&pos,
            o_snake_lifeGet(Snake),
            O_SNAKE_LIFE_MAX,
            22,
            4,
            lColor,
            Z_COLOR_BG_GREEN_03,
            192);

    drawNumber(Z_SCREEN_W - 2, 2, o_game_levelGet(), 2);
}
