################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/RadioSystem/IncomingMessageQueue.cpp \
../src/RadioSystem/LoadBalancing.cpp \
../src/RadioSystem/LoadBalancingConfig.cpp \
../src/RadioSystem/MessageParser.cpp \
../src/RadioSystem/OffloadingManager.cpp \
../src/RadioSystem/ProcessingSpeedEstimator.cpp \
../src/RadioSystem/RadioSystem.cpp 

OBJS += \
./src/RadioSystem/IncomingMessageQueue.o \
./src/RadioSystem/LoadBalancing.o \
./src/RadioSystem/LoadBalancingConfig.o \
./src/RadioSystem/MessageParser.o \
./src/RadioSystem/OffloadingManager.o \
./src/RadioSystem/ProcessingSpeedEstimator.o \
./src/RadioSystem/RadioSystem.o 

CPP_DEPS += \
./src/RadioSystem/IncomingMessageQueue.d \
./src/RadioSystem/LoadBalancing.d \
./src/RadioSystem/LoadBalancingConfig.d \
./src/RadioSystem/MessageParser.d \
./src/RadioSystem/OffloadingManager.d \
./src/RadioSystem/ProcessingSpeedEstimator.d \
./src/RadioSystem/RadioSystem.d 


# Each subdirectory must supply rules for building sources it contributes
src/RadioSystem/%.o: ../src/RadioSystem/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src" -I/home/jordi/Desktop/Thesis/dev/Milano/opencv-2.4.9/include -I/usr/local/include -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src/ASN.1" -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src/MultimediaSystem/includes" -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -g2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


