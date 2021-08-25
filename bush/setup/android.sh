
on_my_mac_2018="`networksetup -listallhardwareports | grep '86:00'`"
if [ "${on_my_mac_2018}" != "" ] ; then
  java_home=/Library/Java/JavaVirtualMachines/jdk1.8.0_181.jdk/Contents/Home
  JAVA_HOME=${java_home};export JAVA_HOME
fi
on_my_mac_2020="`networksetup -listallhardwareports | grep 'f8:ff:c2:07:bb:7b'`"
if [ "${on_my_mac_2020}" != "" ] ; then
  java_home=/usr/local/Android/jdk1.8.0_60.jdk/Contents/Home
  JAVA_HOME=${java_home};export JAVA_HOME
fi

sdk_home=/usr/local/Android/android-sdk_r16-macosx

ndk_home=/usr/local/Android/android-ndk-r17

PATH="${sdk_home}/tools:${PATH}"
PATH="${sdk_home}/platform-tools:${PATH}"
PATH="${ndk_home}:${PATH}"

PATH="${PATH}:/opt/local/bin"   # for ant.

export ANDROID_SDK_HOME=${sdk_home}

unset sdk_home
unset ndk_home
