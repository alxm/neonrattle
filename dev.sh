#!/bin/sh
geany -i neonrattle/* make/* assets/gfx/*.py assets/sfx/*.py dev.sh README.md &
arduino neonrattle/neonrattle.ino &
gimp -n assets/gfx/*.png assets/gfx/*.BMP &
cd make
