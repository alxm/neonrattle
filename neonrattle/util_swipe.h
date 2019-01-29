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

typedef enum {
    Z_SWIPE_INVALID = -1,
    Z_SWIPE_FADE_HIDE,
    Z_SWIPE_FADE_SHOW,
    Z_SWIPE_LINES_HIDE,
    Z_SWIPE_LINES_SHOW,
    Z_SWIPE_NUM
} ZSwipeId;

extern void z_swipe_start(ZSwipeId Swipe);
extern void z_swipe_stop(void);
extern bool z_swipe_running(void);

extern void z_swipe_tick(void);
extern void z_swipe_draw(void);
