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

#include "obj_map.h"
#include "util_graphics.h"

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

    z_draw_rectangleAlpha(Coords->x,
                          Coords->y - Height / 2,
                          progressWidth,
                          Height,
                          ProgressColor,
                          Alpha);

    z_draw_rectangleAlpha(Coords->x + progressWidth,
                          Coords->y - Height / 2,
                          Width - progressWidth,
                          Height,
                          BackgroundColor,
                          Alpha);

    Coords->x += Width + 2;
}

void z_hud_draw(OSnake* Snake)
{
    ZVectorInt pos = {1, 5};

    drawIcon(&pos, Z_SPRITE_APPLE_ALPHAMASK, 0, Z_COLOR_SNAKE_01, 256);
    drawBar(&pos,
            o_snake_eatenNumGet(Snake),
            o_map_applesNumGet(),
            32,
            4,
            Z_COLOR_SNAKE_01,
            Z_COLOR_BG_GREEN_03,
            192);

    drawIcon(&pos, Z_SPRITE_HEART, 0, Z_COLOR_SNAKE_01, 256);
    drawBar(&pos,
            60,
            100,
            20,
            4,
            Z_COLOR_SNAKE_01,
            Z_COLOR_BG_GREEN_03,
            192);
}
