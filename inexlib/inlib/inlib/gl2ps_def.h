/* Copyright (C) 2010, Guy Barrand. All rights reserved. */
/* See the file inlib.license for terms.                 */

#ifndef inlib_gl2ps_def_h
#define inlib_gl2ps_def_h

typedef int            inlib_GLint;
typedef unsigned int   inlib_GLuint;
typedef float          inlib_GLfloat;
typedef unsigned int   inlib_GLenum;
typedef short          inlib_GLshort;
typedef unsigned short inlib_GLushort;
typedef int            inlib_GLsizei;
typedef unsigned char  inlib_GLboolean;

/*----------------------------------------------------------*/
/*---  from gl2ps.h : --------------------------------------*/
/*----------------------------------------------------------*/
#define INLIB_GL2PSDLL_API inline

#define INLIB_GL2PS_MAJOR_VERSION 1
#define INLIB_GL2PS_MINOR_VERSION 4
#define INLIB_GL2PS_PATCH_VERSION 0
#define INLIB_GL2PS_EXTRA_VERSION ""

#define INLIB_GL2PS_VERSION (INLIB_GL2PS_MAJOR_VERSION + \
                       0.01 * INLIB_GL2PS_MINOR_VERSION + \
                       0.0001 * INLIB_GL2PS_PATCH_VERSION)

#define INLIB_GL2PS_COPYRIGHT "(C) 1999-2017 C. Geuzaine"

/* Output file formats (the values and the ordering are important!) */

#define INLIB_GL2PS_PS  0
#define INLIB_GL2PS_EPS 1
#define INLIB_GL2PS_TEX 2
#define INLIB_GL2PS_PDF 3
#define INLIB_GL2PS_SVG 4
#define INLIB_GL2PS_PGF 5

/* Sorting algorithms */

#define INLIB_GL2PS_NO_SORT     1
#define INLIB_GL2PS_SIMPLE_SORT 2
#define INLIB_GL2PS_BSP_SORT    3

/* Message levels and error codes */

#define INLIB_GL2PS_SUCCESS       0
#define INLIB_GL2PS_INFO          1
#define INLIB_GL2PS_WARNING       2
#define INLIB_GL2PS_ERROR         3
#define INLIB_GL2PS_NO_FEEDBACK   4
#define INLIB_GL2PS_OVERFLOW      5
#define INLIB_GL2PS_UNINITIALIZED 6

/* Options for inlib_gl2psBeginPage */

#define INLIB_GL2PS_NONE                 0
#define INLIB_GL2PS_DRAW_BACKGROUND      (1<<0)
#define INLIB_GL2PS_SIMPLE_LINE_OFFSET   (1<<1)
#define INLIB_GL2PS_SILENT               (1<<2)
#define INLIB_GL2PS_BEST_ROOT            (1<<3)
#define INLIB_GL2PS_OCCLUSION_CULL       (1<<4)
#define INLIB_GL2PS_NO_TEXT              (1<<5)
#define INLIB_GL2PS_LANDSCAPE            (1<<6)
#define INLIB_GL2PS_NO_PS3_SHADING       (1<<7)
#define INLIB_GL2PS_NO_PIXMAP            (1<<8)
#define INLIB_GL2PS_USE_CURRENT_VIEWPORT (1<<9)
#define INLIB_GL2PS_COMPRESS             (1<<10)
#define INLIB_GL2PS_NO_BLENDING          (1<<11)
#define INLIB_GL2PS_TIGHT_BOUNDING_BOX   (1<<12)
#define INLIB_GL2PS_NO_OPENGL_CONTEXT    (1<<13)

/* Arguments for inlib_gl2psEnable/inlib_gl2psDisable */

#define INLIB_GL2PS_POLYGON_OFFSET_FILL 1
#define INLIB_GL2PS_POLYGON_BOUNDARY    2
#define INLIB_GL2PS_LINE_STIPPLE        3
#define INLIB_GL2PS_BLEND               4


/* Arguments for inlib_gl2psLineCap/Join */

#define INLIB_GL2PS_LINE_CAP_BUTT       0
#define INLIB_GL2PS_LINE_CAP_ROUND      1
#define INLIB_GL2PS_LINE_CAP_SQUARE     2

#define INLIB_GL2PS_LINE_JOIN_MITER     0
#define INLIB_GL2PS_LINE_JOIN_ROUND     1
#define INLIB_GL2PS_LINE_JOIN_BEVEL     2

/* Text alignment (o=raster position; default mode is BL):
   +---+ +---+ +---+ +---+ +---+ +---+ +-o-+ o---+ +---o
   | o | o   | |   o |   | |   | |   | |   | |   | |   |
   +---+ +---+ +---+ +-o-+ o---+ +---o +---+ +---+ +---+
    C     CL    CR    B     BL    BR    T     TL    TR */

#define INLIB_GL2PS_TEXT_C  1
#define INLIB_GL2PS_TEXT_CL 2
#define INLIB_GL2PS_TEXT_CR 3
#define INLIB_GL2PS_TEXT_B  4
#define INLIB_GL2PS_TEXT_BL 5
#define INLIB_GL2PS_TEXT_BR 6
#define INLIB_GL2PS_TEXT_T  7
#define INLIB_GL2PS_TEXT_TL 8
#define INLIB_GL2PS_TEXT_TR 9

