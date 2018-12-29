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
#include "obj_particle.h"

#include "util_camera.h"
#include "util_fps.h"
#include "util_list.h"
#include "util_pool.h"
#include "util_screen.h"

struct ZParticle {
    ZListNode particlesList;
    ZFix x, y;
    ZFix speed;
    unsigned angle;
    int alpha;
};

Z_POOL_DECLARE(ZParticle, 64, g_pool);

void z_particle_setup(void)
{
    z_pool_register(Z_POOL_PARTICLE, g_pool);
}

ZParticle* z_particle_new(ZFix X, ZFix Y)
{
    ZParticle* p = z_pool_alloc(Z_POOL_PARTICLE);

    if(p != NULL) {
        z_list_clearNode(&p->particlesList);

        p->x = X;
        p->y = Y;
        p->speed = z_random_range(Z_FIX_ONE / 2, Z_FIX_ONE);
        p->angle = z_fix_angleWrap(z_random_intu(Z_ANGLES_NUM));
        p->alpha = 256;
    }

    return p;
}

void z_particle_free(ZParticle* Particle)
{
    z_list_remove(&Particle->particlesList);
    z_pool_free(Z_POOL_PARTICLE, Particle);
}

bool z_particle_tick(ZParticle* Particle)
{
    Particle->x += z_fix_mul(Particle->speed, z_fix_cos(Particle->angle));
    Particle->y -= z_fix_mul(Particle->speed, z_fix_sin(Particle->angle));

    Particle->alpha -= 16;

    return Particle->alpha <= 0;
}

void z_particle_draw(const ZParticle* Particle)
{
    int x, y;
    z_camera_coordsToScreen(Particle->x, Particle->y, &x, &y);

    ZVectorInt shake = z_screen_shakeGet();

    x += shake.x;
    y += shake.y;

    if(x >= 0 && x < Z_SCREEN_W && y >= 0 && y < Z_SCREEN_H) {
        z_pixel_drawAlpha2(x, y, z_color_getRandomApple(), Particle->alpha);
    }
}
