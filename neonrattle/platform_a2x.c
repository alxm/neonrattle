/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Neon Rattle is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Neon Rattle is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Neon Rattle.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "platform.h"

#include "state.h"
#include "util_fps.h"
#include "util_input.h"

#if Z_PLATFORM_A2X
ZColor z_colors[Z_COLOR_NUM];
static AInputButton* g_buttons[Z_BUTTON_NUM];
static ASpriteFrames* g_sprites[Z_SPRITE_NUM];
static ASfx* g_sfx[Z_SFX_NUM];

A_SETUP
{
    a_settings_set("app.title", "Neon Rattle");
    a_settings_set("app.version", "Beta 2");
    a_settings_set("app.author", "alxm");
    a_settings_set("app.output.on", "yes");
    a_settings_set("app.output.verbose", "yes");
    a_settings_set("video.color.border", "0x042a26");
    a_settings_set("video.width", A_STRINGIFY(Z_SCREEN_W));
    a_settings_set("video.height", A_STRINGIFY(Z_SCREEN_H));
    a_settings_set("sound.sfx.scale", "25");
    a_settings_set("fps.tick", A_STRINGIFY(Z_FPS));
    a_settings_set("fps.draw", A_STRINGIFY(Z_FPS));
}

A_STATE(run)
{
    A_STATE_INIT
    {
        g_buttons[Z_BUTTON_UP] = a_button_new("key.up gamepad.b.up");
        g_buttons[Z_BUTTON_DOWN] = a_button_new("key.down gamepad.b.down");
        g_buttons[Z_BUTTON_LEFT] = a_button_new("key.left gamepad.b.left");
        g_buttons[Z_BUTTON_RIGHT] = a_button_new("key.right gamepad.b.right");
        g_buttons[Z_BUTTON_A] = a_button_new("key.z gamepad.b.a");
        g_buttons[Z_BUTTON_B] = a_button_new("key.x gamepad.b.b");
        g_buttons[Z_BUTTON_MENU] = a_button_new("key.enter gamepad.b.start");

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
            a_sfx_free(g_sfx[s]);
        }
    }
}

A_MAIN
{
    a_state_new("run", run, "", "");
    a_state_push("run");
}

bool z_button_pressed(ZButtonId Button)
{
    return a_button_getPressed(g_buttons[Button]);
}

void z_button_release(ZButtonId Button)
{
    a_button_release(g_buttons[Button]);
}

ZPixel* z_screen_getPixels(void)
{
    return a_screen_getPixels();
}

void z_platform__loadSprite(ZSpriteId Sprite, const char* Path)
{
    g_sprites[Sprite] = a_spriteframes_newFromFile(Path, 0);
}

ZPixel z_sprite_getTransparentColor(void)
{
    return a_sprite_getColorKey();
}

static inline ASprite* getCurrentSprite(ZSpriteId Sprite, unsigned Frame)
{
    return a_spriteframes_getByIndex(g_sprites[Sprite], Frame);
}

const ZPixel* z_sprite_getPixels(ZSpriteId Sprite, unsigned Frame)
{
    return a_sprite_getPixels(getCurrentSprite(Sprite, Frame));
}

ZPixel z_sprite_getPixel(ZSpriteId Sprite, unsigned Frame, int X, int Y)
{
    return a_sprite_getPixel(getCurrentSprite(Sprite, Frame), X, Y);
}

void z_sprite_blit(ZSpriteId Sprite, int X, int Y, unsigned Frame)
{
    a_sprite_blit(getCurrentSprite(Sprite, Frame), X, Y);
}

int z_sprite_getWidth(ZSpriteId Sprite)
{
    return a_sprite_getWidth(getCurrentSprite(Sprite, 0));
}

int z_sprite_getHeight(ZSpriteId Sprite)
{
    return a_sprite_getHeight(getCurrentSprite(Sprite, 0));
}

uint8_t z_sprite_getNumFrames(ZSpriteId Sprite)
{
    return u8(a_spriteframes_getNum(g_sprites[Sprite]));
}

void z_draw_fill(ZColorId ColorId)
{
    a_pixel_setPixel(z_colors[ColorId].pixel);
    a_draw_fill();
}

void z_draw_rectangle(int X, int Y, int W, int H, ZColorId ColorId)
{
    a_pixel_setPixel(z_colors[ColorId].pixel);
    a_draw_rectangle(X, Y, W, H);
}

void z_draw_pixel(int X, int Y, ZColorId ColorId)
{
    a_pixel_setPixel(z_colors[ColorId].pixel);
    a_draw_pixel(X, Y);
}

void z_draw_circle(int X, int Y, int Radius, ZColorId ColorId)
{
    a_pixel_push();

    a_pixel_setPixel(z_colors[ColorId].pixel);
    a_pixel_setFillDraw(false);
    a_draw_circle(X, Y, Radius);

    a_pixel_pop();
}

uint16_t z_fps_getCounter(void)
{
    return u16(a_fps_getCounter());
}

bool z_fps_isNthFrame(uint8_t N)
{
    return a_fps_isNthFrame(N);
}

void z_platform__loadSfx(ZSfxId Sfx, const char* Path)
{
    g_sfx[Sfx] = a_sfx_new(Path);
}

void z_sfx_play(ZSfxId Sfx)
{
    a_sfx_play(g_sfx[Sfx], A_SFX_RESTART);
}
#endif // Z_PLATFORM_A2X
