/* Copyright (C) 2010, Guy Barrand. All rights reserved. */
/* See the file inlib.license for terms.                 */

#ifndef gldum_gldef_h
#define gldum_gldef_h

typedef unsigned int   GLenum;
typedef double         GLdouble;
typedef float          GLfloat;
typedef int            GLint;
typedef short          GLshort;
typedef int            GLsizei;
typedef unsigned int   GLbitfield;
typedef float          GLclampf;
typedef double         GLclampd;
typedef unsigned int   GLuint;
typedef unsigned char  GLubyte;
typedef unsigned char  GLboolean;
typedef unsigned short GLushort;
typedef void           GLvoid;

#define GL_TRUE                           1
#define GL_FALSE                          0
#define GL_NO_ERROR GL_FALSE

#define GL_MODELVIEW 1
#define GL_PROJECTION 1
#define GL_LIGHTING 1
#define GL_LIGHT0 1
#define GL_DEPTH_TEST 1
#define GL_CULL_FACE 1
#define GL_RGB 1
#define GL_FLAT	1
#define GL_NORMALIZE 1
#define GL_COLOR_BUFFER_BIT 1
#define GL_DEPTH_BUFFER_BIT 1

#define GL_FLOAT 1
#define GL_COLOR_MATERIAL 1
#define GL_BLEND 1
#define GL_SRC_ALPHA 1
#define GL_DST_ALPHA 1
#define GL_ONE_MINUS_SRC_ALPHA 1
#define GL_CCW 1
#define GL_CW 1
#define GL_POLYGON_OFFSET_FILL 1
#define GL_TEXTURE_2D 1
#define GL_PACK_ALIGNMENT 1
#define GL_UNSIGNED_BYTE 1
#define GL_POLYGON_OFFSET_FILL 1
#define GL_VERTEX_ARRAY 1
#define GL_COLOR_ARRAY 1
#define GL_CONSTANT_COLOR 1

#define GL_NORMAL_ARRAY 1
#define GL_MAX_LIGHTS 1
#define GL_POSITION 1
#define GL_DIFFUSE 1
#define GL_SPECULAR 1
#define GL_AMBIENT 1
#define GL_SPOT_EXPONENT 1
#define GL_SPOT_CUTOFF 1
#define GL_CONSTANT_ATTENUATION 1
#define GL_LINEAR_ATTENUATION 1
#define GL_QUADRATIC_ATTENUATION 1

#define GL_LUMINANCE 1

/* for gl2ps : */
#define GL_RGBA                           0x1908

#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP		  0x0005
#define GL_TRIANGLE_FAN			  0x0006

/* for ourex/source/gl2ps.c compiled with -DOUREX_GL2PS_NO_GL : */
#define GL_CURRENT_RASTER_POSITION_VALID	0x0B08
#define GL_CURRENT_RASTER_POSITION		0x0B07
#define GL_CURRENT_RASTER_COLOR			0x0B04
#define GL_ZERO					0x0
#define GL_ONE					0x1
#define GL_COLOR_INDEX				0x1900
#define GL_POINT_TOKEN				0x0701
#define GL_LINE_TOKEN				0x0702
#define GL_LINE_RESET_TOKEN			0x0707
#define GL_POLYGON_TOKEN			0x0703
#define GL_BITMAP_TOKEN				0x0704
#define GL_DRAW_PIXEL_TOKEN			0x0705
#define GL_COPY_PIXEL_TOKEN			0x0706
#define GL_PASS_THROUGH_TOKEN			0x0700
#define GL_FEEDBACK				0x1C01
#define GL_COLOR_CLEAR_VALUE			0x0C22
#define GL_INDEX_CLEAR_VALUE			0x0C20
#define GL_RENDER				0x1C00
#define GL_VIEWPORT				0x0BA2
#define GL_BLEND_SRC				0x0BE1
#define GL_BLEND_DST				0x0BE0
#define GL_3D_COLOR				0x0602
#define GL_POLYGON_OFFSET_FACTOR		0x8038
#define GL_POLYGON_OFFSET_UNITS			0x2A00
#define GL_LINE_STIPPLE_PATTERN			0x0B25
#define GL_LINE_STIPPLE_REPEAT			0x0B26

/* for lego_batch : */
#define GL_TEXTURE_COORD_ARRAY 1
#define GL_TEXTURE_MAG_FILTER  1
#define GL_TEXTURE_MIN_FILTER  1
#define GL_LINEAR  1

#endif  /*gldef_h*/



