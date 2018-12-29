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

#include "obj_particle.h"

#include "util_camera.h"
#include "util_fps.h"
#include "util_graphics.h"
#include "util_list.h"
#include "util_pool.h"

struct OParticle {
    ZListNode particlesList;
    ZVectorFix coords;
    ZFix speed;
    unsigned angle;
    int alpha;
};

Z_POOL_DECLARE(OParticle, 64, g_pool);

void o_particle_setup(void)
{
    z_pool_register(Z_POOL_PARTICLE, g_pool);
}

OParticle* o_particle_new(ZFix X, ZFix Y)
{
    OParticle* p = z_pool_alloc(Z_POOL_PARTICLE);

    if(p != NULL) {
        z_list_clearNode(&p->particlesList);

        p->coords = (ZVectorFix){X, Y};
        p->speed = z_random_range(Z_FIX_ONE / 2, Z_FIX_ONE);
        p->angle = z_fix_angleWrap(z_random_intu(Z_ANGLES_NUM));
        p->alpha = 256;
    }

    return p;
}

void o_particle_free(OParticle* Particle)
{
    z_list_remove(&Particle->particlesList);
    z_pool_free(Z_POOL_PARTICLE, Particle);
}

bool o_particle_tick(OParticle* Particle)
{
    Particle->coords.x += z_fix_mul(
                            Particle->speed, z_fix_cos(Particle->angle));
    Particle->coords.y -= z_fix_mul(
                            Particle->speed, z_fix_sin(Particle->angle));

    Particle->alpha -= 16;

    return Particle->alpha <= 0;
}

void o_particle_draw(const OParticle* Particle)
{
    ZVectorInt screen = z_camera_coordsToScreen(Particle->coords);
    ZVectorInt shake = z_camera_shakeGet();

    screen.x += shake.x;
    screen.y += shake.y;

    if(screen.x >= 0 && screen.x < Z_SCREEN_W
        && screen.y >= 0 && screen.y < Z_SCREEN_H) {

        z_pixel_drawAlpha2(
            screen.x, screen.y, z_color_appleGet(), Particle->alpha);
    }
}
