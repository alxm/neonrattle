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

#include "util_sound.h"

#include "generated/data_sfx_apple_eat.h"
#include "generated/data_sfx_hit_wall.h"
#include "generated/data_sfx_pressed_a.h"
#include "generated/data_sfx_swipe_hide.h"
#include "generated/data_sfx_swipe_show.h"

void z_sound_setup(void)
{
    z_sfx_load(Z_SFX_APPLE_EAT, apple_eat);
    z_sfx_load(Z_SFX_HIT_WALL, hit_wall);
    z_sfx_load(Z_SFX_PRESSED_A, pressed_a);
    z_sfx_load(Z_SFX_SWIPE_HIDE, swipe_hide);
    z_sfx_load(Z_SFX_SWIPE_SHOW, swipe_show);
}
