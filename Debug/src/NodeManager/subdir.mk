################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/NodeManager/NodeManager.cpp 

OBJS += \
./src/NodeManager/NodeManager.o 

CPP_DEPS += \
./src/NodeManager/NodeManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/NodeManager/%.o: ../src/NodeManager/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src" -I/home/jordi/Desktop/Thesis/dev/Milano/opencv-2.4.9/include -I/usr/local/include -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src/ASN.1" -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src/MultimediaSystem/includes" -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -g2 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


