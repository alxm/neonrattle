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

#include "platform.h"
#include "obj_particle.h"

#include "util_fps.h"
#include "util_screen.h"

struct ZParticle {
    ZPoolObjHeader poolObject;
    ZFix x, y;
    unsigned angle;
    unsigned splitNum;
    int ticks;
};

Z_POOL_DECLARE(ZParticle, 24, g_pool);

void z_particle_setup(void)
{
    z_pool_register(Z_POOL_PARTICLE, g_pool);
}

void z_particle_init(ZParticle* Particle, ZFix X, ZFix Y)
{
    Particle->x = X;
    Particle->y = Y;
    Particle->angle = z_fix_wrapAngleInt(z_random_uint(Z_ANGLES_NUM));
    Particle->splitNum = u2(z_random_uint(4));
    Particle->ticks = 0;
}

bool z_particle_tick(ZPoolObjHeader* Particle, void* Context)
{
    Z_UNUSED(Context);

    ZParticle* particle = (ZParticle*)Particle;

    particle->ticks++;

    return particle->ticks < Z_FPS / 4
        || (particle->ticks < Z_FPS && z_random_int(4) != 0);
}

void z_particle_draw(ZPoolObjHeader* Particle)
{
    ZParticle* particle = (ZParticle*)Particle;

    unsigned splitNum = 1 + particle->splitNum;
    unsigned angle = particle->angle;

    static const unsigned incs[4] = {
        Z_INT_DEG_112, Z_INT_DEG_090, Z_INT_DEG_067, Z_INT_DEG_090
    };

    for(unsigned i = 0; i < splitNum; i++) {
        int x =
            z_fix_toInt(particle->x + z_fix_cos(angle) * particle->ticks)
                + z_screen_getXShake();
        int y =
            z_fix_toInt(particle->y - z_fix_sin(angle) * particle->ticks)
                + z_screen_getYShake();

        z_draw_pixel(x,
                     y,
                     z_fps_getCounter() & 2 ? Z_COLOR_RED : Z_COLOR_YELLOW);

        angle = z_fix_wrapAngleInt(angle + incs[i]);
    }
}
