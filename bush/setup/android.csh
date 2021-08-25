
set on_my_mac_2018="`networksetup -listallhardwareports | grep '86:00'`"
if ( "${on_my_mac_2018}" != "" ) then
  set java_home=/Library/Java/JavaVirtualMachines/jdk1.8.0_181.jdk/Contents/Home
  setenv JAVA_HOME ${java_home}
endif

set sdk_home=/usr/local/Android/android-sdk_r16-macosx

set ndk_home=/usr/local/Android/android-ndk-r17

setenv PATH "${sdk_home}/tools:${PATH}"
setenv PATH "${sdk_home}/platform-tools:${PATH}"
setenv PATH "${ndk_home}:${PATH}"

setenv PATH "${PATH}:/opt/local/bin"   # for ant.

setenv ANDROID_SDK_HOME ${sdk_home}

unset sdk_home
unset ndk_home
