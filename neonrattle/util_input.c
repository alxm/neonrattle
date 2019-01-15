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

#include "util_fps.h"
#include "util_timer.h"

typedef struct {
    uint8_t pressed;
    uint8_t waitForRelease;
    uint16_t lastFramePressed;
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
    uint16_t nextFrame = (uint16_t)(z_fps_ticksGet() + 1);

    for(int b = 0; b < Z_BUTTON_NUM; b++) {
        bool pressed = z_platform_buttonPressGet(b);

        if(g_buttons[b].waitForRelease) {
            if(!pressed) {
                g_buttons[b].waitForRelease = false;
            }
        } else {
            if(pressed && !g_buttons[b].pressed) {
                g_buttons[b].lastFramePressed = nextFrame;
            }

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

bool z_button_pressGetDelay(ZButtonId Button, uint8_t Ds)
{
    uint16_t now = z_fps_ticksGet();
    uint16_t ticksDiff = (uint16_t)(now - g_buttons[Button].lastFramePressed);

    if(g_buttons[Button].pressed && ticksDiff >= z_timer_dsToTicks(Ds)) {
        g_buttons[Button].lastFramePressed = now;

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
    g_buttons[Button].lastFramePressed = z_fps_ticksGet();
}
