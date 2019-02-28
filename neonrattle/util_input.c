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

#include "util_input.h"

#include "util_timer.h"

typedef enum {
    Z_PRESSED = Z_FLAG_BIT(0),
    Z_WAIT_FOR_RELEASE = Z_FLAG_BIT(1),
} ZButtonFlags;

typedef struct {
    unsigned flags;
    unsigned ticksHeldDown;
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
        ZButton* button = &g_buttons[b];
        bool pressed = z_platform_buttonPressGet(b);

        if(Z_FLAG_TEST_ANY(button->flags, Z_WAIT_FOR_RELEASE)) {
            if(!pressed) {
                Z_FLAG_CLEAR(button->flags, Z_WAIT_FOR_RELEASE);
            }
        } else {
            if(pressed && Z_FLAG_TEST_ANY(button->flags, Z_PRESSED)) {
                if(button->ticksHeldDown < UINT_MAX) {
                    button->ticksHeldDown++;
                }
            } else {
                button->ticksHeldDown = UINT_MAX;
            }

            if(pressed) {
                Z_FLAG_SET(button->flags, Z_PRESSED);
            } else {
                Z_FLAG_CLEAR(button->flags, Z_PRESSED);
            }
        }
    }
}

bool z_button_pressGet(ZButtonId Button)
{
    return Z_FLAG_TEST_ANY(g_buttons[Button].flags, Z_PRESSED);
}

bool z_button_pressGetOnce(ZButtonId Button)
{
    if(Z_FLAG_TEST_ANY(g_buttons[Button].flags, Z_PRESSED)) {
        z_button_pressClear(Button);

        return true;
    }

    return false;
}

bool z_button_pressGetDelay(ZButtonId Button, unsigned Ms)
{
    ZButton* button = &g_buttons[Button];

    if(Z_FLAG_TEST_ANY(button->flags, Z_PRESSED)
        && button->ticksHeldDown >= z_timer_msToTicks(Ms)) {

        button->ticksHeldDown = 0;

        return true;
    }

    return false;
}

bool z_button_pressGetAny(void)
{
    for(int b = 0; b < Z_BUTTON_NUM; b++) {
        if(Z_FLAG_TEST_ANY(g_buttons[b].flags, Z_PRESSED)) {
            return true;
        }
    }

    return false;
}

void z_button_pressClear(ZButtonId Button)
{
    ZButton* button = &g_buttons[Button];

    Z_FLAG_CLEAR(button->flags, Z_PRESSED);
    Z_FLAG_SET(button->flags, Z_WAIT_FOR_RELEASE);
}
