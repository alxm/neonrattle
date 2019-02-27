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

#pragma once

#include "platform.h"

#include "obj_apple.h"
#include "util_fix.h"
#include "util_list.h"

extern void n_map_new(unsigned Level, ZFix* StartX, ZFix* StartY);

extern void n_map_tick(void);
extern void n_map_draw(void);

extern void n_map_drawMinimap(int X, int Y, ZVectorFix SnakeCoords);

extern void n_map_visibleGet(ZVectorInt* TileStart, ZVectorInt* TileEnd, ZVectorInt* GridStart, ZVectorInt* GridEnd, ZVectorInt* ScreenStart);

extern ZList* n_map_applesListGet(int GridX, int GridY);
extern int n_map_applesNumGet(void);
extern void n_map_appleEat(OApple* Apple);
extern bool n_map_isWall(ZVectorInt Tile);
