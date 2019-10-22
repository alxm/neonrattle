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

#include "util_save.h"

#define Z_SAVE_FILE_NAME F_CONFIG_PATH_STORAGE_PREFIX "neonrattle.sav"

typedef struct {
    uint32_t version;
    uint32_t lastUnlocked;
    uint32_t hiscores[Z_LEVELS_NUM];
} ZSaveFile;

ZSaveFile g_save;
bool g_dirty;

void z_save_setup(void)
{
    g_save.version = 1;

    z_file_readOnce(Z_SAVE_FILE_NAME, &g_save, sizeof(g_save));
}

void z_save_commit(void)
{
    if(g_dirty) {
        g_dirty = false;
        z_file_writeOnce(Z_SAVE_FILE_NAME, &g_save, sizeof(g_save));
    }
}

unsigned z_save_unlockedGet(void)
{
    return g_save.lastUnlocked;
}

void z_save_unlockedSet(unsigned Level)
{
    if(Level > g_save.lastUnlocked) {
        g_save.lastUnlocked = Level;
        g_dirty = true;
    }
}

unsigned z_save_hiscoreGet(unsigned Level)
{
    return g_save.hiscores[Level];
}

void z_save_hiscoreSet(unsigned Level, unsigned Score)
{
    if(Score > g_save.hiscores[Level]) {
        g_save.hiscores[Level] = Score;
        g_dirty = true;
    }
}
