#!/bin/sh
geany -i neonrattle/* make/* dev.sh README.md &
arduino neonrattle/neonrattle.ino &
gimp -n assets/gfx/palette.png assets/gfx/maps_grid16x16.xcf &
cd make
