
#
# cygwin_vc++.csh is used at ssh and tcsh login to setup VisualC++ 14 environment.
#


if ( "`printenv VCINSTALLDIR`" == "" ) then

setenv Framework40Version 'v4.0'
setenv VSINSTALLDIR 'C:\Program Files (x86)\Microsoft Visual Studio 14.0\'
setenv Platform 'X64'
setenv ORIGINAL_PATH '/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/Common7/IDE/CommonExtensions/Microsoft/TestWindow:/cygdrive/c/Program Files (x86)/MSBuild/14.0/bin/amd64:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/VC/BIN/amd64:/cygdrive/c/Windows/Microsoft.NET/Framework64/v4.0.30319:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/VC/VCPackages:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/Common7/IDE:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/Common7/Tools:/cygdrive/c/Program Files (x86)/HTML Help Workshop:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/Team Tools/Performance Tools/x64:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/Team Tools/Performance Tools:/cygdrive/c/Program Files (x86)/Windows Kits/10/bin/x64:/cygdrive/c/Program Files (x86)/Windows Kits/10/bin/x86:/cygdrive/c/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.6 Tools/x64:/cygdrive/c/Windows/system32:/cygdrive/c/Windows:/cygdrive/c/Windows/System32/Wbem:/cygdrive/c/Windows/System32/WindowsPowerShell/v1.0:/cygdrive/c/Windows/system32/config/systemprofile/.dnx/bin:/cygdrive/c/Program Files/Microsoft DNX/Dnvm:/cygdrive/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit:/cygdrive/c/Users/barrand/.dnx/bin'
setenv WindowsLibPath 'C:\Program Files (x86)\Windows Kits\10\UnionMetadata;C:\Program Files (x86)\Windows Kits\10\References;C:\Program Files (x86)\Windows Kits\10\References\Windows.Foundation.UniversalApiContract\1.0.0.0;C:\Program Files (x86)\Windows Kits\10\References\Windows.Foundation.FoundationContract\1.0.0.0;C:\Program Files (x86)\Windows Kits\10\References\indows.Networking.Connectivity.WwanContract\1.0.0.0'
setenv WindowsSDKLibVersion '10.0.10240.0\'
setenv FrameworkVersion64 'v4.0.30319'
setenv VCINSTALLDIR 'C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\'
setenv LIB 'C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\LIB\amd64;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\ATLMFC\LIB\amd64;C:\Program Files (x86)\Windows Kits\10\lib\10.0.10240.0\ucrt\x64;C:\Program Files (x86)\Windows Kits\NETFXSDK\4.6\lib\um\x64;C:\Program Files (x86)\Windows Kits\10\lib\10.0.10240.0\um\x64;'
setenv CommandPromptType 'Native'
setenv LIBPATH 'C:\Windows\Microsoft.NET\Framework64\v4.0.30319;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\LIB\amd64;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\ATLMFC\LIB\amd64;C:\Program Files (x86)\Windows Kits\10\UnionMetadata;C:\Program Files (x86)\Windows Kits\10\References;C:\Program Files (x86)\Windows Kits\10\References\Windows.Foundation.UniversalApiContract\1.0.0.0;C:\Program Files (x86)\Windows Kits\10\References\Windows.Foundation.FoundationContract\1.0.0.0;C:\Program Files (x86)\Windows Kits\10\References\indows.Networking.Connectivity.WwanContract\1.0.0.0;C:\Program Files (x86)\Microsoft SDKs\Windows Kits\10\ExtensionSDKs\Microsoft.VCLibs\14.0\References\CommonConfiguration\neutral;'
setenv PATH '/usr/local/bin:/usr/bin:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/Common7/IDE/CommonExtensions/Microsoft/TestWindow:/cygdrive/c/Program Files (x86)/MSBuild/14.0/bin/amd64:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/VC/BIN/amd64:/cygdrive/c/Windows/Microsoft.NET/Framework64/v4.0.30319:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/VC/VCPackages:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/Common7/IDE:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/Common7/Tools:/cygdrive/c/Program Files (x86)/HTML Help Workshop:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/Team Tools/Performance Tools/x64:/cygdrive/c/Program Files (x86)/Microsoft Visual Studio 14.0/Team Tools/Performance Tools:/cygdrive/c/Program Files (x86)/Windows Kits/10/bin/x64:/cygdrive/c/Program Files (x86)/Windows Kits/10/bin/x86:/cygdrive/c/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.6 Tools/x64:/cygdrive/c/Windows/system32:/cygdrive/c/Windows:/cygdrive/c/Windows/System32/Wbem:/cygdrive/c/Windows/System32/WindowsPowerShell/v1.0:/cygdrive/c/Windows/system32/config/systemprofile/.dnx/bin:/cygdrive/c/Program Files/Microsoft DNX/Dnvm:/cygdrive/c/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit:/cygdrive/c/Users/barrand/.dnx/bin'
setenv WindowsSDKVersion '10.0.10240.0\'
setenv FrameworkDIR64 'C:\Windows\Microsoft.NET\Framework64'
setenv FrameworkDir 'C:\Windows\Microsoft.NET\Framework64'
setenv WindowsSdkDir 'C:\Program Files (x86)\Windows Kits\10\'
setenv NETFXSDKDir 'C:\Program Files (x86)\Windows Kits\NETFXSDK\4.6\'
setenv WindowsSDK_ExecutablePath_x86 'C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.6 Tools\'
setenv VisualStudioVersion '14.0'
setenv ExtensionSdkDir 'C:\Program Files (x86)\Microsoft SDKs\Windows Kits\10\ExtensionSDKs'
setenv UniversalCRTSdkDir 'C:\Program Files (x86)\Windows Kits\10\'
setenv WindowsSDK_ExecutablePath_x64 'C:\Program Files (x86)\Microsoft SDKs\Windows\v10.0A\bin\NETFX 4.6 Tools\x64\'
setenv UCRTVersion '10.0.10240.0'
setenv INCLUDE 'C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\INCLUDE;C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\ATLMFC\INCLUDE;C:\Program Files (x86)\Windows Kits\10\include\10.0.10240.0\ucrt;C:\Program Files (x86)\Windows Kits\NETFXSDK\4.6\include\um;C:\Program Files (x86)\Windows Kits\10\include\10.0.10240.0\shared;C:\Program Files (x86)\Windows Kits\10\include\10.0.10240.0\um;C:\Program Files (x86)\Windows Kits\10\include\10.0.10240.0\winrt;'
setenv FrameworkVersion 'v4.0.30319'

endif




