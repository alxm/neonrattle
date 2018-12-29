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

#include "platform.h"
#include "util_screen.h"

#include "util_input.h"
#include "util_timer.h"

ZVectorInt g_shake;
static uint8_t g_shakeFrames;

void z_screen_reset(void)
{
    g_shake = (ZVectorInt){0, 0};
    g_shakeFrames = 0;
}

void z_screen_tick(void)
{
    if(g_shakeFrames) {
        g_shakeFrames--;
        g_shake = (ZVectorInt){-1 + z_random_int(3), -1 + z_random_int(3)};
    } else {
        g_shake = (ZVectorInt){0, 0};
    }
}

ZVectorInt z_screen_shakeGet(void)
{
    return g_shake;
}

void z_screen_shakeSet(uint8_t Ds)
{
    g_shakeFrames = z_timer_dsToTicks(Ds);
}
