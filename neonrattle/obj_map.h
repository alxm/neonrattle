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

#pragma once

#include "platform.h"

#include "obj_apple.h"
#include "util_fix.h"
#include "util_list.h"

extern void o_map_init(unsigned Level, ZFix* StartX, ZFix* StartY);
extern void o_map_tick(void);
extern void o_map_draw(void);
extern void o_map_drawMinimap(ZVectorFix PlayerSnake);

extern void o_map_visibleGet(ZVectorInt* TileStart, ZVectorInt* TileEnd, ZVectorInt* GridStart, ZVectorInt* GridEnd, ZVectorInt* ScreenStart);

extern ZList* o_map_applesListGet(int GridX, int GridY);
extern int o_map_applesNumGet(void);
extern int o_map_appleEat(OApple* Apple);
extern bool o_map_isWall(ZVectorInt Tile);
