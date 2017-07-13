/*
   glu.h of Mesa-7.8.1 but containing only tess related material.
   We bring also the needed material from gl.h.
 */

#ifndef glutess_src_glu_h
#define glutess_src_glu_h

#include "../glutess/ourex_glutess.h"

/* //////////////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////////////// */

typedef unsigned int	GLenum;
typedef void		GLvoid;
typedef double		GLdouble;	/* double precision float */

typedef unsigned char	GLboolean;
typedef float		GLfloat;	/* single precision float */
typedef double		GLclampd;	/* double precision float in [0,1] */

#ifdef _WIN32
#define GLAPIENTRY __stdcall
#else
#define GLAPIENTRY
#endif
#define GLAPI extern
#define GLAPIENTRYP GLAPIENTRY *

/* Boolean values */
#define GL_FALSE				0x0
#define GL_TRUE					0x1

/* Primitives */
#define GL_LINE_LOOP				0x0002
#define GL_TRIANGLES				0x0004
#define GL_TRIANGLE_STRIP			0x0005
#define GL_TRIANGLE_FAN				0x0006

#define GLU_TESS_MAX_COORD 1.0e150

/* ErrorCode */
#define GLU_INVALID_ENUM                   100900
#define GLU_INVALID_VALUE                  100901
#define GLU_OUT_OF_MEMORY                  100902
#define GLU_INCOMPATIBLE_GL_VERSION        100903
#define GLU_INVALID_OPERATION              100904

/* TessCallback */
#define GLU_TESS_BEGIN                     100100
#define GLU_TESS_VERTEX                    100101
#define GLU_TESS_END                       100102
#define GLU_TESS_ERROR                     100103
#define GLU_TESS_EDGE_FLAG                 100104
#define GLU_EDGE_FLAG                      100104
#define GLU_TESS_COMBINE                   100105
/*
#define GLU_TESS_BEGIN_DATA                100106
#define GLU_TESS_VERTEX_DATA               100107
#define GLU_TESS_END_DATA                  100108
#define GLU_TESS_ERROR_DATA                100109
*/
#define GLU_TESS_EDGE_FLAG_DATA            100110
/*
#define GLU_TESS_COMBINE_DATA              100111
*/

/* TessContour */
#define GLU_CW                             100120
#define GLU_CCW                            100121
#define GLU_INTERIOR                       100122
#define GLU_EXTERIOR                       100123
#define GLU_UNKNOWN                        100124

/* TessProperty */
/*
#define GLU_TESS_WINDING_RULE              100140
*/
#define GLU_TESS_BOUNDARY_ONLY             100141
#define GLU_TESS_TOLERANCE                 100142

/* TessError */
#define GLU_TESS_ERROR1                    100151
#define GLU_TESS_ERROR2                    100152
#define GLU_TESS_ERROR3                    100153
#define GLU_TESS_ERROR4                    100154
#define GLU_TESS_ERROR5                    100155
#define GLU_TESS_ERROR6                    100156
#define GLU_TESS_ERROR7                    100157
#define GLU_TESS_ERROR8                    100158
#define GLU_TESS_MISSING_BEGIN_POLYGON     100151
#define GLU_TESS_MISSING_BEGIN_CONTOUR     100152
#define GLU_TESS_MISSING_END_POLYGON       100153
#define GLU_TESS_MISSING_END_CONTOUR       100154
#define GLU_TESS_COORD_TOO_LARGE           100155
#define GLU_TESS_NEED_COMBINE_CALLBACK     100156

/* TessWinding */
/*
#define GLU_TESS_WINDING_ODD               100130
*/
#define GLU_TESS_WINDING_NONZERO           100131
/*
#define GLU_TESS_WINDING_POSITIVE          100132
#define GLU_TESS_WINDING_NEGATIVE          100133
*/
#define GLU_TESS_WINDING_ABS_GEQ_TWO       100134

#endif /* __glu_h__ */
