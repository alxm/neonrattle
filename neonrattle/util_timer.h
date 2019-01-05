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

#include "util_fps.h"

typedef enum {
    Z_TIMER_INVALID = -1,
    Z_TIMER_G1,
    Z_TIMER_HUD_APPLES,
    Z_TIMER_HUD_LIFE,
    Z_TIMER_NUM
} ZTimerId;

extern void z_timer_tick(void);

extern void z_timer_start(ZTimerId Timer, uint8_t Ds);
extern void z_timer_stop(ZTimerId Timer);
extern void z_timer_restart(ZTimerId Timer);
extern bool z_timer_running(ZTimerId Timer);
extern bool z_timer_expired(ZTimerId Timer);

static inline uint8_t z_timer_dsToTicks(uint8_t Ds)
{
    return (uint8_t)(Z_FPS * Ds / 10);
}
