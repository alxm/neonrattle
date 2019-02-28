/*
    Copyright 2018-2019 Alex Margarit <alex@alxm.org>
    This file is part of Neonrattle, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "state_died.h"

#include "obj_camera.h"
#include "obj_game.h"
#include "util_sound.h"
#include "util_swipe.h"
#include "util_timer.h"

void s_died_init(void)
{
    z_timer_start(Z_TIMER_G1, 1200, false);
    n_camera_shakeSet(300);
    z_sfx_play(Z_SFX_END);
}

void s_died_tick(void)
{
    n_game_tick();

    if(z_timer_isExpired(Z_TIMER_G1)) {
        z_state_set(Z_STATE_MENU);
        z_swipe_start(Z_SWIPE_LINES_HIDE);
    }
}

void s_died_draw(void)
{
    n_game_draw();
}
