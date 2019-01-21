#!/bin/sh
geany -i neonrattle/* make/* assets/gfx/*.py assets/sfx/*.py dev.sh README.md &
arduino neonrattle/neonrattle.ino &
gimp -n assets/gfx/palette.png assets/gfx/maps_grid16x16.xcf &
cd make
