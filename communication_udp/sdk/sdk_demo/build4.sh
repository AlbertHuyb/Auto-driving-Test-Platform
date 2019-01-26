rm -fr testfil
source /opt/poky/1.7/environment-setup-cortexa9hf-vfp-neon-poky-linux-gnueabi
$CXX -I"../CW_CoreData_v1" -L ../CW_CoreData_v1/ -lcw_coredata -I"../CW_Shared_Lib_v1" -L ../CW_Shared_Lib_v1/ -lcw_shared_lib -lpthread -Wall -o testfil *.cpp