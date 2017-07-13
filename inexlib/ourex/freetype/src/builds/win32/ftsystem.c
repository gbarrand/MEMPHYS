/* 
   Copy of the unix/ftsystem.c.
   Have a simple implementation of FT_Stream_Open that uses 
   fopen, fread, fclose to read at once the file in memory.

     G.Barrand 22/02/2006
 */


/***************************************************************************/
/*                                                                         */
/*  ftsystem.c                                                             */
/*                                                                         */
/*    Unix-specific FreeType low-level system interface (body).            */
/*                                                                         */
/*  Copyright 1996-2001, 2002, 2004 by                                     */
/*  David Turner, Robert Wilhelm, and Werner Lemberg.                      */
/*                                                                         */
/*  This file is part of the FreeType project, and may only be used,       */
/*  modified, and distributed under the terms of the FreeType project      */
/*  license, LICENSE.TXT.  By continuing to use, modify, or distribute     */
/*  this file you indicate that you have read the license and              */
/*  understand and accept it fully.                                        */
/*                                                                         */
/***************************************************************************/


#include "../../include/ourex_ft2build.h"
/*G.Barrand #include <ftconfig.h> */
#include FT_INTERNAL_DEBUG_H
#include FT_SYSTEM_H
#include FT_ERRORS_H
#include FT_TYPES_H
#include FT_INTERNAL_OBJECTS_H

  /* memory-mapping includes and definitions */
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

  /*************************************************************************/
  /*                                                                       */
  /*                       MEMORY MANAGEMENT INTERFACE                     */
  /*                                                                       */
  /*************************************************************************/


  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    ft_alloc                                                           */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The memory allocation function.                                    */
  /*                                                                       */
  /* <Input>                                                               */
  /*    memory :: A pointer to the memory object.                          */
  /*                                                                       */
  /*    size   :: The requested size in bytes.                             */
  /*                                                                       */
  /* <Return>                                                              */
  /*    The address of newly allocated block.                              */
  /*                                                                       */
  FT_CALLBACK_DEF( void* )
  ft_alloc( FT_Memory  memory,
            long       size )
  {
    FT_UNUSED( memory );

    return malloc( size );
  }


  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    ft_realloc                                                         */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The memory reallocation function.                                  */
  /*                                                                       */
  /* <Input>                                                               */
  /*    memory   :: A pointer to the memory object.                        */
  /*                                                                       */
  /*    cur_size :: The current size of the allocated memory block.        */
  /*                                                                       */
  /*    new_size :: The newly requested size in bytes.                     */
  /*                                                                       */
  /*    block    :: The current address of the block in memory.            */
  /*                                                                       */
  /* <Return>                                                              */
  /*    The address of the reallocated memory block.                       */
  /*                                                                       */
  FT_CALLBACK_DEF( void* )
  ft_realloc( FT_Memory  memory,
              long       cur_size,
              long       new_size,
              void*      block )
  {
    FT_UNUSED( memory );
    FT_UNUSED( cur_size );

    return realloc( block, new_size );
  }

  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    ft_free                                                            */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The memory release function.                                       */
  /*                                                                       */
  /* <Input>                                                               */
  /*    memory :: A pointer to the memory object.                          */
  /*                                                                       */
  /*    block  :: The address of block in memory to be freed.              */
  /*                                                                       */
  FT_CALLBACK_DEF( void )
  ft_free( FT_Memory  memory,
           void*      block )
  {
    FT_UNUSED( memory );

    free( block );
  }

  /*************************************************************************/
  /*                                                                       */
  /* <Function>                                                            */
  /*    ft_close_stream_by_free                                            */
  /*                                                                       */
  /* <Description>                                                         */
  /*    The function to close a stream which is created by ft_alloc.       */
  /*                                                                       */
  /* <Input>                                                               */
  /*    stream :: A pointer to the stream object.                          */
  /*                                                                       */
  FT_CALLBACK_DEF( void )
  ft_close_stream_by_free( FT_Stream  stream )
  {
    ft_free( NULL, stream->descriptor.pointer );

    stream->descriptor.pointer = NULL;
    stream->size               = 0;
    stream->base               = 0;
  }

  FT_EXPORT_DEF( FT_Error )
  FT_Stream_Open( FT_Stream    stream,
                  const char*  filepathname )
  {
    FILE* file;
    long filesize,length;
    char* buff;
    int nitem;

    file = fopen(filepathname,"rb");
    if(!file) return FT_Err_Cannot_Open_Stream;
    /* Get file size : */
    fseek(file,0L,SEEK_END);
    filesize = ftell(file);
    if(!filesize) {
      fclose(file);
      return FT_Err_Cannot_Open_Stream;
    }
    buff = (char*)ft_alloc( NULL, filesize*sizeof(char) );
    if(buff==NULL) {
      fclose(file);
      return FT_Err_Cannot_Open_Stream;
    }
    rewind(file);
    nitem = fread((char*)buff,(size_t)filesize,(size_t)1,file);
    if(nitem!=1){
      free(buff);
      fclose(file);
      return FT_Err_Cannot_Open_Stream;
    }
    fclose(file);
    length = filesize;

    stream->base = (FT_Byte*)buff;
    stream->size = length;
    stream->pos  = 0;
    stream->descriptor.pointer = stream->base;
    stream->pathname.pointer   = (char*)filepathname;
    stream->read = 0;
    stream->close = ft_close_stream_by_free;
    /* stream->memory */
    /* stream->cursor */
    /* stream->limit */

    return FT_Err_Ok;
  }


#ifdef FT_DEBUG_MEMORY

  extern FT_Int
  ft_mem_debug_init( FT_Memory  memory );

  extern void
  ft_mem_debug_done( FT_Memory  memory );

#endif


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( FT_Memory )
  FT_New_Memory( void )
  {
    FT_Memory  memory;


    memory = (FT_Memory)malloc( sizeof ( *memory ) );
    if ( memory )
    {
      memory->user    = 0;
      memory->alloc   = ft_alloc;
      memory->realloc = ft_realloc;
      memory->free    = ft_free;
#ifdef FT_DEBUG_MEMORY
      ft_mem_debug_init( memory );
#endif
    }

    return memory;
  }


  /* documentation is in ftobjs.h */

  FT_EXPORT_DEF( void )
  FT_Done_Memory( FT_Memory  memory )
  {
#ifdef FT_DEBUG_MEMORY
    ft_mem_debug_done( memory );
#endif
    memory->free( memory, memory );
  }


/* END */
