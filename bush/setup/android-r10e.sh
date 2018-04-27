
sdk_home=/usr/local/Android/android-sdk_r16-macosx

ndk_home=/usr/local/Android/android-ndk-r10e

PATH="${sdk_home}/tools:${PATH}"
PATH="${sdk_home}/platform-tools:${PATH}"
PATH="${ndk_home}:${PATH}"

PATH="${PATH}:/opt/local/bin"   # for ant.

unset sdk_home
unset ndk_home
