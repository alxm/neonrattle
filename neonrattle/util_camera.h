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

#pragma once

#include "platform.h"

#include "obj_snake.h"
#include "util_fix.h"

extern void z_camera_reset(void);
extern void z_camera_tick(const OSnake* Snake);

extern ZVectorFix z_camera_originGet(void);
extern ZVectorInt z_camera_coordsToScreen(ZVectorFix WorldCoords);

extern ZVectorInt z_camera_shakeGet(void);
extern void z_camera_shakeSet(uint8_t Ds);
