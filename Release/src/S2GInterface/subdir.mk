################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/S2GInterface/S2GInterface.cpp 

OBJS += \
./src/S2GInterface/S2GInterface.o 

CPP_DEPS += \
./src/S2GInterface/S2GInterface.d 


# Each subdirectory must supply rules for building sources it contributes
src/S2GInterface/%.o: ../src/S2GInterface/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


