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

#include "util_fps.h"
#include "util_graphics.h"
#include "util_input.h"
#include "util_sound.h"
#include "util_state.h"

#include "generated/data_gfx_palette.h"

#if Z_PLATFORM_META
typedef struct {
    Button index;
    bool pressed : 1;
    bool released : 1;
} ZButton;

typedef struct {
    Image image;
    const uint16_t* buffer;
} ZSprite;

typedef struct {
    const uint8_t* buffer;
    uint32_t size;
} ZSfx;

ZColor z_colors[Z_COLOR_NUM];
static ZButton g_buttons[Z_BUTTON_NUM];
static ZSprite g_sprites[Z_SPRITE_NUM];
static ZSfx g_sfx[Z_SFX_NUM];

void setup(void)
{
    gb.begin();
    gb.setFrameRate(Z_FPS);

    g_buttons[Z_BUTTON_UP].index = BUTTON_UP;
    g_buttons[Z_BUTTON_DOWN].index = BUTTON_DOWN;
    g_buttons[Z_BUTTON_LEFT].index = BUTTON_LEFT;
    g_buttons[Z_BUTTON_RIGHT].index = BUTTON_RIGHT;
    g_buttons[Z_BUTTON_A].index = BUTTON_A;
    g_buttons[Z_BUTTON_B].index = BUTTON_B;
    g_buttons[Z_BUTTON_MENU].index = BUTTON_MENU;

    z_state_setup();
}

void loop(void)
{
    if(!gb.update()) {
        return;
    }

    for(uint8_t b = 0; b < Z_BUTTON_NUM; b++) {
        bool pressed = gb.buttons.repeat(g_buttons[b].index, 0);

        if(g_buttons[b].released) {
            if(!pressed) {
                g_buttons[b].released = false;
            }
        } else {
            g_buttons[b].pressed = pressed;
        }
    }

    z_state_tick();
    z_state_draw();

    #if Z_DEBUG_STATS
        gb.display.setColor(WHITE);
        gb.display.setCursor(2, 2);
        gb.display.print(gb.getCpuLoad(), DEC);
        gb.display.setCursor(2, 8);
        gb.display.print(gb.getFreeRam(), DEC);
        gb.display.setCursor(2, 14);
        gb.display.print(SOUND_CHANNELS, DEC);
    #endif
}

bool z_button_pressGet(ZButtonId Button)
{
    return g_buttons[Button].pressed;
}

void z_button_pressClear(ZButtonId Button)
{
    g_buttons[Button].pressed = false;
    g_buttons[Button].released = true;
}

ZPixel* z_screen_pixelsGet(void)
{
    return gb.display._buffer;
}

static void prepLights(ZColorId BgColorId, ZColorId ColorId, int Alpha)
{
    ZPixel color = 0;

    if(ColorId != Z_COLOR_INVALID) {
        int r2 = z_colors[ColorId].r;
        int g2 = z_colors[ColorId].g;
        int b2 = z_colors[ColorId].b;

        if(BgColorId != Z_COLOR_INVALID) {
            int r1 = z_colors[BgColorId].r;
            int g1 = z_colors[BgColorId].g;
            int b1 = z_colors[BgColorId].b;

            color = z_pixel_fromRGB(r1 + (((r2 - r1) * Alpha) >> 8),
                                    g1 + (((g2 - g1) * Alpha) >> 8),
                                    b1 + (((b2 - b1) * Alpha) >> 8));
        } else {
            color = z_pixel_fromRGB((r2 * Alpha) >> 8,
                                    (g2 * Alpha) >> 8,
                                    (b2 * Alpha) >> 8);
        }
    } else if(BgColorId != Z_COLOR_INVALID) {
        color = z_colors[BgColorId].pixel;
    }

    gb.lights.setColor((Color)color);
}

void z_platform_meta_lightsFill(int BgColorId, int ColorId, int Alpha)
{
    prepLights((ZColorId)BgColorId, (ZColorId)ColorId, Alpha);
    gb.lights.fill();
}

void z_platform_meta_lightsDraw(int ColorId, int Alpha, int X, int Y)
{
    prepLights(Z_COLOR_INVALID, (ZColorId)ColorId, Alpha);
    gb.lights.drawPixel(X, Y);
}

void z_platform__loadSprite(int Sprite, const uint16_t* Buffer)
{
    g_sprites[Sprite].image.init(Buffer);
    g_sprites[Sprite].buffer = Buffer;
}

ZPixel z_sprite_transparentColorGet(void)
{
    return g_sprites[Z_SPRITE_PALETTE].buffer[4];
}

const ZPixel* z_sprite_pixelsGet(ZSpriteId Sprite, unsigned Frame)
{
    unsigned dim = g_sprites[Sprite].buffer[0] * g_sprites[Sprite].buffer[1];

    return g_sprites[Sprite].buffer + Z_META_IMAGE_HEADER_LEN + dim * Frame;
}

ZPixel z_sprite_pixelGet(ZSpriteId Sprite, unsigned Frame, int X, int Y)
{
    unsigned w = g_sprites[Sprite].buffer[0];
    unsigned dim = w * g_sprites[Sprite].buffer[1];

    return *(g_sprites[Sprite].buffer
                + Z_META_IMAGE_HEADER_LEN + dim * Frame + Y * w + X);
}

void z_sprite_blit(ZSpriteId Sprite, int X, int Y, unsigned Frame)
{
    g_sprites[Sprite].image.setFrame(Frame);
    gb.display.drawImage(X, Y, g_sprites[Sprite].image);
}

int z_sprite_widthGet(ZSpriteId Sprite)
{
    return g_sprites[Sprite].buffer[0];
}

int z_sprite_heightGet(ZSpriteId Sprite)
{
    return g_sprites[Sprite].buffer[1];
}

uint8_t z_sprite_framesNumGet(ZSpriteId Sprite)
{
    return g_sprites[Sprite].buffer[2];
}

void z_draw_fill(ZColorId ColorId)
{
    gb.display.setColor((Color)z_colors[ColorId].pixel);
    gb.display.fill();
}

void z_draw_rectangle(int X, int Y, int W, int H, ZColorId ColorId)
{
    gb.display.setColor((Color)z_colors[ColorId].pixel);
    gb.display.fillRect(X, Y, W, H);
}

void z_draw_pixel(int X, int Y, ZColorId ColorId)
{
    gb.display.setColor((Color)z_colors[ColorId].pixel);
    gb.display.drawPixel(X, Y);
}

void z_draw_circle(int X, int Y, int Radius, ZColorId ColorId)
{
    gb.display.setColor((Color)z_colors[ColorId].pixel);
    gb.display.drawCircle(X, Y, Radius);
}

uint16_t z_fps_ticksGet(void)
{
    return (uint16_t)gb.frameCount;
}

bool z_fps_ticksNth(uint8_t N)
{
    return (gb.frameCount % N) == 0;
}

void z_platform__loadSfx(int Sfx, const uint8_t* Buffer, uint32_t Size)
{
    g_sfx[Sfx].buffer = Buffer;
    g_sfx[Sfx].size = Size;
}

void z_sfx_play(ZSfxId Sfx)
{
    gb.sound.stop(0);
    gb.sound.play(g_sfx[Sfx].buffer, g_sfx[Sfx].size);
}
#endif // Z_PLATFORM_META
