

# These lines will be installation-dependent.
export VSINSTALLDIR='C:\Program Files\Microsoft Visual Studio 9.0\'
export WindowsSdkDir='C:\Program Files\Microsoft SDKs\Windows\v6.0A\'
export FrameworkDir='C:\WINDOWS\Microsoft.NET\Framework\'
export FrameworkVersion=v4.0.30319
export Framework35Version=v3.5

# The following should be largely installation-independent.
export VCINSTALLDIR="$VSINSTALLDIR"'VC\'
export DevEnvDir="$VSINSTALLDIR"'Common7\IDE\'

export FrameworkDIR32="$FrameworkDir"
export FrameworkVersion32="$FrameworkVersion"

export INCLUDE="${VCINSTALLDIR}INCLUDE;${WindowsSdkDir}include;"
export LIB="${VCINSTALLDIR}LIB;${WindowsSdkDir}lib;"
export LIBPATH="${FrameworkDir}${FrameworkVersion};"
export LIBPATH="${LIBPATH}${FrameworkDir}${Framework35Version};"
export LIBPATH="${LIBPATH}${VCINSTALLDIR}LIB;"

c_VSINSTALLDIR=`cygpath -ua "$VSINSTALLDIR\\\\"`
c_WindowsSdkDir=`cygpath -ua "$WindowsSdkDir\\\\"`
c_FrameworkDir=`cygpath -ua "$FrameworkDir\\\\"`

export PATH="${c_WindowsSdkDir}bin:$PATH"
export PATH="${c_WindowsSdkDir}bin/NETFX 4.0 Tools:$PATH"
export PATH="${c_VSINSTALLDIR}VC/VCPackages:$PATH"
export PATH="${c_FrameworkDir}${Framework35Version}:$PATH"
export PATH="${c_FrameworkDir}${FrameworkVersion}:$PATH"
export PATH="${c_VSINSTALLDIR}Common7/Tools:$PATH"
export PATH="${c_VSINSTALLDIR}VC/BIN:$PATH"
export PATH="${c_VSINSTALLDIR}Common7/IDE/:$PATH"

#export PATH="${c_VSINSTALLDIR}VC/redist/x86/Microsoft.VC90.CRT:$PATH"

