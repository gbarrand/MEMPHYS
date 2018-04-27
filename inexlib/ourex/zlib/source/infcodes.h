/* infcodes.h -- header to use infcodes.c
 * Copyright (C) 1995-2002 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h 
 */

/* WARNING: this file should *not* be used by applications. It is
   part of the implementation of the compression library and is
   subject to change. Applications should only use zlib.h.
 */

/* G.Barrand : */
#define inflate_block         ourex_z_inflate_block
#define inflate_codes         ourex_z_inflate_codes
#define inflate_codes_free    ourex_z_inflate_codes_free
#define inflate_codes_new     ourex_z_inflate_codes_new
#define inflate_fast          ourex_z_inflate_fast
#define inflate_flush         ourex_z_inflate_flush
#define inflate_mask          ourex_z_inflate_mask

struct inflate_codes_state;
typedef struct inflate_codes_state FAR inflate_codes_statef;

extern inflate_codes_statef *inflate_codes_new OF((
    uInt, uInt,
    inflate_huft *, inflate_huft *,
    z_streamp ));

extern int inflate_codes OF((
    inflate_blocks_statef *,
    z_streamp ,
    int));

extern void inflate_codes_free OF((
    inflate_codes_statef *,
    z_streamp ));

