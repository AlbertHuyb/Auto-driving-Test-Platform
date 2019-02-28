source /opt/poky/1.7/environment-setup-cortexa9hf-vfp-neon-poky-linux-gnueabi
$CXX -o cw_commtest -lpthread -Wall -I"../CW_CoreData_v1" -L ../CW_CoreData_v1/ -lcw_coredata -I"../CW_DSRCComm_v1" -L ../CW_DSRCComm_v1/ -lcw_dsrccomm *.cpp
