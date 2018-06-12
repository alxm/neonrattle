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
    Z_INT_DEG_022 = (Z_ANGLES_NUM / 16),
    Z_INT_DEG_045 = (Z_ANGLES_NUM / 8),
    Z_INT_DEG_067 = (Z_INT_DEG_045 + Z_INT_DEG_022),
    Z_INT_DEG_090 = (2 * Z_INT_DEG_045),
    Z_INT_DEG_112 = (Z_INT_DEG_090 + Z_INT_DEG_022),
    Z_INT_DEG_135 = (3 * Z_INT_DEG_045),
    Z_INT_DEG_157 = (Z_INT_DEG_135 + Z_INT_DEG_022),
    Z_INT_DEG_180 = (4 * Z_INT_DEG_045),
    Z_INT_DEG_202 = (Z_INT_DEG_180 + Z_INT_DEG_022),
    Z_INT_DEG_225 = (5 * Z_INT_DEG_045),
    Z_INT_DEG_247 = (Z_INT_DEG_225 + Z_INT_DEG_022),
    Z_INT_DEG_270 = (6 * Z_INT_DEG_045),
    Z_INT_DEG_292 = (Z_INT_DEG_270 + Z_INT_DEG_022),
    Z_INT_DEG_315 = (7 * Z_INT_DEG_045),
    Z_INT_DEG_337 = (Z_INT_DEG_315 + Z_INT_DEG_022),
    Z_INT_DEG_360 = (8 * Z_INT_DEG_045),
} ZIntAngles;

typedef enum {
    Z_FIX_DEG_022 = (Z_INT_DEG_022 * Z_FIX_ONE),
    Z_FIX_DEG_045 = (Z_INT_DEG_045 * Z_FIX_ONE),
    Z_FIX_DEG_067 = (Z_INT_DEG_067 * Z_FIX_ONE),
    Z_FIX_DEG_090 = (Z_INT_DEG_090 * Z_FIX_ONE),
    Z_FIX_DEG_112 = (Z_INT_DEG_112 * Z_FIX_ONE),
    Z_FIX_DEG_135 = (Z_INT_DEG_135 * Z_FIX_ONE),
    Z_FIX_DEG_157 = (Z_INT_DEG_157 * Z_FIX_ONE),
    Z_FIX_DEG_180 = (Z_INT_DEG_180 * Z_FIX_ONE),
    Z_FIX_DEG_202 = (Z_INT_DEG_202 * Z_FIX_ONE),
    Z_FIX_DEG_225 = (Z_INT_DEG_225 * Z_FIX_ONE),
    Z_FIX_DEG_247 = (Z_INT_DEG_247 * Z_FIX_ONE),
    Z_FIX_DEG_270 = (Z_INT_DEG_270 * Z_FIX_ONE),
    Z_FIX_DEG_292 = (Z_INT_DEG_292 * Z_FIX_ONE),
    Z_FIX_DEG_315 = (Z_INT_DEG_315 * Z_FIX_ONE),
    Z_FIX_DEG_337 = (Z_INT_DEG_337 * Z_FIX_ONE),
    Z_FIX_DEG_360 = (Z_INT_DEG_360 * Z_FIX_ONE),
    Z_FIX_DEG_001 = (Z_FIX_DEG_360 / 360),
} ZFixAngles;

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

static inline unsigned z_fix_wrapAngleInt(unsigned Angle)
{
    return Angle & (Z_ANGLES_NUM - 1);
}

static inline ZFixu z_fix_wrapAngleFix(ZFixu Angle)
{
    return Angle & (Z_ANGLES_NUM * Z_FIX_ONE - 1);
}

static inline ZFix z_fix_sin(unsigned Angle)
{
    return z_fix__sin[z_fix_wrapAngleInt(Angle)];
}

static inline ZFix z_fix_cos(unsigned Angle)
{
    return z_fix__sin[z_fix_wrapAngleInt(Angle + Z_INT_DEG_090)];
}

static inline ZFix z_fix_sinf(ZFixu Angle)
{
    return z_fix__sin[z_fix_wrapAngleInt(z_fixu_toInt(Angle))];
}

static inline ZFix z_fix_cosf(ZFixu Angle)
{
    return z_fix__sin[z_fix_wrapAngleInt(z_fixu_toInt(Angle + Z_FIX_DEG_090))];
}

static inline ZFix z_fix_cscf(ZFixu Angle)
{
    return z_fix__csc[z_fix_wrapAngleInt(z_fixu_toInt(Angle))];
}

static inline ZFix z_fix_secf(ZFixu Angle)
{
    return z_fix__csc[z_fix_wrapAngleInt(z_fixu_toInt(Angle + Z_FIX_DEG_090))];
}

extern unsigned z_fix_atan(ZFix X1, ZFix Y1, ZFix X2, ZFix Y2);

extern void z_fix_rotateCounter(ZFix X, ZFix Y, unsigned Angle, ZFix* NewX, ZFix* NewY);
extern void z_fix_rotateClockwise(ZFix X, ZFix Y, unsigned Angle, ZFix* NewX, ZFix* NewY);

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
