/*
    Copyright 2018-2019 Alex Margarit <alex@alxm.org>
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
*/

#pragma once

#include "platform.h"

typedef int32_t ZFix;
typedef uint32_t ZFixu;

#define Z_FIX_BIT_PRECISION (16)
#define Z_FIX_ONE           (1 << Z_FIX_BIT_PRECISION)
#define Z_FIX_FRACTION_MASK (Z_FIX_ONE - 1)
#define Z_FIX_MIN_INT       (INT32_MIN >> Z_FIX_BIT_PRECISION)
#define Z_FIX_MAX_INT       (INT32_MAX >> Z_FIX_BIT_PRECISION)
#define Z_FIXU_MAX_INT      (UINT32_MAX >> Z_FIX_BIT_PRECISION)

#define Z_ANGLES_NUM 4096u

typedef enum {
    Z_DEG_001_INT = (Z_ANGLES_NUM / 360),
    Z_DEG_022_INT = (Z_ANGLES_NUM / 16),
    Z_DEG_045_INT = (Z_ANGLES_NUM / 8),
    Z_DEG_067_INT = (Z_DEG_045_INT + Z_DEG_022_INT),
    Z_DEG_090_INT = (2 * Z_DEG_045_INT),
    Z_DEG_112_INT = (Z_DEG_090_INT + Z_DEG_022_INT),
    Z_DEG_135_INT = (3 * Z_DEG_045_INT),
    Z_DEG_157_INT = (Z_DEG_135_INT + Z_DEG_022_INT),
    Z_DEG_180_INT = (4 * Z_DEG_045_INT),
    Z_DEG_202_INT = (Z_DEG_180_INT + Z_DEG_022_INT),
    Z_DEG_225_INT = (5 * Z_DEG_045_INT),
    Z_DEG_247_INT = (Z_DEG_225_INT + Z_DEG_022_INT),
    Z_DEG_270_INT = (6 * Z_DEG_045_INT),
    Z_DEG_292_INT = (Z_DEG_270_INT + Z_DEG_022_INT),
    Z_DEG_315_INT = (7 * Z_DEG_045_INT),
    Z_DEG_337_INT = (Z_DEG_315_INT + Z_DEG_022_INT),
    Z_DEG_360_INT = (8 * Z_DEG_045_INT),
} ZDegInt;

typedef enum {
    Z_DEG_001_FIX = (Z_ANGLES_NUM * Z_FIX_ONE / 360),
    Z_DEG_022_FIX = (Z_DEG_022_INT * Z_FIX_ONE),
    Z_DEG_045_FIX = (Z_DEG_045_INT * Z_FIX_ONE),
    Z_DEG_067_FIX = (Z_DEG_067_INT * Z_FIX_ONE),
    Z_DEG_090_FIX = (Z_DEG_090_INT * Z_FIX_ONE),
    Z_DEG_112_FIX = (Z_DEG_112_INT * Z_FIX_ONE),
    Z_DEG_135_FIX = (Z_DEG_135_INT * Z_FIX_ONE),
    Z_DEG_157_FIX = (Z_DEG_157_INT * Z_FIX_ONE),
    Z_DEG_180_FIX = (Z_DEG_180_INT * Z_FIX_ONE),
    Z_DEG_202_FIX = (Z_DEG_202_INT * Z_FIX_ONE),
    Z_DEG_225_FIX = (Z_DEG_225_INT * Z_FIX_ONE),
    Z_DEG_247_FIX = (Z_DEG_247_INT * Z_FIX_ONE),
    Z_DEG_270_FIX = (Z_DEG_270_INT * Z_FIX_ONE),
    Z_DEG_292_FIX = (Z_DEG_292_INT * Z_FIX_ONE),
    Z_DEG_315_FIX = (Z_DEG_315_INT * Z_FIX_ONE),
    Z_DEG_337_FIX = (Z_DEG_337_INT * Z_FIX_ONE),
    Z_DEG_360_FIX = (Z_DEG_360_INT * Z_FIX_ONE),
} ZDegFix;

extern const ZFix z_fix__sin[Z_ANGLES_NUM];
extern const ZFix z_fix__csc[Z_ANGLES_NUM];

