// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file exlib.license for terms.

#import <Cocoa/Cocoa.h>

// this file needs the typedef app_main_t :
//   typedef some_app_namespace>::main app_main_t;

@interface GLView : NSOpenGLView {
  app_main_t* m_main;
}
- (id)initWithFrame:(NSRect)rect;
- (void)dealloc;
- (void)drawRect:(NSRect)rect;
- (void)set_main:(app_main_t*)a_main;

@end
