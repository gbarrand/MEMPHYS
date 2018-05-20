/**

@page bush_options <h1>Options</h1>

 Options to be known :
@verbatim
     -v : coarse graining verbosity.
     -x : full traceback (it does a : set -x)
     -bin_<xxx> : enforce ./bin_xxx as the target bin directory. If not the target directory is set in
                  bush/header on the build_dir variable to be bin_${config_name} with the config_name variable
                  set in use_cpp, use_cc. Typically build_dir is bin_gnu on a Linux, bin_visual on Windows/CYGWIN,
                  bin_clang on Mac for not graphics apps, bin_sdk_mac for Cocoa/graphics apps.
    -g  : to compile with -g.
 -debug : to compile with -g.
 -c++11 : to compile with c++11 extensions. (Used in bush/use_cpp).
 -parallel : if the GNU parallel application is known from your prompt, C and C++ file compilations
             are distributed on your cores. This feature had been tested only on macOS for the moment
             with a MacPorts installation of the parallel program. Since good part of the code of our
             apps is pure header, it will not boost the "main" part of them, but it clearly boosts the
             build of the ourex externals, in particular if using some ourex/geant4<versio> one.
@endverbatim

 On apps (for example from pmx/mgr) :
@verbatim
     -offscreen -bin_offscreen : to build offscreen version of the app, with binary directory being mgr/bin_offscreen.
     -android : result is put in bin_sdk_android.
     -x11 -bin_x11 : to build the X11 version of the app (for example on a Mac, which is not the default), with
                     binary directory being mgr/bin_x11.     
@endverbatim

*/
