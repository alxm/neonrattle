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

#include "util_state.h"

#include "obj_apple.h"
#include "obj_circle.h"
#include "obj_map.h"
#include "obj_particle.h"
#include "obj_snake.h"
#include "state_died.h"
#include "state_intro.h"
#include "state_play.h"
#include "util_camera.h"
#include "util_input.h"
#include "util_light.h"
#include "util_sound.h"
#include "util_swipe.h"
#include "util_timer.h"

typedef struct {
    ZStateInit* init;
    ZStateTick* tick;
    ZStateDraw* draw;
    ZStateFree* free;
    ZSwipeId intro;
    ZSwipeId outro;
} ZState;

static const ZState g_states[Z_STATE_NUM] = {
    [Z_STATE_DIED] = {
        s_died_init,
        s_died_tick,
        s_died_draw,
        s_died_free,
        Z_SWIPE_INVALID,
        Z_SWIPE_HIDE,
    },
    [Z_STATE_INTRO] = {
        s_intro_init,
        s_intro_tick,
        s_intro_draw,
        s_intro_free,
        Z_SWIPE_INVALID,
        Z_SWIPE_HIDE,
    },
    [Z_STATE_PLAY] = {
        s_play_init,
        s_play_tick,
        s_play_draw,
        s_play_free,
        Z_SWIPE_SHOW,
        Z_SWIPE_INVALID,
    },
};

static struct {
    ZStateId current;
    ZStateId next;
} g_state = {
    Z_STATE_INVALID,
    Z_STATE_INVALID
};

void z_state_setup(void)
{
    z_graphics_setup();
    o_map_setup();
    z_sound_setup();

    o_apple_setup();
    o_circle_setup();
    o_particle_setup();
    o_snake_setup();

    z_camera_reset();
    z_light_reset();

    #if Z_DEBUG_STATS
        z_state_set(Z_STATE_PLAY);
    #else
        z_state_set(Z_STATE_INTRO);
    #endif
}

static void checkNewState(void)
{
    if(g_state.next == Z_STATE_INVALID || z_swipe_running()) {
        return;
    }

    if(g_state.current != Z_STATE_INVALID && g_states[g_state.current].free) {
        g_states[g_state.current].free();
    }

    g_state.current = g_state.next;
    g_state.next = Z_STATE_INVALID;

    if(g_states[g_state.current].init) {
        g_states[g_state.current].init();
    }

    z_swipe_start(g_states[g_state.current].intro);
    z_input_reset();
}

void z_state_tick(void)
{
    z_swipe_tick();
    z_timer_tick();
    z_light_tick();

    checkNewState();

    if(g_states[g_state.current].tick) {
        g_states[g_state.current].tick();

        checkNewState();
    }
}

void z_state_draw(void)
{
    if(g_states[g_state.current].draw) {
        g_states[g_state.current].draw();
    }

    z_swipe_draw();
}

void z_state_set(ZStateId NewState)
{
    if(g_state.next != Z_STATE_INVALID) {
        return;
    }

    g_state.next = NewState;

    z_swipe_stop();

    if(g_state.current != Z_STATE_INVALID) {
        z_swipe_start(g_states[g_state.current].outro);
    }
}

bool z_state_changed(void)
{
    return g_state.next != Z_STATE_INVALID;
}
