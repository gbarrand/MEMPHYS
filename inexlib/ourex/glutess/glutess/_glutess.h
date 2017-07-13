/*
   glu.h of Mesa-7.8.1 but containing only tess related material.
   We bring also the needed material from gl.h.
 */

#ifndef glutess_glutess_h
#define glutess_glutess_h

/* G.Barrand : avoid gl.h */

#include "rename.h" /* G.Barrand */

/* //////////////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////////////// */

/* G.Barrand : have GLU... */
typedef unsigned int	GLUenum;
typedef void		GLUvoid;
typedef double		GLUdouble;

/* //////////////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////////////// */
/* //////////////////////////////////////////////////////////////// */

/* G.Barrand : have GLU... */
#ifdef _WIN32
#define GLUAPIENTRY __stdcall
#else
#define GLUAPIENTRY
#endif
#define GLUAPI extern
/*G.Barrand : SWIG : #define GLUAPIENTRYP GLUAPIENTRY * */

#ifdef __cplusplus
extern "C" {
#endif

/*************************************************************/
/* G.Barrand : have here only the cpp macros used in exlib. */

/* TessCallback */
#define GLU_TESS_END                       100102
#define GLU_TESS_BEGIN_DATA                100106
#define GLU_TESS_VERTEX_DATA               100107
#define GLU_TESS_END_DATA                  100108
#define GLU_TESS_ERROR_DATA                100109
#define GLU_TESS_COMBINE_DATA              100111

/* TessProperty */
#define GLU_TESS_WINDING_RULE              100140
/*
#define GLU_TESS_TOLERANCE                 100142
*/

/* TessWinding */
#define GLU_TESS_WINDING_ODD               100130
#define GLU_TESS_WINDING_POSITIVE          100132
#define GLU_TESS_WINDING_NEGATIVE          100133

/*************************************************************/


#ifdef __cplusplus
class GLUtesselator;
#else
typedef struct GLUtesselator GLUtesselator;
#endif

typedef GLUtesselator GLUtesselatorObj;
typedef GLUtesselator GLUtriangulatorObj;

/* Internal convenience typedefs */
/*G.Barrand : SWIG : typedef void (GLUAPIENTRYP _GLUfuncptr)(); */
#ifdef _WIN32
typedef void (__stdcall* _GLUfuncptr)();
#else
typedef void (* _GLUfuncptr)();
#endif

GLUAPI GLUtesselator* GLUAPIENTRY gluNewTess (void);
GLUAPI void GLUAPIENTRY gluDeleteTess (GLUtesselator* tess);

GLUAPI void GLUAPIENTRY gluTessBeginContour (GLUtesselator* tess);
GLUAPI void GLUAPIENTRY gluTessBeginPolygon (GLUtesselator* tess, GLUvoid* data);
GLUAPI void GLUAPIENTRY gluTessCallback (GLUtesselator* tess, GLUenum which, _GLUfuncptr CallBackFunc);
GLUAPI void GLUAPIENTRY gluTessEndContour (GLUtesselator* tess);
GLUAPI void GLUAPIENTRY gluTessEndPolygon (GLUtesselator* tess);
GLUAPI void GLUAPIENTRY gluTessNormal (GLUtesselator* tess, GLUdouble valueX, GLUdouble valueY, GLUdouble valueZ);
GLUAPI void GLUAPIENTRY gluTessProperty (GLUtesselator* tess, GLUenum which, GLUdouble data);
GLUAPI void GLUAPIENTRY gluTessVertex (GLUtesselator* tess, GLUdouble *location, GLUvoid* data);

#ifdef __cplusplus
}
#endif

#endif
