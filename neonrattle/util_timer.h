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

#include "util_fps.h"

typedef enum {
    Z_TIMER_INVALID = -1,
    Z_TIMER_CAMERA_SHAKE,
    Z_TIMER_G1,
    Z_TIMER_HUD_APPLES,
    Z_TIMER_HUD_LIFE,
    Z_TIMER_HUD_LIFE_2,
    Z_TIMER_NUM
} ZTimerId;

extern void z_timer_tick(void);

extern void z_timer_start(ZTimerId Timer, unsigned Ms, bool Repeat);
extern void z_timer_stop(ZTimerId Timer);
extern bool z_timer_running(ZTimerId Timer);
extern bool z_timer_expired(ZTimerId Timer);

static inline unsigned z_timer_msToTicks(unsigned Ms)
{
    return (Z_FPS * Ms + 500) / 1000;
}
