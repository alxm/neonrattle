#!/bin/sh
geany -i build/make/* neonrattle/* dev.sh README.md &
arduino neonrattle/neonrattle.ino &
gimp -n assets/gfx/maps_16x16.xcf assets/gfx/palette.png &
cd build/make
