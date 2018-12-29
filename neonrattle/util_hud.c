/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

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

#include "util_graphics.h"
#include "util_map.h"

void z_hud_draw(OSnake* Snake)
{
    #define BAR_X (2)
    #define BAR_Y (2)
    #define BAR_W (Z_SCREEN_W - BAR_X * 2)
    #define BAR_H (4)

    int width = BAR_W * o_snake_eatenNumGet(Snake) / z_map_applesNumGet();

    z_draw_rectangleAlpha(BAR_X, BAR_Y - 1, BAR_W, 1, Z_COLOR_APPLE_03, 96);

    z_draw_rectangleAlpha(BAR_X,
                          BAR_Y,
                          width,
                          BAR_H,
                          Z_COLOR_APPLE_05,
                          208);

    z_draw_rectangleAlpha(BAR_X + width,
                          BAR_Y,
                          BAR_W - width,
                          BAR_H,
                          Z_COLOR_APPLE_02,
                          128);

    z_draw_rectangleAlpha(BAR_X, BAR_Y + BAR_H, BAR_W, 1, Z_COLOR_APPLE_03, 96);
}
