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

#include "util_fix.h"

typedef struct ZParticle ZParticle;

extern void z_particle_setup(void);

extern ZParticle* z_particle_new(ZFix X, ZFix Y);
extern void z_particle_free(ZParticle* Particle);

extern bool z_particle_tick(ZParticle* Particle);
extern void z_particle_draw(const ZParticle* Particle);
