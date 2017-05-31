/*
 * SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008)
 * Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice including the dates of first publication and
 * either this permission notice or a reference to
 * http://oss.sgi.com/projects/FreeB/
 * shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * SILICON GRAPHICS, INC. BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Except as contained in this notice, the name of Silicon Graphics, Inc.
 * shall not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization from
 * Silicon Graphics, Inc.
 */
/*
** Author: Eric Veach, July 1994.
**
*/

#include "memalloc.h"
#include "string.h"

int G_Barrand__gl_memInit( size_t maxFast )
{
#ifndef NO_MALLOPT
/*  mallopt( M_MXFAST, maxFast );*/
#ifdef MEMORY_DEBUG
  mallopt( M_DEBUG, 1 );
#endif
#endif
   return 1;
}

#ifdef MEMORY_DEBUG
void *G_Barrand__gl_memAlloc( size_t n )
{
  return memset( malloc( n ), 0xa5, n );
}
#endif

/*G.Barrand*/
/*
#define INLIB_MEM
*/
#ifdef INLIB_MEM
#else
void* osc__gl_memAlloc(size_t a_size){
  return memset( malloc( a_size ), 0xa5, a_size);
}
void* osc__gl_memRealloc(void* a_ptr,size_t a_size){
  return realloc(a_ptr,a_size);
}
void osc__gl_memFree(void* a_ptr){
  free(a_ptr);
}
#endif

int osc__gl_memInit( size_t maxFast ){
  return 1;
}
