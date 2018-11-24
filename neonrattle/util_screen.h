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

typedef struct {
    int xShake, yShake;
} ZScreen;

extern ZScreen z_screen;

extern void z_screen_reset(void);
extern void z_screen_tick(void);

extern void z_screen_shake(uint8_t Ds);

static inline int z_screen_getXShake(void)
{
    return -1 + z_screen.xShake;
}

static inline int z_screen_getYShake(void)
{
    return -1 + z_screen.yShake;
}
