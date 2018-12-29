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

typedef enum {
    Z_LIGHT_INVALID = -1,
    Z_LIGHT_APPLE_EAT,
    Z_LIGHT_SNAKE_HIT_WALL,
    Z_LIGHT_NUM
} ZLightId;

extern void z_light_reset(void);
extern void z_light_tick(void);

extern void z_light_pulseSet(ZLightId Light);
extern void z_light_backgroundSet(ZColorId ColorId);
