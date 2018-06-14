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
#include "state.h"

#include "state_died.h"
#include "state_intro.h"
#include "state_play.h"
#include "obj_apple.h"
#include "obj_circle.h"
#include "obj_particle.h"
#include "obj_snake.h"
#include "util_input.h"
#include "util_light.h"
#include "util_map.h"
#include "util_screen.h"
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
        z_state_died_init,
        z_state_died_tick,
        z_state_died_draw,
        z_state_died_free,
        Z_SWIPE_INVALID,
        Z_SWIPE_HIDE,
    },
    [Z_STATE_INTRO] = {
        z_state_intro_init,
        z_state_intro_tick,
        z_state_intro_draw,
        z_state_intro_free,
        Z_SWIPE_INVALID,
        Z_SWIPE_HIDE,
    },
    [Z_STATE_PLAY] = {
        z_state_play_init,
        z_state_play_tick,
        z_state_play_draw,
        NULL,
        Z_SWIPE_SHOW,
        Z_SWIPE_INVALID,
    },
};

static struct {
    ZStateId current;
    ZStateId next;
} g_state;

static struct {
    ZSwipeId swipeOut;
    ZSwipeId swipeIn;
} g_swipe;

void z_state_setup(void)
{
    g_state.current = Z_STATE_INVALID;
    g_state.next = Z_STATE_INVALID;

    g_swipe.swipeOut = Z_SWIPE_INVALID;
    g_swipe.swipeIn = Z_SWIPE_INVALID;

    z_graphics_setup();
    z_light_reset();
    z_map_setup();
    z_screen_reset();
    z_sound_setup();

    z_apple_setup();
    z_circle_setup();
    z_particle_setup();
    z_snake_setup();

    #if Z_DEBUG_STATS
        z_state_set(Z_STATE_PLAY, false);
    #else
        z_state_set(Z_STATE_INTRO, false);
    #endif
}

void z_state_tick(void)
{
    z_screen_tick();
    z_swipe_tick();
    z_timer_tick();
    z_light_tick();

    if(g_state.next != Z_STATE_INVALID && g_swipe.swipeOut == Z_SWIPE_INVALID) {
        if(g_state.current != Z_STATE_INVALID
            && g_states[g_state.current].free) {

            g_states[g_state.current].free();
        }

        g_state.current = g_state.next;
        g_state.next = Z_STATE_INVALID;

        if(g_states[g_state.current].init) {
            g_states[g_state.current].init();
        }

        z_swipe_init(&g_swipe.swipeIn);
        z_input_reset();
    }

    if(g_states[g_state.current].tick) {
        g_states[g_state.current].tick();
    }
}

void z_state_draw(void)
{
    if(g_states[g_state.current].draw) {
        g_states[g_state.current].draw();
    }

    z_swipe_draw();
}

void z_state_set(ZStateId NewState, bool Transition)
{
    if(g_state.next != Z_STATE_INVALID) {
        return;
    }

    g_state.next = NewState;

    if(Transition) {
        g_swipe.swipeOut = g_states[g_state.current].outro;
        g_swipe.swipeIn = g_states[g_state.next].intro;

        z_swipe_init(&g_swipe.swipeOut);
    }
}

bool z_state_changed(void)
{
    return g_state.next != Z_STATE_INVALID;
}
