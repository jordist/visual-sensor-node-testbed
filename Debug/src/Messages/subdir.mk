################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Messages/DataATCMsg.cpp \
../src/Messages/DataCTAMsg.cpp \
../src/Messages/Header.cpp \
../src/Messages/Message.cpp \
../src/Messages/StartATCMsg.cpp \
../src/Messages/StartCTAMsg.cpp \
../src/Messages/StopMsg.cpp 

OBJS += \
./src/Messages/DataATCMsg.o \
./src/Messages/DataCTAMsg.o \
./src/Messages/Header.o \
./src/Messages/Message.o \
./src/Messages/StartATCMsg.o \
./src/Messages/StartCTAMsg.o \
./src/Messages/StopMsg.o 

CPP_DEPS += \
./src/Messages/DataATCMsg.d \
./src/Messages/DataCTAMsg.d \
./src/Messages/Header.d \
./src/Messages/Message.d \
./src/Messages/StartATCMsg.d \
./src/Messages/StartCTAMsg.d \
./src/Messages/StopMsg.d 


# Each subdirectory must supply rules for building sources it contributes
src/Messages/%.o: ../src/Messages/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/greeneyes/Scrivania/TESTBEDV2/testbed-v2/src" -I/home/greeneyes/OpenCV-Install/2.4.4/include -I"/home/greeneyes/Scrivania/TESTBEDV2/testbed-v2/src/ASN.1" -I"/home/greeneyes/Scrivania/TESTBEDV2/testbed-v2/src/MultimediaSystem/includes" -I/usr/include -O0 -g3 -Wall -c -fmessage-length=0 -g2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

