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
#include "obj_map.h"
#include "util_hud.h"
#include "util_input.h"
#include "util_light.h"
#include "util_sound.h"
#include "util_swipe.h"

void s_start_init(void)
{
    o_apple_setup();
    o_snake_setup();

    o_game_levelNext();

    o_camera_reset();
    z_input_reset();
    z_light_reset();

    z_swipe_start(Z_SWIPE_SHOW);
}

void s_start_tick(void)
{
    OSnake* snake = o_game_snakeGet();

    if(z_button_pressGetAny()) {
        z_sfx_play(Z_SFX_PRESSED_A);
        z_state_set(Z_STATE_PLAY);
    }

    o_map_tick();
    o_snake_tickStart(snake);
    o_camera_tick(o_snake_coordsGet(snake));
    z_hud_tick(snake);
}

void s_start_draw(void)
{
    OSnake* snake = o_game_snakeGet();

    o_map_draw();
    o_snake_draw(snake);
    o_map_drawMinimap(o_snake_coordsGet(snake));
    z_hud_draw(snake);
}

void s_start_free(void)
{
    //
}
