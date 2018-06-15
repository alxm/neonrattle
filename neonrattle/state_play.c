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
#include "state_play.h"

#include "obj_snake.h"
#include "util_camera.h"
#include "util_effects.h"
#include "util_input.h"
#include "util_light.h"
#include "util_map.h"
#include "util_pool.h"

static ZSnake* g_snake;
static bool g_canMove;

void z_state_play_init(void)
{
    z_effects_init();
    z_light_reset();

    ZFix startX, startY;
    z_map_init(&startX, &startY);

    g_snake = z_snake_new(startX, startY);
    g_canMove = false;
}

void z_state_play_tick(void)
{
    z_map_tick();

    if(!g_canMove
        && (z_button_pressed(Z_BUTTON_A) || z_button_pressed(Z_BUTTON_B))) {

        g_canMove = true;
        z_sfx_play(Z_SFX_PRESSED_A);
    }

    if(g_canMove && z_snake_tick(g_snake)) {
        z_state_set(Z_STATE_DIED, false);
    }

    z_camera_tick(g_snake);
    z_effects_tick();
}

void z_state_play_draw(void)
{
    z_map_draw();
    z_effects_draw1();
    z_snake_draw(g_snake);
    z_effects_draw2();
}

void z_state_play_free(void)
{
    g_canMove = false;
}

ZSnake* z_state_play_getSnake(void)
{
    return g_snake;
}
