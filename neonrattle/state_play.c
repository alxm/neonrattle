/*
    Copyright 2018, 2019 Alex Margarit <alex@alxm.org>

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

#include "state_play.h"

#include "obj_camera.h"
#include "obj_game.h"
#include "obj_map.h"
#include "util_effects.h"
#include "util_hud.h"
#include "util_swipe.h"

void s_play_init(void)
{
    //
}

void s_play_tick(void)
{
    OSnake* snake = o_game_snakeGet();

    o_map_tick();
    o_snake_tickPlay(snake);
    o_camera_tick(o_snake_coordsGet(snake));
    z_effects_tick();
    z_hud_tick(snake);

    if(!z_state_changed()) {
        if(o_snake_flagsTest(snake, O_SNAKE_FLAG_DEAD)) {
            z_state_set(Z_STATE_DIED);
        } else if(o_snake_eatenNumGet(snake) == o_map_applesNumGet()) {
            z_state_set(Z_STATE_START);
            z_swipe_start(Z_SWIPE_HIDE);
        }
    }
}

void s_play_draw(void)
{
    OSnake* snake = o_game_snakeGet();

    o_map_draw();
    z_effects_draw1();
    o_snake_draw(snake);
    z_effects_draw2();
    z_hud_draw(snake);
}

void s_play_free(void)
{
    //
}
