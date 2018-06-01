/*
    Copyright 2018 Alex Margarit <alex@alxm.org>
    This file is part of Neon Rattle.
    Generated by assets/gfx/compile.py from ../assets/gfx/alxm_wing.png
*/

#if Z_PLATFORM_ARDUBOY

static const uint8_t z_data_gfx_alxm_wing_frames = 1;

PROGMEM static const uint8_t z_data_gfx_alxm_wing_buffer[] = {
    // Frame dimension
    11, 6,

    // Image frames
    0x18,0x1e,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x07,0x01,0x00,

    // Mask frames
    0x18,0x3e,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x0f,0x02,
};

#elif Z_PLATFORM_META

static const uint8_t z_data_gfx_alxm_wing_frames = 1;

static const uint16_t z_data_gfx_alxm_wing_buffer[] = {
    // Frame dimension
    11, 6,

    // Number of frames, looping
    1, 0,

    // Transparent color, color mode
    0xf81f, 0,

    // Image frames
    0xf81f,0xf81f,0xce79,0xce79,0x4cfd,0x4cfd,0x4e75,0x4e75,0xaee9,0xaee9,0xf81f,0xf81f,0xce79,0xce79,0x4cfd,0x4cfd,0x4e75,0x4e75,0xaee9,0xaee9,0x1861,0x1861,0xf81f,0xce79,0xce79,0x4cfd,0x4cfd,0x4e75,0x4e75,0xaee9,0xaee9,0x1861,0xf81f,0xce79,0xce79,0x4cfd,0x4cfd,0x4e75,0x4e75,0xaee9,0xaee9,0x1861,0x1861,0xf81f,0xad75,0xad75,0x3c7b,0x3c7b,0x3df3,0x3df3,0x9e67,0x9e67,0x1861,0xf81f,0xf81f,0xf81f,0x1861,0x1861,0x1861,0x1861,0x1861,0x1861,0x1861,0x1861,0xf81f,0xf81f,
};

#endif
