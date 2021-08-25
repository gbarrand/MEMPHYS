/* Copyright (C) 2010, Guy Barrand. All rights reserved. */
/* See the file inlib.license for terms.                 */

#ifndef inlib_gl2ps_def_h
#define inlib_gl2ps_def_h

typedef int            GLint;
typedef unsigned int   GLuint;
typedef float          GLfloat;
typedef unsigned int   GLenum;
typedef short          GLshort;
typedef unsigned short GLushort;
typedef int            GLsizei;
typedef unsigned char  GLboolean;

/*//////////////////////////////////////////////////////////*/
/*/// from gl2ps.h : ///////////////////////////////////////*/
/*//////////////////////////////////////////////////////////*/
#define GL2PSDLL_API inline

#define GL2PS_MAJOR_VERSION 1
#define GL2PS_MINOR_VERSION 4
#define GL2PS_PATCH_VERSION 0
#define GL2PS_EXTRA_VERSION ""

#define GL2PS_VERSION (GL2PS_MAJOR_VERSION + \
                       0.01 * GL2PS_MINOR_VERSION + \
                       0.0001 * GL2PS_PATCH_VERSION)

#define GL2PS_COPYRIGHT "(C) 1999-2017 C. Geuzaine"

/* Output file formats (the values and the ordering are important!) */

#define GL2PS_PS  0
#define GL2PS_EPS 1
#define GL2PS_TEX 2
#define GL2PS_PDF 3
#define GL2PS_SVG 4
#define GL2PS_PGF 5

/* Sorting algorithms */

#define GL2PS_NO_SORT     1
#define GL2PS_SIMPLE_SORT 2
#define GL2PS_BSP_SORT    3

/* Message levels and error codes */

#define GL2PS_SUCCESS       0
#define GL2PS_INFO          1
#define GL2PS_WARNING       2
#define GL2PS_ERROR         3
#define GL2PS_NO_FEEDBACK   4
#define GL2PS_OVERFLOW      5
#define GL2PS_UNINITIALIZED 6

/* Options for gl2psBeginPage */

#define GL2PS_NONE                 0
#define GL2PS_DRAW_BACKGROUND      (1<<0)
#define GL2PS_SIMPLE_LINE_OFFSET   (1<<1)
#define GL2PS_SILENT               (1<<2)
#define GL2PS_BEST_ROOT            (1<<3)
#define GL2PS_OCCLUSION_CULL       (1<<4)
#define GL2PS_NO_TEXT              (1<<5)
#define GL2PS_LANDSCAPE            (1<<6)
#define GL2PS_NO_PS3_SHADING       (1<<7)
#define GL2PS_NO_PIXMAP            (1<<8)
#define GL2PS_USE_CURRENT_VIEWPORT (1<<9)
#define GL2PS_COMPRESS             (1<<10)
#define GL2PS_NO_BLENDING          (1<<11)
#define GL2PS_TIGHT_BOUNDING_BOX   (1<<12)
#define GL2PS_NO_OPENGL_CONTEXT    (1<<13)

/* Arguments for gl2psEnable/gl2psDisable */

#define GL2PS_POLYGON_OFFSET_FILL 1
#define GL2PS_POLYGON_BOUNDARY    2
#define GL2PS_LINE_STIPPLE        3
#define GL2PS_BLEND               4


/* Arguments for gl2psLineCap/Join */

#define GL2PS_LINE_CAP_BUTT       0
#define GL2PS_LINE_CAP_ROUND      1
#define GL2PS_LINE_CAP_SQUARE     2

#define GL2PS_LINE_JOIN_MITER     0
#define GL2PS_LINE_JOIN_ROUND     1
#define GL2PS_LINE_JOIN_BEVEL     2

/* Text alignment (o=raster position; default mode is BL):
   +---+ +---+ +---+ +---+ +---+ +---+ +-o-+ o---+ +---o
   | o | o   | |   o |   | |   | |   | |   | |   | |   |
   +---+ +---+ +---+ +-o-+ o---+ +---o +---+ +---+ +---+
    C     CL    CR    B     BL    BR    T     TL    TR */

#define GL2PS_TEXT_C  1
#define GL2PS_TEXT_CL 2
#define GL2PS_TEXT_CR 3
#define GL2PS_TEXT_B  4
#define GL2PS_TEXT_BL 5
#define GL2PS_TEXT_BR 6
#define GL2PS_TEXT_T  7
#define GL2PS_TEXT_TL 8
#define GL2PS_TEXT_TR 9

typedef GLfloat GL2PSrgba[4];
typedef GLfloat GL2PSxyz[3];

typedef struct {
  GL2PSxyz xyz;
  GL2PSrgba rgba;
} GL2PSvertex;

/* Primitive types */
#define GL2PS_NO_TYPE          -1
#define GL2PS_TEXT             1
#define GL2PS_POINT            2
#define GL2PS_LINE             3
#define GL2PS_QUADRANGLE       4
#define GL2PS_TRIANGLE         5
#define GL2PS_PIXMAP           6
#define GL2PS_IMAGEMAP         7
#define GL2PS_IMAGEMAP_WRITTEN 8
#define GL2PS_IMAGEMAP_VISIBLE 9
#define GL2PS_SPECIAL          10

/*//////////////////////////////////////////////////////////*/
/*/// from OpenGL : ////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////*/
#define GL_TRUE                           1
#define GL_FALSE                          0

#define GL_FLOAT				0x1406
#define GL_BLEND				0x0BE2

#define GL_SRC_ALPHA				0x0302
#define GL_ONE_MINUS_SRC_ALPHA                  0x0303

#define GL_RGB					0x1907
#define GL_RGBA                                 0x1908

#define GL_POINTS                               0x0000

#define GL_CURRENT_RASTER_POSITION_VALID	0x0B08
#define GL_CURRENT_RASTER_POSITION		0x0B07
#define GL_CURRENT_RASTER_COLOR			0x0B04
#define GL_ZERO					0
#define GL_ONE					1
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

#define GL_ZOOM_X				0x0D16
#define GL_ZOOM_Y				0x0D17

/*//////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////*/
/*//////////////////////////////////////////////////////////*/
typedef struct {
  GLboolean (*m_glIsEnabled)      (GLenum);
  void      (*m_glBegin)          (GLenum);
  void      (*m_glEnd)            ();
  void      (*m_glGetFloatv)      (GLenum,GLfloat*);
  void      (*m_glVertex3f)       (GLfloat,GLfloat,GLfloat);
  void      (*m_glGetBooleanv)    (GLenum,GLboolean*);
  void      (*m_glGetIntegerv)    (GLenum,GLint*);
  GLint     (*m_glRenderMode)     (GLenum);
  void      (*m_glFeedbackBuffer) (GLsizei,GLenum,GLfloat*);
  void      (*m_glPassThrough)    (GLfloat);
} gl2ps_gl_funcs_t;

#endif
