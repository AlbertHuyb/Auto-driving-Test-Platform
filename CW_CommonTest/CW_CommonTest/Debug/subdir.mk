################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BoxInfoBase.cpp \
../CConfig.cpp \
../DSRCPrvMsg.cpp \
../DataProcess.cpp \
../EngrDevBase.cpp \
../EngrDevMng.cpp \
../Main.cpp \
../TestCtrlBase.cpp \
../TestInfo.cpp \
../TestMng.cpp \
../recorddatabase.cpp \
../testtool.cpp 

OBJS += \
./BoxInfoBase.o \
./CConfig.o \
./DSRCPrvMsg.o \
./DataProcess.o \
./EngrDevBase.o \
./EngrDevMng.o \
./Main.o \
./TestCtrlBase.o \
./TestInfo.o \
./TestMng.o \
./recorddatabase.o \
./testtool.o 

CPP_DEPS += \
./BoxInfoBase.d \
./CConfig.d \
./DSRCPrvMsg.d \
./DataProcess.d \
./EngrDevBase.d \
./EngrDevMng.d \
./Main.d \
./TestCtrlBase.d \
./TestInfo.d \
./TestMng.d \
./recorddatabase.d \
./testtool.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/wyz/Software/eclipse-c/Workspace2/CW_CoreData_v1" -I"/home/wyz/Software/eclipse-c/Workspace2/CW_DSRCComm_v1" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


