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

#pragma once

Z_EXTERN_C_START

static inline void z_pixel_drawAlpha(ZPixel* Dst, int Red, int Green, int Blue, int Alpha)
{
    int r, g, b;
    z_pixel_toRGB(*Dst, &r, &g, &b);

    *Dst = z_pixel_fromRGB(r + (((Red   - r) * Alpha) >> 8),
                           g + (((Green - g) * Alpha) >> 8),
                           b + (((Blue  - b) * Alpha) >> 8));
}

Z_EXTERN_C_END
