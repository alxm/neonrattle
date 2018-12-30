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

#pragma once

#include "platform.h"

Z_EXTERN_C_START

typedef enum {
    Z_STATE_INVALID = -1,
    Z_STATE_DIED,
    Z_STATE_INTRO,
    Z_STATE_PLAY,
    Z_STATE_START,
    Z_STATE_NUM
} ZStateId;

typedef void (ZStateInit)(void);
typedef void (ZStateTick)(void);
typedef void (ZStateDraw)(void);
typedef void (ZStateFree)(void);

extern void z_state_setup(void);

extern void z_state_tick(void);
extern void z_state_draw(void);

extern void z_state_set(ZStateId NewState);
extern bool z_state_changed(void);

extern void* z_state_contextGet(void);
extern void z_state_contextSet(void* Context);

Z_EXTERN_C_END
