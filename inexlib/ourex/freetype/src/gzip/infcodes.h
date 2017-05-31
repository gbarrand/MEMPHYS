/* infcodes.h -- header to use infcodes.c
 * Copyright (C) 1995-2002 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */

#ifndef _INFCODES_H
#define _INFCODES_H

/* G.Barrand : */
#define inflate_blocks            ourex_freetype2_inflate_blocks
#define inflate_blocks_free       ourex_freetype2_inflate_blocks_free
#define inflate_blocks_new        ourex_freetype2_inflate_blocks_new
#define inflate_blocks_reset      ourex_freetype2_inflate_blocks_reset
#define inflate_codes             ourex_freetype2_inflate_codes
#define inflate_codes_free        ourex_freetype2_inflate_codes_free
#define inflate_codes_new         ourex_freetype2_inflate_codes_new

#define inflate_copyright         ourex_freetype2_inflate_copyright
#define inflate_fast              ourex_freetype2_inflate_fast
#define inflate_flush             ourex_freetype2_inflate_flush
#define inflate_mask              ourex_freetype2_inflate_mask
#define inflate_trees_bits        ourex_freetype2_inflate_trees_bits
#define inflate_trees_dynamic     ourex_freetype2_inflate_trees_dynamic
#define inflate_trees_fixed       ourex_freetype2_inflate_trees_fixed

struct inflate_codes_state;
typedef struct inflate_codes_state FAR inflate_codes_statef;

local inflate_codes_statef *inflate_codes_new OF((
    uInt, uInt,
    inflate_huft *, inflate_huft *,
    z_streamp ));

local int inflate_codes OF((
    inflate_blocks_statef *,
    z_streamp ,
    int));

local void inflate_codes_free OF((
    inflate_codes_statef *,
    z_streamp ));

#endif /* _INFCODES_H */
