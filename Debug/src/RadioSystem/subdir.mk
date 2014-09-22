################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/RadioSystem/IncomingMessageQueue.cpp \
../src/RadioSystem/OffloadingManager.cpp \
../src/RadioSystem/RadioSystem.cpp 

OBJS += \
./src/RadioSystem/IncomingMessageQueue.o \
./src/RadioSystem/OffloadingManager.o \
./src/RadioSystem/RadioSystem.o 

CPP_DEPS += \
./src/RadioSystem/IncomingMessageQueue.d \
./src/RadioSystem/OffloadingManager.d \
./src/RadioSystem/RadioSystem.d 


# Each subdirectory must supply rules for building sources it contributes
src/RadioSystem/%.o: ../src/RadioSystem/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/greeneyes/Scrivania/TESTBEDV2/testbed-v2/src" -I/home/greeneyes/OpenCV-Install/2.4.4/include -I"/home/greeneyes/Scrivania/TESTBEDV2/testbed-v2/src/ASN.1" -I"/home/greeneyes/Scrivania/TESTBEDV2/testbed-v2/src/MultimediaSystem/includes" -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -g2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


