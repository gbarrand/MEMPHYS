#ifndef zlib_ourex_zlib_h
#define zlib_ourex_zlib_h

/* first the same as in zconf.h : */
#  define deflateInit_	ourex_z_deflateInit_
#  define deflate	ourex_z_deflate
#  define deflateEnd	ourex_z_deflateEnd
#  define inflateInit_ 	ourex_z_inflateInit_
#  define inflate	ourex_z_inflate
#  define inflateEnd	ourex_z_inflateEnd
#  define deflateInit2_	ourex_z_deflateInit2_
#  define deflateSetDictionary ourex_z_deflateSetDictionary
#  define deflateCopy	ourex_z_deflateCopy
#  define deflateReset	ourex_z_deflateReset
#  define deflateParams	ourex_z_deflateParams
#  define inflateInit2_	ourex_z_inflateInit2_
#  define inflateSetDictionary ourex_z_inflateSetDictionary
#  define inflateSync	ourex_z_inflateSync
#  define inflateSyncPoint ourex_z_inflateSyncPoint
#  define inflateReset	ourex_z_inflateReset
#  define compress	ourex_z_compress
#  define compress2	ourex_z_compress2
#  define uncompress	ourex_z_uncompress
#  define adler32	ourex_z_adler32
#  define crc32		ourex_z_crc32
#  define get_crc_table ourex_z_get_crc_table

/* don't redefine Byte. Conflict with jni.h.
#  define Byte		ourex_z_Byte
*/

#  define uInt		ourex_z_uInt
#  define uLong		ourex_z_uLong
#  define Bytef	        ourex_z_Bytef
#  define charf		ourex_z_charf
#  define intf		ourex_z_intf
#  define uIntf		ourex_z_uIntf
#  define uLongf	ourex_z_uLongf
#  define voidpf	ourex_z_voidpf
#  define voidp		ourex_z_voidp

#define gzbuffer ourex_zlib_gzbuffer
#define gzclearerr ourex_zlib_gzclearerr
#define gzclose ourex_zlib_gzclose
#define gzclose_r ourex_zlib_gzclose_r
#define gzclose_w ourex_zlib_gzclose_w
#define gzdirect ourex_zlib_gzdirect
#define gzdopen ourex_zlib_gzdopen
#define gzeof ourex_zlib_gzeof
#define gzerror ourex_zlib_gzerror
#define gzflush ourex_zlib_gzflush
#define gzgetc ourex_zlib_gzgetc
#define gzgets ourex_zlib_gzgets
#define gzoffset ourex_zlib_gzoffset
#define gzopen ourex_zlib_gzopen
#define gzprintf ourex_zlib_gzprintf
#define gzputc ourex_zlib_gzputc
#define gzputs ourex_zlib_gzputs
#define gzread ourex_zlib_gzread
#define gzrewind ourex_zlib_gzrewind
#define gzseek ourex_zlib_gzseek
#define gzsetparams ourex_zlib_gzsetparams
#define gztell ourex_zlib_gztell
#define gzungetc ourex_zlib_gzungetc
#define gzwrite ourex_zlib_gzwrite

/*
#define adler32_combine ourex_zlib_adler32_combine
#define compressBound ourex_zlib_compressBound
#define crc32_combine ourex_zlib_crc32_combine
#define deflateBound ourex_zlib_deflateBound
#define deflatePrime ourex_zlib_deflatePrime
#define deflateSetHeader ourex_zlib_deflateSetHeader
#define deflateTune ourex_zlib_deflateTune
#define get_crc_table ourex_zlib_get_crc_table
#define inflateBack ourex_zlib_inflateBack
#define inflateBackEnd ourex_zlib_inflateBackEnd
#define inflateBackInit_ ourex_zlib_inflateBackInit_
#define inflateCopy ourex_zlib_inflateCopy
#define inflateGetHeader ourex_zlib_inflateGetHeader
#define inflateMark ourex_zlib_inflateMark
#define inflatePrime ourex_zlib_inflatePrime
#define inflateReset ourex_zlib_inflateReset
#define inflateReset2 ourex_zlib_inflateReset2
#define inflateUndermine ourex_zlib_inflateUndermine
*/

/* to avoid clash with libG4zlib : */
#define _dist_code       ourex_zlib__dist_code
#define _length_code     ourex_zlib__length_code
#define z_errmsg         ourex_zlib_zerrmsg
#define zError           ourex_zlib_zError
#define zlibCompileFlags ourex_zlib_zlibCompileFlags
#define zlibVersion      ourex_zlib_zlibVersion

#include "_zlib.h"

#endif
