
set on_my_mac_2018="`networksetup -listallhardwareports | grep '86:00'`"
if ( "${on_my_mac_2018}" != "" ) then
  set java_home=/Library/Java/JavaVirtualMachines/jdk1.8.0_181.jdk/Contents/Home
  setenv JAVA_HOME ${java_home}
endif

set spark_home=/usr/local/spark/2.3.0/spark-2.3.0-bin-hadoop2.7

setenv PATH "${spark_home}/bin:$PATH"


