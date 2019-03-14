#!/bin/sh
geany -i make/* neonrattle/* dev.sh README.md &
arduino neonrattle/neonrattle.ino &
gimp -n assets/gfx/maps_grid16x16.xcf assets/gfx/palette.png &
cd make
