################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Tasks/Tasks.cpp 

OBJS += \
./src/Tasks/Tasks.o 

CPP_DEPS += \
./src/Tasks/Tasks.d 


# Each subdirectory must supply rules for building sources it contributes
src/Tasks/%.o: ../src/Tasks/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/home/greeneyes/OpenCV-Install/2.4.9-arm/include -I/home/greeneyes/boost-arm/include -I"/home/greeneyes/Scrivania/TESTBEDV2/visual-sensor-node-testbed/src" -I"/home/greeneyes/Scrivania/TESTBEDV2/visual-sensor-node-testbed/src/ASN.1" -I"/home/greeneyes/Scrivania/TESTBEDV2/visual-sensor-node-testbed/src/MultimediaSystem/includes" -O0 -g3 -Wall -c -fmessage-length=0 -g2 -mfpu=neon -mfloat-abi=hard -flax-vector-conversions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


