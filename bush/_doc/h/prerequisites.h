/**

@page bush_prerequisites <h1>Prerequisites</h1>

@section bush_prerequisites_linux Linux

  You need :
@verbatim
 - a C++ compiler: g++ or clang++. In bush/use_cc, use_cpp, the default uses g++ for Linux.
   If having to tweak things, you have to follow the ${build_gnu} or ${build_clang}
   usage in these scripts and customize what is found within the corresponding "if".
 - zip to build the final binary kit file, but if not here tar is used.
 - ar, ranlib.
 - for graphics apps (most of them!), the X11 and OpenGL "devels" to have the .h headers files.
   (In general the run time .so shared libraries are already here).
@endverbatim
 and... that's all (folks). On most Linux distributions g++, zip, ar, ranlib, X11 and OpenGL are here.
 If not, install them with "sudo apt-get..." or "sudo yum..." according your Linux distribution,
 for example with something as:
@verbatim
    ubuntu> sudo apt-get install libglu1-mesa-dev  # to have OpenGL .h.
 or on centos7:
    centos> sudo yum install gcc gcc-c++
    centos> sudo yum install zip unzip
    centos> sudo yum install libX11-devel libXpm-devel libXft-devel libXext-devel
    centos> sudo yum install mesa-libGL-devel mesa-libGLU-devel
@endverbatim

@section bush_prerequisites_macOS macOS

  In bush/use_cc, use_cpp, the default built for apps is to build with Cocoa, then
 the compiler (clang++) and Cocoa access (under /Applications/Xcode.app/Contents/...)
 uses the one of a Xcode installation, then you have to install Xcode and the
 "Xcode command line tools" to build. If having to tweak things, you have to act
 within the "if" for ${buidl_sdk_mac} or ${build_sdk_mac_gnu}.

  Note that if building a non Cocoa app (for example the X11 version of the pmx application),
 it is the build with clang (${build_clang}) which is used.
@verbatim
    macOS> cd <some_path>/pmx/mgr
    macOS> ./build  # it uses clang++ and put binaries in bin_sdk_mac.
    macOS> open ./bin_sdk_mac/distrib/pmx/<verions>/pmx.app  # to run the Cocoa version.
    macOS> ./build -x11 -bin_x11  # it uses clang++.
    macOS> ./bin_x11/distrib/pmx/<verions>/bin/pmx  # to run the X11 version.
 or:
    macOS> cd <some_path>/inlib/examples/cpp
    macOS> ./build wroot.cpp
    macOS> ./bin_clang/wroot
 or:
    macOS> cd <some_path>/exlib/examples/cpp
    macOS> ./build GL_X11.cpp
    macOS> ./bin_clang/GL_X11
@endverbatim

@section bush_prerequisites_windows Windows-10

  On Windows-10 we build with VisualC++ (cl.exe) from a CYGWIN prompt.
 VisualC++ is mendatory since, by default, we do the graphics of our apps with the
 Microsoft WIN32 libs (in particular opengl32.lib). Stricly speaking our apps may be built by using the CYGWIN g++
 and X11 client libs, but this would need to pass by a local X11 server and would be clearly
 less performant than being "pure Microsoft". (Moreover the CYGWIN/g++ is known
 to be a snail compared to VisualC++). (We did not tried to use a clang++ coming from CYGWIN).

 Then you need:
@verbatim
 - Visual Studio. Note that we do not use the IDE, only the compiler and linker
   and that a free version is available from Microsoft that permits to use these.
 - CYGWIN with at least zip (bash is here by default). We installed git (to get code)
   and openssh (to log in and out to remote machines), and... it must be all.
@endverbatim
 If having to tweak bush/use_cc or use_cpp follow the string "${build_visual}". Note that the access
 to cl.exe to compile and the Visual link.exe to link is concentrated in the script bush/vc++.
 In particular all other build scripts (for example bush/compile_one_cpp) uses bush/vc++
 with the same options than g++ and clang++ to declare include paths (-I), link path
 (-L), etc... The script bush/vc++ translates these to the /I, /L known by cl.exe, link.exe
 and does also the job to translate CYGWIN unix paths to Windows paths.

  IMPORTANT : bush/vc++ assumes that "cl.exe" is in your PATH, then that the "vcvars.bat" of VisualC++
 had been executed. (vcvars is a DOS script that setup the environment of VisualC++). Then to work, the
 coarse graining procedure is something as :
@verbatim
 - install Visual Studio.
 - install CYGWIN.
 - create a command line window DOS prompt by arranging to run the Windows cmd.exe program.
 - from here, arrange to "CALL VCVARS.BAT" of VisualC++. It should be somewhere under the C:\Program files
   directory where VisualStudio is installed.
 - from the same DOS prompt, launch CYGWIN. Then from the CYGWIN bash prompt, typing cl.exe should do something.
 - from the CYGWIN prompt you can start to build some app by using bush.
@endverbatim

 (Note that there is a falltrap with "link.exe" which exists as a VisualStudio program to link an application
 and a CYGWIN program found under /usr/bin. It is the one of Visual that should be used. The bush/find_link_exe
 script is used by bush/vc++ to find the Visual one. You may have to customize it to find the link.exe under your
 VisualC++ installation).

@subsection bush_prerequisites_getline Programs that use Getline to enter commands

  Programs, as gopaw, that use Getline to enter commands from the terminal, must be launched from a cmd.exe
 command line DOS window, and not from the CYGWIN prompt. Getline does not work if the program is started from
 a CYGWIN prompt. This goes for gopaw but also for apps that had been built to be able to receive commands
 from the terminal and started with the "-terminal" option.

*/
