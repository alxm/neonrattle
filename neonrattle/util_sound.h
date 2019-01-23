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

#pragma once

typedef enum {
    Z_SFX_INVALID = -1,
    Z_SFX_APPLE_EAT,
    Z_SFX_END,
    Z_SFX_FINISHED,
    Z_SFX_HIT_WALL,
    Z_SFX_PRESSED_A,
    Z_SFX_MENU_BROWSE,
    Z_SFX_MENU_REJECT,
    Z_SFX_MENU_SELECT,
    Z_SFX_SWIPE_HIDE,
    Z_SFX_SWIPE_SHOW,
    Z_SFX_NUM
} ZSfxId;

#include "platform.h"

Z_EXTERN_C_START

extern void z_sound_setup(void);

extern void z_sfx_play(ZSfxId Sfx);

Z_EXTERN_C_END
