/**

@page bush_minimum <h1>Some minimum</h1>

  We remember here some "Bourne shell minimum".

  variable :
@verbatim
     my_variable=value
     echo "my_variable value is ${my_variable}"
@endverbatim

  conditional :
@verbatim
     if [ "${my_variable}" = "hello" ] ; then
       echo "my_variable is hello"
     elif [ "${my_variable}" != "bye" ] ; then
       echo "my_variable is not bye"
     else
       echo "my_variable is not hello and is bye"
     fi
@endverbatim

  loop :
@verbatim
     list='aa bb'
     for item in ${list} ; do echo "item ${item}"; done
@endverbatim

  The great backquoting and the definitive pipe :
@verbatim
     if [ "`uname`" = Darwin ] ; then
       echo 'On my beloved Mac ?'
     elif [ "`uname | grep CYGWIN`" != "" ] ; then
       echo 'Aie, I am on a Windows.'
     else
       echo 'Hi Linus!'
     fi
@endverbatim

 With that in head you have good chance to be able to read and tweak our build scripts.

*/
