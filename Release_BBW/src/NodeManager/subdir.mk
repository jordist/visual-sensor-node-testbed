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
	arm-linux-gnueabi-g++ -I/home/greeneyes/OpenCV-Install/2.4.9-arm/include -I/home/greeneyes/boost-arm/include -I"/home/greeneyes/Scrivania/TESTBEDV2/visual-sensor-node-testbed/src" -I"/home/greeneyes/Scrivania/TESTBEDV2/visual-sensor-node-testbed/src/ASN.1" -I"/home/greeneyes/Scrivania/TESTBEDV2/visual-sensor-node-testbed/src/MultimediaSystem/includes" -O0 -g3 -Wall -c -fmessage-length=0 -g2 -mfpu=neon -mfloat-abi=softfp -flax-vector-conversions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