typedef inlib_GLfloat inlib_GL2PSrgba[4];
typedef inlib_GLfloat inlib_GL2PSxyz[3];

typedef struct {
  inlib_GL2PSxyz xyz;
  inlib_GL2PSrgba rgba;
} inlib_GL2PSvertex;

/* Primitive types */
#define INLIB_GL2PS_NO_TYPE          -1
#define INLIB_GL2PS_TEXT             1
#define INLIB_GL2PS_POINT            2
#define INLIB_GL2PS_LINE             3
#define INLIB_GL2PS_QUADRANGLE       4
#define INLIB_GL2PS_TRIANGLE         5
#define INLIB_GL2PS_PIXMAP           6
#define INLIB_GL2PS_IMAGEMAP         7
#define INLIB_GL2PS_IMAGEMAP_WRITTEN 8
#define INLIB_GL2PS_IMAGEMAP_VISIBLE 9
#define INLIB_GL2PS_SPECIAL          10

/*----------------------------------------------------------*/
/*--- from OpenGL : ----------------------------------------*/
/*----------------------------------------------------------*/
#define INLIB_GL_TRUE                           1
#define INLIB_GL_FALSE                          0

#define INLIB_GL_FLOAT				0x1406
#define INLIB_GL_BLEND				0x0BE2

#define INLIB_GL_SRC_ALPHA				0x0302
#define INLIB_GL_ONE_MINUS_SRC_ALPHA                  0x0303

#define INLIB_GL_RGB					0x1907
#define INLIB_GL_RGBA                                 0x1908

#define INLIB_GL_POINTS                               0x0000

#define INLIB_GL_CURRENT_RASTER_POSITION_VALID	0x0B08
#define INLIB_GL_CURRENT_RASTER_POSITION		0x0B07
#define INLIB_GL_CURRENT_RASTER_COLOR			0x0B04
#define INLIB_GL_ZERO					0
#define INLIB_GL_ONE					1
#define INLIB_GL_COLOR_INDEX				0x1900

#define INLIB_GL_POINT_TOKEN				0x0701
#define INLIB_GL_LINE_TOKEN				0x0702
#define INLIB_GL_LINE_RESET_TOKEN			0x0707
#define INLIB_GL_POLYGON_TOKEN			0x0703
#define INLIB_GL_BITMAP_TOKEN				0x0704
#define INLIB_GL_DRAW_PIXEL_TOKEN			0x0705

#define INLIB_GL_COPY_PIXEL_TOKEN			0x0706
#define INLIB_GL_PASS_THROUGH_TOKEN			0x0700

#define INLIB_GL_FEEDBACK				0x1C01
#define INLIB_GL_COLOR_CLEAR_VALUE			0x0C22
#define INLIB_GL_INDEX_CLEAR_VALUE			0x0C20
#define INLIB_GL_RENDER				0x1C00
#define INLIB_GL_VIEWPORT				0x0BA2
#define INLIB_GL_BLEND_SRC				0x0BE1
#define INLIB_GL_BLEND_DST				0x0BE0
#define INLIB_GL_3D_COLOR				0x0602

#define INLIB_GL_POLYGON_OFFSET_FACTOR		0x8038
#define INLIB_GL_POLYGON_OFFSET_UNITS			0x2A00
#define INLIB_GL_LINE_STIPPLE_PATTERN			0x0B25
#define INLIB_GL_LINE_STIPPLE_REPEAT			0x0B26

#define INLIB_GL_ZOOM_X				0x0D16
#define INLIB_GL_ZOOM_Y				0x0D17

/*----------------------------------------------------------*/
/*----------------------------------------------------------*/
/*----------------------------------------------------------*/
typedef inlib_GLboolean (*inlib_glIsEnabled_func)      (inlib_GLenum);
typedef void            (*inlib_glBegin_func)          (inlib_GLenum);
typedef void            (*inlib_glEnd_func)            ();
typedef void            (*inlib_glGetFloatv_func)      (inlib_GLenum,inlib_GLfloat*);
typedef void            (*inlib_glVertex3f_func)       (inlib_GLfloat,inlib_GLfloat,inlib_GLfloat);
typedef void            (*inlib_glGetBooleanv_func)    (inlib_GLenum,inlib_GLboolean*);
typedef void            (*inlib_glGetIntegerv_func)    (inlib_GLenum,inlib_GLint*);
typedef inlib_GLint     (*inlib_glRenderMode_func)     (inlib_GLenum);
typedef void            (*inlib_glFeedbackBuffer_func) (inlib_GLsizei,inlib_GLenum,inlib_GLfloat*);
typedef void            (*inlib_glPassThrough_func)    (inlib_GLfloat);

typedef struct {
  inlib_glIsEnabled_func      m_glIsEnabled;
  inlib_glBegin_func          m_glBegin;
  inlib_glEnd_func            m_glEnd;
  inlib_glGetFloatv_func      m_glGetFloatv;
  inlib_glVertex3f_func       m_glVertex3f;
  inlib_glGetBooleanv_func    m_glGetBooleanv;
  inlib_glGetIntegerv_func    m_glGetIntegerv;
  inlib_glRenderMode_func     m_glRenderMode;
  inlib_glFeedbackBuffer_func m_glFeedbackBuffer;
  inlib_glPassThrough_func    m_glPassThrough;
} inlib_gl2ps_gl_funcs_t;

#endif
