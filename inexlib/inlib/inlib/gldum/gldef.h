/* Copyright (C) 2010, Guy Barrand. All rights reserved. */
/* See the file inlib.license for terms.                 */

#ifndef inlib_gldum_gldef_h
#define inlib_gldum_gldef_h

/* GL-ES defs, typedefs to pass exlib GL-ES related : */

typedef unsigned int   GLenum;
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

#define GL_RGBA                           0x1908
#define GL_VERSION          1
#define GL_VENDOR           1
#define GL_RENDERER         1
#define GL_EXTENSIONS       1
#define GL_MAX_TEXTURE_SIZE 1
#define GL_POINT_SMOOTH     1
#define GL_LINE_SMOOTH      1
#define GL_TEXTURE_COORD_ARRAY 1
#define GL_TEXTURE_MAG_FILTER  1
#define GL_TEXTURE_MIN_FILTER  1
#define GL_LINEAR  1
#define GL_NEAREST 1
#define GL_MULTISAMPLE      1
#define GL_POLYGON_STIPPLE  1

#define GL_TRIANGLE_FAN			  0x0006

#endif



