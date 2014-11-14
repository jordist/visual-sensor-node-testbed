################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MultimediaSystem/thirdparty/coder/ac_extended.cpp 

OBJS += \
./src/MultimediaSystem/thirdparty/coder/ac_extended.o 

CPP_DEPS += \
./src/MultimediaSystem/thirdparty/coder/ac_extended.d 


# Each subdirectory must supply rules for building sources it contributes
src/MultimediaSystem/thirdparty/coder/%.o: ../src/MultimediaSystem/thirdparty/coder/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/home/jordi/opencv-arm/include -I/home/jordi/gsl-arm/include/gsl -I/home/jordi/boost-arm/include -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src" -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src/ASN.1" -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src/MultimediaSystem/includes" -O0 -Wall -c -fmessage-length=0 -g2 -mfpu=neon -mfloat-abi=hard -flax-vector-conversions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


