/* Copyright (C) 2010, Guy Barrand. All rights reserved. */
/* See the file inlib.license for terms.                 */

#ifndef inlib_gldum_gl_h
#define inlib_gldum_gl_h

#include "gldef.h"

/* GL-ES functions to pass exlib GL-ES related : */

#ifdef __cplusplus
extern "C"{
#endif

inline GLboolean glIsEnabled(GLenum a_0){(void)a_0;return GL_FALSE;}

inline void glPointSize(GLfloat a_0){(void)a_0;}
inline void glClearColor(GLclampf a_0,GLclampf a_1,GLclampf a_2,GLclampf a_3){
  (void)a_0;(void)a_1;(void)a_2;(void)a_3;
}
inline void glReadPixels(GLint a_0,GLint a_1,GLsizei a_2,GLsizei a_3,GLenum a_4,GLenum a_5,GLvoid* a_6){
  (void)a_0;(void)a_1;(void)a_2;(void)a_3;(void)a_4;(void)a_5;(void)a_6;
}
inline void glViewport(GLint a_0,GLint a_1,GLsizei a_2,GLsizei a_3){
  (void)a_0;(void)a_1;(void)a_2;(void)a_3;
}
inline void glPixelStorei(GLenum a_0,GLint a_1){
  (void)a_0;(void)a_1;
}
inline void glShadeModel(GLenum a_0){(void)a_0;}
inline void glClear(GLbitfield a_0){(void)a_0;}
inline void glFinish(){}
inline void glMatrixMode(GLenum a_0){(void)a_0;}
inline void glLoadIdentity(){}
inline void glLineWidth(GLfloat a_0){(void)a_0;}
  /*inline void glNormal3f(GLfloat a_0,GLfloat a_1,GLfloat a_2){
  (void)a_0;(void)a_1;(void)a_2;
  }*/
inline void glEnable(GLenum a_0){(void)a_0;}
inline void glDisable(GLenum a_0){(void)a_0;}
inline void glLoadMatrixf(const GLfloat* a_0){(void)a_0;}
inline GLenum glGetError(){return GL_NO_ERROR;}
inline void glFrontFace(GLenum a_0){(void)a_0;}
  /*inline void glColor4f(GLfloat a_0,GLfloat a_1,GLfloat a_2,GLfloat a_3){
  (void)a_0;(void)a_1;(void)a_2;(void)a_3;
  }*/
inline void glBlendFunc(GLenum a_0,GLenum a_1){
  (void)a_0;(void)a_1;
}
inline void glVertexPointer(GLint a_0,GLenum a_1,GLsizei a_2,const GLvoid* a_3){
  (void)a_0;(void)a_1;(void)a_2;(void)a_3;
}
inline void glColorPointer(GLint a_0,GLenum a_1,GLsizei a_2,const GLvoid* a_3){
  (void)a_0;(void)a_1;(void)a_2;(void)a_3;
}
inline void glDrawArrays(GLenum a_0,GLint a_1,GLsizei a_2){
  (void)a_0;(void)a_1;(void)a_2;
}
inline void glEnableClientState(GLenum a_0){(void)a_0;}
inline void glDisableClientState(GLenum a_0){(void)a_0;}
inline void glPolygonOffset(GLfloat a_0,GLfloat a_1){
  (void)a_0;(void)a_1;
}
inline void glTexImage2D(GLenum a_0,GLint a_1,GLint a_2,GLsizei a_3,GLsizei a_4,GLint a_5,GLenum a_6,GLenum a_7,const GLvoid* a_8){
  (void)a_0;(void)a_1;(void)a_2;(void)a_3;(void)a_4;(void)a_5;(void)a_6;(void)a_7;(void)a_8;
}

inline const GLubyte* glGetString(GLenum a_name) {(void)a_name;return 0;}
inline void glGetIntegerv(GLenum a_0,GLint* a_1){(void)a_0;(void)a_1;}
inline void glBindTexture(GLenum a_0,GLuint a_1){(void)a_0;(void)a_1;}
inline void glTexCoordPointer(GLint a_0,GLenum a_1,GLsizei a_2,const GLvoid* a_3){(void)a_0;(void)a_1;(void)a_2;(void)a_3;}
inline void glDeleteTextures(GLsizei a_0,const GLuint* a_1){(void)a_0;(void)a_1;}
inline GLboolean glIsTexture(GLuint a_texture) {(void)a_texture;return 0;}
inline void glColor4f(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha ) {}
inline void glNormal3f(GLfloat nx,GLfloat ny,GLfloat nz ) {}
inline void glNormalPointer(GLenum a_0,GLsizei a_1,const GLvoid* a_2){(void)a_0;(void)a_1;(void)a_2;}
inline void glLightfv(GLenum a_0,GLenum a_1,const GLfloat* a_2){(void)a_0;(void)a_1;(void)a_2;}
inline void glLightf(GLenum a_0,GLenum a_1,GLfloat a_2){(void)a_0;(void)a_1;(void)a_2;}
inline void glTexParameterf(GLenum a_0,GLenum a_1,GLfloat a_2){(void)a_0;(void)a_1;(void)a_2;}
inline void glGenTextures(GLsizei a_0,GLuint* a_1){(void)a_0;(void)a_1;}

#ifdef __cplusplus
}
#endif

#endif
