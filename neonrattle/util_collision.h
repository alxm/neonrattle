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

static inline bool z_collision_boxAndBox(int X1, int Y1, int W1, int H1, int X2, int Y2, int W2, int H2)
{
    X1 = X1 - W1 / 2;
    Y1 = Y1 - H1 / 2;

    X2 = X2 - W2 / 2;
    Y2 = Y2 - H2 / 2;

    return !(Y1 >= Y2 + H2 || Y2 >= Y1 + H1 || X1 >= X2 + W2 || X2 >= X1 + W1);
}

static inline bool z_collision_sqAndSq(int X1, int Y1, int Dim1, int X2, int Y2, int Dim2)
{
    X1 = X1 - Dim1 / 2;
    Y1 = Y1 - Dim1 / 2;

    X2 = X2 - Dim2 / 2;
    Y2 = Y2 - Dim2 / 2;

    return !(Y1 >= Y2 + Dim2 || Y2 >= Y1 + Dim1 || X1 >= X2 + Dim2 || X2 >= X1 + Dim1);
}
