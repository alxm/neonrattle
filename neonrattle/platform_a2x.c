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
#include "util_fps.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_sound.h"

#if Z_PLATFORM_A2X
ZColor z_colors[Z_COLOR_NUM];
static AButton* g_buttons[Z_BUTTON_NUM];
static ASpriteFrames* g_sprites[Z_SPRITE_NUM];
static struct {
    ASample* sample;
    int channel;
} g_sfx[Z_SFX_NUM];

A_SETUP
{
    a_settings_stringSet(A_SETTING_APP_TITLE, "Neonrattle");
    a_settings_stringSet(A_SETTING_APP_VERSION, "0.2.0");
    a_settings_stringSet(A_SETTING_APP_AUTHOR, "alxm");
    a_settings_boolSet(A_SETTING_OUTPUT_ON, true);
    a_settings_boolSet(A_SETTING_OUTPUT_VERBOSE, true);
    a_settings_colorSet(A_SETTING_COLOR_SCREEN_BORDER, 0x042a26);
    a_settings_intSet(A_SETTING_VIDEO_WIDTH, Z_SCREEN_W);
    a_settings_intSet(A_SETTING_VIDEO_HEIGHT, Z_SCREEN_H);
    a_settings_intSet(A_SETTING_SOUND_VOLUME_SCALE_SAMPLE, 50);
    a_settings_intuSet(A_SETTING_FPS_TICK, Z_FPS);
    a_settings_intuSet(A_SETTING_FPS_DRAW, Z_FPS);
}

A_STATE(run)
{
    A_STATE_INIT
    {
        g_buttons[Z_BUTTON_UP] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_UP], A_KEY_UP);
        a_button_bind(g_buttons[Z_BUTTON_UP], A_BUTTON_UP);

        g_buttons[Z_BUTTON_DOWN] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_DOWN], A_KEY_DOWN);
        a_button_bind(g_buttons[Z_BUTTON_DOWN], A_BUTTON_DOWN);

        g_buttons[Z_BUTTON_LEFT] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_LEFT], A_KEY_LEFT);
        a_button_bind(g_buttons[Z_BUTTON_LEFT], A_BUTTON_LEFT);

        g_buttons[Z_BUTTON_RIGHT] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_RIGHT], A_KEY_RIGHT);
        a_button_bind(g_buttons[Z_BUTTON_RIGHT], A_BUTTON_RIGHT);

        g_buttons[Z_BUTTON_A] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_A], A_KEY_Z);
        a_button_bind(g_buttons[Z_BUTTON_A], A_BUTTON_A);

        g_buttons[Z_BUTTON_B] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_B], A_KEY_X);
        a_button_bind(g_buttons[Z_BUTTON_B], A_BUTTON_B);

        g_buttons[Z_BUTTON_MENU] = a_button_new();
        a_button_bind(g_buttons[Z_BUTTON_MENU], A_KEY_ENTER);
        a_button_bind(g_buttons[Z_BUTTON_MENU], A_BUTTON_START);

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
        for(ZButtonId b = 0; b < Z_BUTTON_NUM; b++) {
            a_button_free(g_buttons[b]);
        }

        for(ZSpriteId s = 0; s < Z_SPRITE_NUM; s++) {
            a_spriteframes_free(g_sprites[s], true);
        }

        for(ZSfxId s = 0; s < Z_SFX_NUM; s++) {
            a_sample_free(g_sfx[s].sample);
        }
    }
}

A_MAIN
{
    a_state_push(run, "Neonrattle");
}

bool z_button_pressGet(ZButtonId Button)
{
    return a_button_pressGet(g_buttons[Button]);
}

void z_button_pressClear(ZButtonId Button)
{
    a_button_pressClear(g_buttons[Button]);
}

ZPixel* z_screen_pixelsGet(void)
{
    return a_screen_pixelsGetBuffer();
}

void z_platform__loadSprite(int Sprite, const char* Path)
{
    g_sprites[Sprite] = a_spriteframes_newFromPng(Path);
}

void z_platform__loadSpriteGrid(int Sprite, const char* Path, int W, int H)
{
    g_sprites[Sprite] = a_spriteframes_newFromPngGrid(Path, W, H);
}

ZPixel z_sprite_transparentColorGet(void)
{
    return a_sprite_colorKeyGet();
}

static inline ASprite* getCurrentSprite(ZSpriteId Sprite, unsigned Frame)
{
    return a_spriteframes_getByIndex(g_sprites[Sprite], Frame);
}

const ZPixel* z_sprite_pixelsGet(ZSpriteId Sprite, unsigned Frame)
{
    return a_sprite_pixelsGetBuffer(getCurrentSprite(Sprite, Frame));
}

ZPixel z_sprite_pixelGet(ZSpriteId Sprite, unsigned Frame, int X, int Y)
{
    return a_sprite_pixelsGetPixel(getCurrentSprite(Sprite, Frame), X, Y);
}

void z_sprite_blit(ZSpriteId Sprite, int X, int Y, unsigned Frame)
{
    a_sprite_blit(getCurrentSprite(Sprite, Frame), X, Y);
}

int z_sprite_widthGet(ZSpriteId Sprite)
{
    return a_sprite_widthGet(getCurrentSprite(Sprite, 0));
}

int z_sprite_heightGet(ZSpriteId Sprite)
{
    return a_sprite_heightGet(getCurrentSprite(Sprite, 0));
}

uint8_t z_sprite_framesNumGet(ZSpriteId Sprite)
{
    return (uint8_t)a_spriteframes_numGet(g_sprites[Sprite]);
}

void z_draw_fill(ZColorId Color)
{
    a_pixel_colorSetPixel(z_colors[Color].pixel);
    a_draw_fill();
}

void z_draw_rectangle(int X, int Y, int W, int H, ZColorId Color)
{
    a_pixel_colorSetPixel(z_colors[Color].pixel);
    a_draw_rectangle(X, Y, W, H);
}

void z_draw_pixel(int X, int Y, ZColorId Color)
{
    a_pixel_colorSetPixel(z_colors[Color].pixel);
    a_draw_pixel(X, Y);
}

void z_draw_circle(int X, int Y, int Radius, ZColorId Color)
{
    a_pixel_push();

    a_pixel_colorSetPixel(z_colors[Color].pixel);
    a_pixel_fillDrawSet(false);
    a_draw_circle(X, Y, Radius);

    a_pixel_pop();
}

uint16_t z_fps_ticksGet(void)
{
    return (uint16_t)a_fps_ticksGet();
}

bool z_fps_ticksNth(uint8_t N)
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
#endif // Z_PLATFORM_A2X
