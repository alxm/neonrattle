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
#include "util_effects.h"

#include "obj_circle.h"
#include "obj_particle.h"
#include "util_list.h"
#include "util_pool.h"

static ZList g_circles;
static ZList g_particles;

void z_effects_particles(ZFix X, ZFix Y, unsigned Num)
{
    while(Num--) {
        ZParticle* p = z_particle_new(X, Y);

        if(p) {
            z_list_addLast(&g_particles, p);
        }
    }
}

void z_effects_circles(ZFix X, ZFix Y)
{
    ZCircle* c = z_circle_new(X, Y);

    if(c) {
        z_list_addLast(&g_circles, c);
    }
}

void z_effects_init(void)
{
    z_list_init(&g_circles, 0);
    z_list_init(&g_particles, 0);

    z_pool_reset(Z_POOL_CIRCLE);
    z_pool_reset(Z_POOL_PARTICLE);
}

void z_effects_tick(void)
{
    Z_LIST_ITERATE(&g_circles, ZCircle*, c) {
        if(z_circle_tick(c)) {
            z_circle_free(c);
        }
    }

    Z_LIST_ITERATE(&g_particles, ZParticle*, p) {
        if(z_particle_tick(p)) {
            z_particle_free(p);
        }
    }
}

void z_effects_draw1(void)
{
    Z_LIST_ITERATE(&g_circles, ZCircle*, c) {
        z_circle_draw(c);
    }
}

void z_effects_draw2(void)
{
    Z_LIST_ITERATE(&g_particles, ZParticle*, p) {
        z_particle_draw(p);
    }
}
