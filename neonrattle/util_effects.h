/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Neon Rattle is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Neon Rattle is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Neon Rattle.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "util_fix.h"

extern void z_effects_particles(ZFix X, ZFix Y, unsigned Num);
extern void z_effects_circles(ZFix X, ZFix Y);

extern void z_effects_init(void);
extern void z_effects_tick(void);
extern void z_effects_draw1(void);
extern void z_effects_draw2(void);