static inline ZFix z_fix_fromInt(int X)
{
    return X << Z_FIX_BIT_PRECISION;
}

static inline ZFix z_fix_fromFloat(float X)
{
    return (ZFix)(X * Z_FIX_ONE);
}

static inline ZFix z_fix_fromDouble(double X)
{
    return (ZFix)(X * Z_FIX_ONE);
}

static inline int z_fix_toInt(ZFix X)
{
    return X >> Z_FIX_BIT_PRECISION;
}

static inline float z_fix_toFloat(ZFix X)
{
    return (float)X / Z_FIX_ONE;
}

static inline double z_fix_toDouble(ZFix X)
{
    return (double)X / Z_FIX_ONE;
}

static inline ZFix z_fix_mul(ZFix X, ZFix Y)
{
    return (ZFix)(((int64_t)X * Y) >> Z_FIX_BIT_PRECISION);
}

static inline ZFix z_fix_div(ZFix X, ZFix Y)
{
    return (ZFix)(((int64_t)X << Z_FIX_BIT_PRECISION) / Y);
}

static inline ZFix z_fix_round(ZFix X)
{
    return (X + Z_FIX_ONE / 2) & (ZFix)~Z_FIX_FRACTION_MASK;
}

static inline ZFix z_fix_floor(ZFix X)
{
    return X & (ZFix)~Z_FIX_FRACTION_MASK;
}

static inline ZFix z_fix_ceiling(ZFix X)
{
    return (X + Z_FIX_ONE - 1) & (ZFix)~Z_FIX_FRACTION_MASK;
}

static inline ZFix z_fix_truncate(ZFix X)
{
    if(X >= 0) {
        return X & (ZFix)~Z_FIX_FRACTION_MASK;
    } else {
        return -((-X) & (ZFix)~Z_FIX_FRACTION_MASK);
    }
}

static inline ZFix z_fix_fraction(ZFix X)
{
    return X & Z_FIX_FRACTION_MASK;

    /*if(X >= 0) {
        return X & Z_FIX_FRACTION_MASK;
    } else {
        return -((-X) & Z_FIX_FRACTION_MASK);
    }*/
}

static inline ZFixu z_fixu_fromInt(unsigned X)
{
    return X << Z_FIX_BIT_PRECISION;
}

static inline ZFixu z_fixu_fromFloat(float X)
{
    return (ZFixu)(X * Z_FIX_ONE);
}

static inline ZFixu z_fixu_fromDouble(double X)
{
    return (ZFixu)(X * Z_FIX_ONE);
}

static inline unsigned z_fixu_toInt(ZFixu X)
{
    return X >> Z_FIX_BIT_PRECISION;
}

static inline float z_fixu_toFloat(ZFixu X)
{
    return (float)X / Z_FIX_ONE;
}

static inline double z_fixu_toDouble(ZFixu X)
{
    return (double)X / Z_FIX_ONE;
}

static inline ZFixu z_fixu_mul(ZFixu X, ZFixu Y)
{
    return (ZFixu)(((uint64_t)X * Y) >> Z_FIX_BIT_PRECISION);
}

static inline ZFixu z_fixu_div(ZFixu X, ZFixu Y)
{
    return (ZFixu)(((uint64_t)X << Z_FIX_BIT_PRECISION) / Y);
}

static inline ZFixu z_fixu_round(ZFixu X)
{
    return (X + Z_FIX_ONE / 2) & (ZFixu)~Z_FIX_FRACTION_MASK;
}

static inline ZFixu z_fixu_floor(ZFixu X)
{
    return X & (ZFixu)~Z_FIX_FRACTION_MASK;
}

static inline ZFixu z_fixu_ceiling(ZFixu X)
{
    return (X + Z_FIX_ONE - 1) & (ZFixu)~Z_FIX_FRACTION_MASK;
}

static inline ZFixu z_fixu_truncate(ZFixu X)
{
    return X & (ZFixu)~Z_FIX_FRACTION_MASK;
}

static inline ZFixu z_fixu_fraction(ZFixu X)
{
    return X & Z_FIX_FRACTION_MASK;
}

