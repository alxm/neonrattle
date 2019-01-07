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

#include "state_start.h"

#include "obj_apple.h"
#include "obj_map.h"
#include "obj_snake.h"
#include "util_camera.h"
#include "util_effects.h"
#include "util_hud.h"
#include "util_input.h"
#include "util_light.h"
#include "util_sound.h"

static OSnake* g_snake;

void s_start_init(void)
{
    o_apple_setup();
    o_snake_setup();

    z_camera_reset();
    z_effects_init();
    z_input_reset();
    z_light_reset();

    ZFix startX, startY;
    o_map_init(0, &startX, &startY);

    g_snake = o_snake_new(startX, startY);
    z_state_contextSet(g_snake);
}

void s_start_tick(void)
{
    if(z_button_pressGetAny()) {
        z_sfx_play(Z_SFX_PRESSED_A);
        z_state_set(Z_STATE_PLAY);
    }

    o_map_tick();
    o_snake_tickStart(g_snake);
    z_camera_tick(o_snake_coordsGet(g_snake));
    z_effects_tick();
    z_hud_tick(g_snake);
}

void s_start_draw(void)
{
    o_map_draw();
    z_effects_draw1();
    o_snake_draw(g_snake);
    z_effects_draw2();
    z_hud_draw(g_snake);
}

void s_start_free(void)
{
    //
}
