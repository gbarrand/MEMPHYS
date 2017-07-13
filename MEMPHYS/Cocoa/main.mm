// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file MEMPHYS.license for terms.

#define pi messy_pi
#define perThousand messy_perThousand

#import <Cocoa/Cocoa.h>

#undef pi
#undef perThousand
#undef pascal
#undef negativeInfinity

#include "../MEMPHYS/main"

typedef MEMPHYS::main app_main_t;

#import <exlib/app/Cocoa/main_mm>

int main(int argc,char** argv) {return exlib_main<MEMPHYS::context,MEMPHYS::main>("MEMPHYS",argc,argv);}
