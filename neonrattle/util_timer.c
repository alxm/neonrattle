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

#include "util_timer.h"

typedef enum {
    Z_EXPIRED = Z_FLAG_BIT(0),
    Z_REPEAT = Z_FLAG_BIT(1),
    Z_RUNNING = Z_FLAG_BIT(2),
} ATimerFlags;

typedef struct {
    unsigned base;
    unsigned period;
    unsigned flags;
} ZTimer;

static ZTimer g_timers[Z_TIMER_NUM];

void z_timer_tick(void)
{
    unsigned now = z_fps_ticksGet();

    for(int t = 0; t < Z_TIMER_NUM; t++) {
        ZTimer* timer = &g_timers[t];

        if(!Z_FLAG_TEST_ANY(timer->flags, Z_RUNNING)) {
            Z_FLAG_CLEAR(timer->flags, Z_EXPIRED);
            continue;
        }

        if(now - timer->base >= timer->period) {
            Z_FLAG_SET(timer->flags, Z_EXPIRED);

            if(Z_FLAG_TEST_ANY(timer->flags, Z_REPEAT)) {
                timer->base = now;
            } else {
                Z_FLAG_CLEAR(timer->flags, Z_RUNNING);
            }
        } else {
            Z_FLAG_CLEAR(timer->flags, Z_EXPIRED);
        }
    }
}

void z_timer_start(ZTimerId Timer, unsigned Ms, bool Repeat)
{
    ZTimer* timer = &g_timers[Timer];

    timer->base = z_fps_ticksGet();
    timer->period = z_timer_msToTicks(Ms);
    timer->flags = Z_RUNNING;

    if(Ms == 0) {
        Z_FLAG_SET(timer->flags, Z_EXPIRED);
    }

    if(Repeat) {
        Z_FLAG_SET(timer->flags, Z_REPEAT);
    }
}

void z_timer_stop(ZTimerId Timer)
{
    ZTimer* timer = &g_timers[Timer];

    Z_FLAG_CLEAR(timer->flags, Z_RUNNING | Z_EXPIRED);
}

bool z_timer_isRunning(ZTimerId Timer)
{
    return Z_FLAG_TEST_ANY(g_timers[Timer].flags, Z_RUNNING);
}

bool z_timer_isExpired(ZTimerId Timer)
{
    return Z_FLAG_TEST_ANY(g_timers[Timer].flags, Z_EXPIRED);
}
