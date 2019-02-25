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

#include "state_start.h"

#include "obj_camera.h"
#include "obj_game.h"
#include "util_input.h"
#include "util_light.h"
#include "util_sound.h"
#include "util_swipe.h"

void s_start_init(void)
{
    z_input_reset();
    z_light_reset();

    n_camera_new();

    z_swipe_start(Z_SWIPE_LINES_SHOW);
}

void s_start_tick(void)
{
    n_game_tick();

    if(z_button_pressGetAny()) {
        z_sfx_play(Z_SFX_PRESSED_A);
        z_light_pulseSet(Z_LIGHT_GAME_START);
        z_state_set(Z_STATE_PLAY);
    }
}

void s_start_draw(void)
{
    n_game_draw();
}
