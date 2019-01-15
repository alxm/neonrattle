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

#include "util_input.h"

typedef struct {
    bool pressed;
    bool waitForRelease;
} ZButton;

static ZButton g_buttons[Z_BUTTON_NUM];

void z_input_reset(void)
{
    for(int b = 0; b < Z_BUTTON_NUM; b++) {
        z_button_pressClear(b);
    }
}

void z_input_tick(void)
{
    for(int b = 0; b < Z_BUTTON_NUM; b++) {
        bool pressed = z_platform_buttonPressGet(b);

        if(g_buttons[b].waitForRelease) {
            if(!pressed) {
                g_buttons[b].waitForRelease = false;
            }
        } else {
            g_buttons[b].pressed = pressed;
        }
    }
}

bool z_button_pressGet(ZButtonId Button)
{
    return g_buttons[Button].pressed;
}

bool z_button_pressGetOnce(ZButtonId Button)
{
    if(g_buttons[Button].pressed) {
        z_button_pressClear(Button);

        return true;
    }

    return false;
}

bool z_button_pressGetAny(void)
{
    for(int b = 0; b < Z_BUTTON_NUM; b++) {
        if(g_buttons[b].pressed) {
            return true;
        }
    }

    return false;
}

void z_button_pressClear(ZButtonId Button)
{
    g_buttons[Button].pressed = false;
    g_buttons[Button].waitForRelease = true;
}
