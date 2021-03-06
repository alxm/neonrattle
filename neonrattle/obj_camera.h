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

#include "util_fix.h"

extern void n_camera_new(ZVectorFix Coords);

extern void n_camera_tick(ZVectorFix Origin);

extern ZVectorFix n_camera_originGet(void);
extern ZVectorInt n_camera_coordsToScreen(ZVectorFix WorldCoords);
extern void n_camera_boundsGet(ZVectorInt* TileStart, ZVectorInt* TileEnd, ZVectorInt* GridStart, ZVectorInt* GridEnd, ZVectorInt* ScreenStart);

extern ZVectorInt n_camera_shakeGet(void);
extern void n_camera_shakeSet(unsigned Ms);
