/**

@page bush_introduction <h1>Introduction</h1>

  "bush" is for "BUild with (Bourne) SHell".

  It is a set of Bourne shell scripts used to build softinex applications from a prompt (then it is not an IDE).

  It is a rather simple and rustic build system compared to various other build systems around now (in particular cmake,
 scons or various IDEs as Xcode, VisualStudio, Eclipse) but it permits us to build graphics applications on various
 heterogeneous platforms by using a maximum local capabilities (compiler, graphics system, etc).

  A long experience showed us that the build system may finish to be an upsetting part of the development of an application
 and that there is nothing more upsetting that having a build system that... does not build itself when installing a software!
 Since with bush all is in a well known language, it is rather easy to tweak things in case of problem. In fact it is intended
 to be as much as possible tweakable! (Somewhere it is intended to pass on very exotic/hostile environments :-) ).

  With it we can build our apps on Windows (forever highly exotic) from a CYGWIN prompt by using the Microsoft VisualC++ compiler
 but by avoiding the VisualStudio IDE, and also build macOS/Cocoa apps without having to pass by the Xcode IDE. (It is a pain
 for a single developer, for the same app, to maintain various IDEs). With bush we can build also offscreen version of
 our apps, WebGL and Android version of them. (Obviously we build on Linux, which is a piece of cake).

  If having problem with the compiler and access to some package, the files to look/customize are :
@verbatim
    bush/use_cpp, use_cc.
    bush/use/<package>
@endverbatim
  But there is more in the Usage section.

@section bush_introduction_no_make No make logic

  In bush there is no "make logic". A make logic is the one found in GNU/make where the compilation of a file is decided relative
 to its "touched" includes or if its touched date is less recent than another file belonging to the build. A build is in general
 used in two main cases : build when installing, and/or build when developing. When doing a "build from source" to install,
 someone (a user that may not be a developer) starts anyway from scratch and then compile, link everything. Then in this case
 the make logic is not needed. As in softinex apps we intensively use "pure headers", a make logic is quite not usefull for us too
 when developing; we embed anyway all the code in a "main.cpp" and compile everything. (Ok, the drawback is that the development
 loop is more lengthy than a traditional approach with libraries plus a make logic, but as it permits us to build for highly
 heterogeneous environments, and that it is a high priority for us, we stay with this approach for the moment).

*/

