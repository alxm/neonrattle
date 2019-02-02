#!/usr/bin/env python3

"""
    Copyright 2018 Alex Margarit <alex@alxm.org>
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
"""

import re
import sys

from PIL import Image

class Palette:
    def __init__(self, PaletteName):
        image = Image.open(PaletteName).convert('RGB')
        width, height = image.size

        if width < 2 or height < 1:
            print('Invalid palette {}'.format(PaletteName))
            sys.exit(1)

        pixels = image.load()

        self.transparent = pixels[0, 0]

class Sheet:
    def __init__(self, ImageName, Palette):
        image = Image.open(ImageName).convert('RGB')

        self.width, self.height = image.size
        self.pixels = image.load()

        self.frameWidth = 0
        self.frameHeight = 0

        if Palette.transparent in image.getdata():
            self.transparentColor = self.rgb888ToRgb565(Palette.transparent)
        else:
            self.transparentColor = 0

        grid_match = re.match('^.*_grid([0-9]+)x([0-9]+)\..*$', ImageName)

        if grid_match:
            self.frameWidth = int(grid_match.group(1))
            self.frameHeight = int(grid_match.group(2))
        else:
            self.frameWidth = self.width
            self.frameHeight = self.height

    def rgb888ToRgb565(self, Pixel):
        r, g, b = Pixel

        return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)

class Frame:
    def __init__(self, Palette, Sheet, X, Y):
        self.metaSpriteBytes = []

        #
        # Gamebuino META
        #
        for y in range(Y, Y + Sheet.frameHeight):
            for x in range(X, X + Sheet.frameWidth):
                packed = Sheet.rgb888ToRgb565(Sheet.pixels[x, y])
                self.metaSpriteBytes.append(packed)

def main(PaletteName, ImageName, UniqueName):
    palette = Palette(PaletteName)
    sheet = Sheet(ImageName, palette)

    metaSpriteBytes = []
    numFrames = 0

    for y in range(0, sheet.height, sheet.frameHeight):
        for x in range(0, sheet.width, sheet.frameWidth):
            frame = Frame(palette, sheet, x, y)
            metaSpriteBytes += frame.metaSpriteBytes
            numFrames += 1

    def formatBytes16(Bytes):
        formattedBytes = ''

        for index, pixel in enumerate(Bytes):
            """if index % 10 == 0:
                formattedBytes += '\n    '"""

            formattedBytes += '0x{:0>4x},'.format(pixel)

        return formattedBytes

    contents = """\
/*
    Copyright Alex Margarit <alex@alxm.org>
    This file is part of Neonrattle, a video game.
    Generated by assets/gfx/compile.py from {path}
*/

#if Z_PLATFORM_META
"""

    if PaletteName == ImageName:
        contents += """
#define Z_META_IMAGE_HEADER_LEN 6
"""

    contents += """
static const uint16_t z_data_gfx_{name}_buffer[] = {{
    // Frame dimension
    {width}, {height},

    // Number of frames, looping
    {numFrames}, 0,

    // Transparent color, color mode
    0x{transparentColor:0>4x}, 0,

    // Image frames
    {metaSpriteBytes}
}};

#endif"""

    print(contents.format(path = ImageName,
                          name = UniqueName,
                          numFrames = numFrames,
                          width = sheet.frameWidth,
                          height = sheet.frameHeight,
                          transparentColor = sheet.transparentColor,
                          metaSpriteBytes = formatBytes16(metaSpriteBytes)))

if __name__ == '__main__':
    if len(sys.argv) != 4:
        print('Usage: {} Palette.png Image.png UniqueName'.format(sys.argv[0]))
        sys.exit(1)
    else:
        main(sys.argv[1], sys.argv[2], sys.argv[3])