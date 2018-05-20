/**

@page bush_usage <h1>Usage</h1>

  On the source code of a softinex app, there is the "mgr" (for manager) directory to build with bush scripts. In general there
 is a main "build" script that triggers the build. Binaries are put under the mgr/bin_[config] directory (for exa bin_gnu on
 a Linux), and for an app the result of the build is in general a :
@verbatim
    bin_<config>/distrib/<app>-<version>-<platform>.zip
@endverbatim
 containing a binary distribution.

  By default, the build is silent. If all is ok, bush scripts say nothing, but you can have a coarse graining traceback with :
@verbatim
    ./build -v
 or a full verbose one with :
    ./build -x
@endverbatim
 The "-v" shows the files that are compiled, lib that are created, and app executable which are linked.

@section bush_usage_tweak Tweak things

 In general in softinex we arrange to minimize the number of "externals", but as they aer hard to avoid we have some logic
 to deal with them. The "access" to an external (mainly the -I to find the includes, and the -L to find the lib), is put in
 a "use" file which is under bush/use. For example use/X11 to compile and link a X11 application, etc...
@verbatim
    bush/use/<package>
 for example :
    bush/use/X11, bush/use/GL
    bush/use/geant4
@endverbatim
  If having a problem to access a package, it is here that you have to look and tweak things. As some "externals" are coming from
 our distrib (then put under ourex/package in a softinex source distribution), you may fall in their "use" file to some logic
 "use_sys_package" or "use_our_package" to choose between the ourex one or the one of the system. In bush we give priority to
 an "ourex" source. ("ourex" is for "our distribution of an external").

  A critical point are the compiler and link commands. If having problem with them, the places to look are the two files :
@verbatim
    bush/use_cpp  # for C++
    bush/use_cc   # for C
  ( bush/use_f77  # for what? FORTRAN. Yes there is still some legacy around. )
@endverbatim

  A couple of things tied to the system, as the shared lib or plugin extension are put in :
@verbatim
    bush/use_sys
@endverbatim

@section bush_usage_app An app bush build file

  For example, the pmx/mgr/build file looks like :
@verbatim
#!/bin/sh -f

bush_home=../../bush            # where to find bush files.

. ${bush_home}/args             # put args concerning bush in a build_args variable.
. ${bush_home}/app_header_use   # analyse the build_args, decide the compiler, the name of the bin directory, etc...

#//////////////////////////////////////////////////////
#//////////////////////////////////////////////////////
#//////////////////////////////////////////////////////
if [ ${build_visual} = "yes" ] ; then
  cppflags="${cppflags} -bigobj" # some extra local flags. Here one for VisualC++.
fi

#//////////////////////////////////////////////////////
#// the used external packages : //////////////////////
#//////////////////////////////////////////////////////

use_csz=yes

. ${bush_home}/use/csz

. ${bush_home}/app_trailer_use   # it calls a set of common  ". ${bush_home}/use/<package>" for graphics.

#//////////////////////////////////////////////////////
#//////////////////////////////////////////////////////
#//////////////////////////////////////////////////////

. ${bush_home}/app_trailer      # the place where C and C++ files are found, compiled and where the app exe is linked.
                                # It calls also ". ${bush_home}/app_distrib" to build the final :
                                #    mgr/bin_<config>/distrib/<distrib zip file>
@endverbatim

*/

