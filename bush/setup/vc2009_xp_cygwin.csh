

# These lines will be installation-dependent.
setenv VSINSTALLDIR 'C:\Program Files\Microsoft Visual Studio 9.0\'
setenv WindowsSdkDir 'C:\Program Files\Microsoft SDKs\Windows\v6.0A\'
setenv FrameworkDir 'C:\WINDOWS\Microsoft.NET\Framework\'
setenv FrameworkVersion v4.0.30319
setenv Framework35Version v3.5

# The following should be largely installation-independent.
setenv VCINSTALLDIR "$VSINSTALLDIR"'VC\'
setenv DevEnvDir "$VSINSTALLDIR"'Common7\IDE\'

setenv FrameworkDIR32 "$FrameworkDir"
setenv FrameworkVersion32 "$FrameworkVersion"

setenv INCLUDE "${VCINSTALLDIR}INCLUDE;${WindowsSdkDir}include;"
setenv LIB "${VCINSTALLDIR}LIB;${WindowsSdkDir}lib;"
setenv LIBPATH "${FrameworkDir}${FrameworkVersion};"
setenv LIBPATH "${LIBPATH}${FrameworkDir}${Framework35Version};"
setenv LIBPATH "${LIBPATH}${VCINSTALLDIR}LIB;"

set c_VSINSTALLDIR=`cygpath -ua "$VSINSTALLDIR\\\\"`
set c_WindowsSdkDir=`cygpath -ua "$WindowsSdkDir\\\\"`
set c_FrameworkDir=`cygpath -ua "$FrameworkDir\\\\"`

setenv PATH "${c_WindowsSdkDir}bin:$PATH"
setenv PATH "${c_WindowsSdkDir}bin/NETFX 4.0 Tools:$PATH"
setenv PATH "${c_VSINSTALLDIR}VC/VCPackages:$PATH"
setenv PATH "${c_FrameworkDir}${Framework35Version}:$PATH"
setenv PATH "${c_FrameworkDir}${FrameworkVersion}:$PATH"
setenv PATH "${c_VSINSTALLDIR}Common7/Tools:$PATH"
setenv PATH "${c_VSINSTALLDIR}VC/BIN:$PATH"
setenv PATH "${c_VSINSTALLDIR}Common7/IDE/:$PATH"

#setenv PATH "${c_VSINSTALLDIR}VC/redist/x86/Microsoft.VC90.CRT:$PATH"

