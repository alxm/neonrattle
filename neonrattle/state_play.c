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

#include "state_play.h"

#include "obj_game.h"
#include "obj_map.h"
#include "state_menu.h"
#include "util_input.h"
#include "util_save.h"

void s_play_tick(void)
{
    OSnake* snake = n_game_snakeGet();

    #if Z_PLATFORM_META
        #define Z_EXTRA_LEFT z_button_pressGet(Z_BUTTON_A)
        #define Z_EXTRA_RIGHT z_button_pressGet(Z_BUTTON_B)
    #else
        #define Z_EXTRA_LEFT false
        #define Z_EXTRA_RIGHT false
    #endif

    if(z_button_pressGet(Z_BUTTON_LEFT) || Z_EXTRA_LEFT) {
        o_snake_turnLeft(snake);
    }

    if(z_button_pressGet(Z_BUTTON_RIGHT) || Z_EXTRA_RIGHT) {
        o_snake_turnRight(snake);
    }

    n_game_tick();

    if(o_snake_flagsTest(snake, O_SNAKE_FLAG_DEAD)) {
        z_state_set(Z_STATE_DIED);
    } else if(o_snake_eatenNumGet(snake) == n_map_applesNumGet()) {
        z_state_set(Z_STATE_END);
    }
}

void s_play_draw(void)
{
    n_game_draw();
}

void s_play_free(void)
{
    unsigned level = n_game_levelGet();

    z_save_hiscoreSet(level, n_game_scoreGet());
    z_save_commit();

    s_menu_select(level);
}
