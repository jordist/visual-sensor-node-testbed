################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TaskManager/TaskManager.cpp \
../src/TaskManager/TaskQueue.cpp 

OBJS += \
./src/TaskManager/TaskManager.o \
./src/TaskManager/TaskQueue.o 

CPP_DEPS += \
./src/TaskManager/TaskManager.d \
./src/TaskManager/TaskQueue.d 


# Each subdirectory must supply rules for building sources it contributes
src/TaskManager/%.o: ../src/TaskManager/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src" -I/opt/local/include -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src/ASN.1" -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src/MultimediaSystem/includes" -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -g2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


