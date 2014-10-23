################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/RadioSystem/WiFi/Connection.cpp \
../src/RadioSystem/WiFi/ConnectionManager.cpp \
../src/RadioSystem/WiFi/WiFiRadioSystem.cpp 

OBJS += \
./src/RadioSystem/WiFi/Connection.o \
./src/RadioSystem/WiFi/ConnectionManager.o \
./src/RadioSystem/WiFi/WiFiRadioSystem.o 

CPP_DEPS += \
./src/RadioSystem/WiFi/Connection.d \
./src/RadioSystem/WiFi/ConnectionManager.d \
./src/RadioSystem/WiFi/WiFiRadioSystem.d 


# Each subdirectory must supply rules for building sources it contributes
src/RadioSystem/WiFi/%.o: ../src/RadioSystem/WiFi/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	arm-linux-gnueabihf-g++ -I/home/jordi/opencv-arm/include -I/home/jordi/gsl-arm/include/gsl -I/home/jordi/boost-arm/include -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src" -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src/ASN.1" -I"/home/jordi/Desktop/Thesis/dev/Milano/visual-sensor-node-testbed/src/MultimediaSystem/includes" -O0 -g3 -Wall -c -fmessage-length=0 -g2 -mfpu=neon -mfloat-abi=hard -flax-vector-conversions -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


