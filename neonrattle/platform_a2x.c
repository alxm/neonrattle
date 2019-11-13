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

#if Z_PLATFORM_FAUR
typedef struct {
    FSample* sample;
    int channel;
} ZSfx;

ZColor z_colors[Z_COLOR_NUM];
static FSprite* g_sprites[Z_SPRITE_NUM];
static ZSfx g_sfx[Z_SFX_NUM];
static FButton* g_buttons[Z_BUTTON_NUM];

void s_run(void)
{
    F_STATE_INIT
    {
        g_buttons[Z_BUTTON_UP] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_UP], F_KEY_UP);
        f_button_bindButton(g_buttons[Z_BUTTON_UP], NULL, F_BUTTON_UP);

        g_buttons[Z_BUTTON_DOWN] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_DOWN], F_KEY_DOWN);
        f_button_bindButton(g_buttons[Z_BUTTON_DOWN], NULL, F_BUTTON_DOWN);

        g_buttons[Z_BUTTON_LEFT] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_LEFT], F_KEY_LEFT);
        f_button_bindButton(g_buttons[Z_BUTTON_LEFT], NULL, F_BUTTON_LEFT);
        f_button_bindButton(g_buttons[Z_BUTTON_LEFT], NULL, F_BUTTON_L);

        g_buttons[Z_BUTTON_RIGHT] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_RIGHT], F_KEY_RIGHT);
        f_button_bindButton(g_buttons[Z_BUTTON_RIGHT], NULL, F_BUTTON_RIGHT);
        f_button_bindButton(g_buttons[Z_BUTTON_RIGHT], NULL, F_BUTTON_R);

        g_buttons[Z_BUTTON_A] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_A], F_KEY_SPACE);
        f_button_bindKey(g_buttons[Z_BUTTON_A], F_KEY_Z);
        f_button_bindButton(g_buttons[Z_BUTTON_A], NULL, F_BUTTON_A);

        g_buttons[Z_BUTTON_B] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_B], F_KEY_X);
        f_button_bindButton(g_buttons[Z_BUTTON_B], NULL, F_BUTTON_B);

        g_buttons[Z_BUTTON_MENU] = f_button_new();
        f_button_bindKey(g_buttons[Z_BUTTON_MENU], F_KEY_ENTER);
        f_button_bindButton(g_buttons[Z_BUTTON_MENU], NULL, F_BUTTON_START);

        z_state_setup();
    }

    F_STATE_TICK
    {
        z_state_tick();
    }

    F_STATE_DRAW
    {
        z_state_draw();
    }

    F_STATE_FREE
    {
        for(int b = 0; b < Z_BUTTON_NUM; b++) {
            f_button_free(g_buttons[b]);
        }

        for(int s = 0; s < Z_SPRITE_NUM; s++) {
            f_sprite_free(g_sprites[s]);
        }

        for(int s = 0; s < Z_SFX_NUM; s++) {
            f_sample_free(g_sfx[s].sample);
        }
    }
}

void f_main(void)
{
    f_state_push(s_run);
}

bool z_platform_buttonPressGet(int Button)
{
    return f_button_pressGet(g_buttons[Button]);
}

ZPixel* z_screen_pixelsGet(void)
{
    return f_screen_pixelsGetBuffer();
}

void z_platform__loadSprite(int Sprite, const char* Path)
{
    g_sprites[Sprite] = f_sprite_newFromPng(Path, 0, 0, 0, 0);
}

ZPixel z_sprite_transparentColorGet(void)
{
    return f_color_pixelFromHex(F_CONFIG_COLOR_SPRITE_KEY);
}

const ZPixel* z_sprite_pixelsGet(ZSpriteId Sprite, unsigned Frame)
{
    return f_sprite_pixelsGetBuffer(g_sprites[Sprite], Frame);
}

ZPixel z_sprite_pixelGet(ZSpriteId Sprite, unsigned Frame, int X, int Y)
{
    return f_sprite_pixelsGetValue(g_sprites[Sprite], Frame, X, Y);
}

void z_platform_spriteBlit(int Sprite, int X, int Y, unsigned Frame)
{
    f_sprite_blit(g_sprites[Sprite], Frame, X, Y);
}

ZVectorInt z_sprite_sizeGet(ZSpriteId Sprite)
{
    FVectorInt size = f_sprite_sizeGet(g_sprites[Sprite]);

    return (ZVectorInt){size.x, size.y};
}

int z_sprite_sizeGetWidth(ZSpriteId Sprite)
{
    return f_sprite_sizeGetWidth(g_sprites[Sprite]);
}

int z_sprite_sizeGetHeight(ZSpriteId Sprite)
{
    return f_sprite_sizeGetHeight(g_sprites[Sprite]);
}

unsigned z_sprite_framesNumGet(ZSpriteId Sprite)
{
    return f_sprite_framesNumGet(g_sprites[Sprite]);
}

unsigned z_fps_ticksGet(void)
{
    return f_fps_ticksGet();
}

bool z_fps_ticksNth(unsigned N)
{
    return f_fps_ticksNth(N);
}

void z_platform__loadSfx(int Sfx, const char* Path)
{
    g_sfx[Sfx].sample = f_sample_new(Path);
    g_sfx[Sfx].channel = f_channel_new();
}

void z_sfx_play(ZSfxId Sfx)
{
    f_channel_play(
        g_sfx[Sfx].channel, g_sfx[Sfx].sample, F_CHANNEL_PLAY_RESTART);
}

bool z_file_readOnce(const char* FileName, void* Buffer, size_t Size)
{
    FFile* f = f_file_new(FileName, F_FILE_READ | F_FILE_BINARY);

    if(f == NULL) {
        return false;
    }

    bool ret = f_file_read(f, Buffer, Size);

    f_file_free(f);

    return ret;
}

bool z_file_writeOnce(const char* FileName, const void* Buffer, size_t Size)
{
    FFile* f = f_file_new(FileName, F_FILE_WRITE | F_FILE_BINARY);

    if(f == NULL) {
        return false;
    }

    bool ret = f_file_write(f, Buffer, Size);
    f_file_free(f);

    return ret;
}
#endif // Z_PLATFORM_FAUR
