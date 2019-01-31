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

#pragma once

#include "platform.h"

#define Z_LEVELS_NUM 32

extern void z_save_setup(void);

extern void z_save_commit(void);

extern unsigned z_save_unlockedGet(void);
extern void z_save_unlockedSet(unsigned Level);

extern unsigned z_save_hiscoreGet(unsigned Level);
extern void z_save_hiscoreSet(unsigned Level, unsigned Score);