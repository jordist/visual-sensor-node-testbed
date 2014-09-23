################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/example.cpp 

OBJS += \
./src/example.o 

CPP_DEPS += \
./src/example.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src" -I/opt/local/include -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src/ASN.1" -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src/MultimediaSystem/includes" -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -g2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


