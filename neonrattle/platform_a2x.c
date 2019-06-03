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

#include "platform.h"

#include "util_fps.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_sound.h"
#include "util_state.h"

#if Z_PLATFORM_A2X
typedef struct {
    ASample* sample;
    int channel;
} ZSfx;

ZColor z_colors[Z_COLOR_NUM];
static ASprite* g_sprites[Z_SPRITE_NUM];
static ZSfx g_sfx[Z_SFX_NUM];
static AButton* g_buttons[Z_BUTTON_NUM];

void s_run(void)
{
    A_STATE_INIT
    {
        g_buttons[Z_BUTTON_UP] = a_button_new();
        a_button_bindKey(g_buttons[Z_BUTTON_UP], A_KEY_UP);
        a_button_bindButton(g_buttons[Z_BUTTON_UP], NULL, A_BUTTON_UP);

        g_buttons[Z_BUTTON_DOWN] = a_button_new();
        a_button_bindKey(g_buttons[Z_BUTTON_DOWN], A_KEY_DOWN);
        a_button_bindButton(g_buttons[Z_BUTTON_DOWN], NULL, A_BUTTON_DOWN);

        g_buttons[Z_BUTTON_LEFT] = a_button_new();
        a_button_bindKey(g_buttons[Z_BUTTON_LEFT], A_KEY_LEFT);
        a_button_bindButton(g_buttons[Z_BUTTON_LEFT], NULL, A_BUTTON_LEFT);
        a_button_bindButton(g_buttons[Z_BUTTON_LEFT], NULL, A_BUTTON_L);

        g_buttons[Z_BUTTON_RIGHT] = a_button_new();
        a_button_bindKey(g_buttons[Z_BUTTON_RIGHT], A_KEY_RIGHT);
        a_button_bindButton(g_buttons[Z_BUTTON_RIGHT], NULL, A_BUTTON_RIGHT);
        a_button_bindButton(g_buttons[Z_BUTTON_RIGHT], NULL, A_BUTTON_R);

        g_buttons[Z_BUTTON_A] = a_button_new();
        a_button_bindKey(g_buttons[Z_BUTTON_A], A_KEY_SPACE);
        a_button_bindKey(g_buttons[Z_BUTTON_A], A_KEY_Z);
        a_button_bindButton(g_buttons[Z_BUTTON_A], NULL, A_BUTTON_A);

        g_buttons[Z_BUTTON_B] = a_button_new();
        a_button_bindKey(g_buttons[Z_BUTTON_B], A_KEY_X);
        a_button_bindButton(g_buttons[Z_BUTTON_B], NULL, A_BUTTON_B);

        g_buttons[Z_BUTTON_MENU] = a_button_new();
        a_button_bindKey(g_buttons[Z_BUTTON_MENU], A_KEY_ENTER);
        a_button_bindButton(g_buttons[Z_BUTTON_MENU], NULL, A_BUTTON_START);

        z_state_setup();
    }

    A_STATE_TICK
    {
        z_state_tick();
    }

    A_STATE_DRAW
    {
        z_state_draw();
    }

    A_STATE_FREE
    {
        for(int b = 0; b < Z_BUTTON_NUM; b++) {
            a_button_free(g_buttons[b]);
        }

        for(int s = 0; s < Z_SPRITE_NUM; s++) {
            a_sprite_free(g_sprites[s]);
        }

        for(int s = 0; s < Z_SFX_NUM; s++) {
            a_sample_free(g_sfx[s].sample);
        }
    }
}

void a_main(void)
{
    a_state_push(s_run);
}

bool z_platform_buttonPressGet(int Button)
{
    return a_button_pressGet(g_buttons[Button]);
}

ZPixel* z_screen_pixelsGet(void)
{
    return a_screen_pixelsGetBuffer();
}

void z_platform__loadSprite(int Sprite, const char* Path)
{
    g_sprites[Sprite] = a_sprite_newFromPng(Path, 0, 0, 0, 0);
}

ZPixel z_sprite_transparentColorGet(void)
{
    return a_pixel_fromHex(A_CONFIG_COLOR_SPRITE_KEY);
}

const ZPixel* z_sprite_pixelsGet(ZSpriteId Sprite, unsigned Frame)
{
    return a_sprite_pixelsGetBuffer(g_sprites[Sprite], Frame);
}

ZPixel z_sprite_pixelGet(ZSpriteId Sprite, unsigned Frame, int X, int Y)
{
    return a_sprite_pixelsGetValue(g_sprites[Sprite], Frame, X, Y);
}

void z_platform_spriteBlit(int Sprite, int X, int Y, unsigned Frame)
{
    a_sprite_blit(g_sprites[Sprite], Frame, X, Y);
}

ZVectorInt z_sprite_sizeGet(ZSpriteId Sprite)
{
    AVectorInt size = a_sprite_sizeGet(g_sprites[Sprite]);

    return (ZVectorInt){size.x, size.y};
}

int z_sprite_sizeGetWidth(ZSpriteId Sprite)
{
    return a_sprite_sizeGetWidth(g_sprites[Sprite]);
}

int z_sprite_sizeGetHeight(ZSpriteId Sprite)
{
    return a_sprite_sizeGetHeight(g_sprites[Sprite]);
}

unsigned z_sprite_framesNumGet(ZSpriteId Sprite)
{
    return a_sprite_framesNumGet(g_sprites[Sprite]);
}

unsigned z_fps_ticksGet(void)
{
    return a_fps_ticksGet();
}

bool z_fps_ticksNth(unsigned N)
{
    return a_fps_ticksNth(N);
}

void z_platform__loadSfx(int Sfx, const char* Path)
{
    g_sfx[Sfx].sample = a_sample_new(Path);
    g_sfx[Sfx].channel = a_channel_new();
}

void z_sfx_play(ZSfxId Sfx)
{
    a_channel_play(g_sfx[Sfx].channel, g_sfx[Sfx].sample, A_CHANNEL_RESTART);
}

bool z_file_readOnce(const char* FileName, void* Buffer, size_t Size)
{
    AFile* f = a_file_new(FileName, A_FILE_READ | A_FILE_BINARY);

    if(f == NULL) {
        return false;
    }

    bool ret = a_file_read(f, Buffer, Size);

    a_file_free(f);

    return ret;
}

bool z_file_writeOnce(const char* FileName, const void* Buffer, size_t Size)
{
    AFile* f = a_file_new(FileName, A_FILE_WRITE | A_FILE_BINARY);

    if(f == NULL) {
        return false;
    }

    bool ret = a_file_write(f, Buffer, Size);
    a_file_free(f);

    return ret;
}
#endif // Z_PLATFORM_A2X
