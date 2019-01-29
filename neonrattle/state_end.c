/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
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

#include "state_end.h"

#include "obj_camera.h"
#include "obj_game.h"
#include "obj_map.h"
#include "state_menu.h"
#include "util_hud.h"
#include "util_light.h"
#include "util_sound.h"
#include "util_swipe.h"
#include "util_timer.h"

void s_end_init(void)
{
    z_timer_start(Z_TIMER_G1, 8);
    z_sfx_play(Z_SFX_FINISHED);
    z_light_pulseSet(Z_LIGHT_LEVEL_COMPLETE);
}

void s_end_tick(void)
{
    OSnake* snake = o_game_snakeGet();

    o_map_tick();
    o_snake_tickEnd(snake);
    o_camera_tick(o_snake_coordsGet(snake));
    z_hud_tick(snake);

    if(z_timer_expired(Z_TIMER_G1)) {
        z_timer_stop(Z_TIMER_G1);

        z_state_set(Z_STATE_START);
        z_swipe_start(Z_SWIPE_LINES_HIDE);
        s_menu_selectNext();
    }
}

void s_end_draw(void)
{
    OSnake* snake = o_game_snakeGet();

    o_map_draw();
    o_snake_draw(snake);
    o_map_drawMinimap(o_snake_coordsGet(snake));
    z_hud_draw(snake);
}

void s_end_free(void)
{
    //
}
