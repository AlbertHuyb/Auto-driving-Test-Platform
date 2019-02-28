################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../engr/EngrDevBase.cpp \
../engr/EngrDevMng.cpp 

OBJS += \
./engr/EngrDevBase.o \
./engr/EngrDevMng.o 

CPP_DEPS += \
./engr/EngrDevBase.d \
./engr/EngrDevMng.d 


# Each subdirectory must supply rules for building sources it contributes
engr/%.o: ../engr/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I"/home/wyz/Software/eclipse-c/Workspace2/CW_CoreData_v1" -I"/home/wyz/Software/eclipse-c/Workspace2/CW_DSRCComm_v1" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