static inline unsigned z_fix_angleWrap(unsigned Angle)
{
    return Angle & (Z_ANGLES_NUM - 1);
}

static inline ZFixu z_fix_angleWrapf(ZFixu Angle)
{
    return Angle & (Z_ANGLES_NUM * Z_FIX_ONE - 1);
}

static inline unsigned z_fix_angleFromDeg(unsigned Degrees)
{
    return Z_ANGLES_NUM * Degrees / 360;
}

static inline ZFixu z_fix_angleFromDegf(unsigned Degrees)
{
    return (ZFixu)((uint64_t)(Z_FIX_ONE * Z_ANGLES_NUM) * Degrees / 360);
}

static inline ZFix z_fix_sin(unsigned Angle)
{
    return z_fix__sin[z_fix_angleWrap(Angle)];
}

static inline ZFix z_fix_cos(unsigned Angle)
{
    return z_fix__sin[z_fix_angleWrap(Angle + Z_DEG_090_INT)];
}

static inline ZFix z_fix_sinf(ZFixu Angle)
{
    return z_fix__sin[z_fix_angleWrap(z_fixu_toInt(Angle))];
}

static inline ZFix z_fix_cosf(ZFixu Angle)
{
    return z_fix__sin[z_fix_angleWrap(z_fixu_toInt(Angle + Z_DEG_090_FIX))];
}

static inline ZFix z_fix_cscf(ZFixu Angle)
{
    return z_fix__csc[z_fix_angleWrap(z_fixu_toInt(Angle))];
}

static inline ZFix z_fix_secf(ZFixu Angle)
{
    return z_fix__csc[z_fix_angleWrap(z_fixu_toInt(Angle + Z_DEG_090_FIX))];
}

extern unsigned z_fix_atan(ZFix X1, ZFix Y1, ZFix X2, ZFix Y2);

extern void z_fix_rotateCounter(ZFix X, ZFix Y, unsigned Angle, ZFix* NewX, ZFix* NewY);
extern void z_fix_rotateClockwise(ZFix X, ZFix Y, unsigned Angle, ZFix* NewX, ZFix* NewY);

typedef struct {
    ZFix x, y;
} ZVectorFix;

typedef struct {
    int x, y;
} ZVectorInt;

static inline ZVectorInt z_vectorfix_toInt(const ZVectorFix Fix)
{
    return (ZVectorInt){z_fix_toInt(Fix.x), z_fix_toInt(Fix.y)};
}

static inline ZVectorFix z_vectorint_toFix(const ZVectorInt Int)
{
    return (ZVectorFix){z_fix_fromInt(Int.x), z_fix_fromInt(Int.y)};
}

static inline bool z_vectorfix_equal(ZVectorFix A, ZVectorFix B)
{
    return A.x == B.x && A.y == B.y;
}

static inline bool z_vectorint_equal(ZVectorInt A, ZVectorInt B)
{
    return A.x == B.x && A.y == B.y;
}

static inline int z_math_min(int X, int Y)
{
    return X < Y ? X : Y;
}

static inline unsigned z_math_minu(unsigned X, unsigned Y)
{
    return X < Y ? X : Y;
}

static inline int z_math_max(int X, int Y)
{
    return X > Y ? X : Y;
}

static inline unsigned z_math_maxu(unsigned X, unsigned Y)
{
    return X > Y ? X : Y;
}

static inline int z_math_clamp(int X, int Min, int Max)
{
    return X < Min ? Min : (X > Max ? Max : X);
}

static inline unsigned z_math_clampu(unsigned X, unsigned Min, unsigned Max)
{
    return X < Min ? Min : (X > Max ? Max : X);
}

static inline int z_math_abs(int X)
{
    return X >= 0 ? X : -X;
}

static inline int z_random_int(int Max)
{
    return rand() % Max;
}

static inline unsigned z_random_intu(unsigned Max)
{
    return (unsigned)rand() % Max;
}

static inline int z_random_range(int Min, int Max)
{
    return Min + (rand() % (Max - Min));
}

static inline unsigned z_random_rangeu(unsigned Min, unsigned Max)
{
    return Min + ((unsigned)rand() % (Max - Min));
}
