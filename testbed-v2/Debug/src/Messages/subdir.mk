################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Messages/CoopInfoMsg.cpp \
../src/Messages/DataATCMsg.cpp \
../src/Messages/DataCTAMsg.cpp \
../src/Messages/Header.cpp \
../src/Messages/Message.cpp \
../src/Messages/StartATCMsg.cpp \
../src/Messages/StartCTAMsg.cpp \
../src/Messages/StartDATCMsg.cpp \
../src/Messages/StopMsg.cpp 

OBJS += \
./src/Messages/CoopInfoMsg.o \
./src/Messages/DataATCMsg.o \
./src/Messages/DataCTAMsg.o \
./src/Messages/Header.o \
./src/Messages/Message.o \
./src/Messages/StartATCMsg.o \
./src/Messages/StartCTAMsg.o \
./src/Messages/StartDATCMsg.o \
./src/Messages/StopMsg.o 

CPP_DEPS += \
./src/Messages/CoopInfoMsg.d \
./src/Messages/DataATCMsg.d \
./src/Messages/DataCTAMsg.d \
./src/Messages/Header.d \
./src/Messages/Message.d \
./src/Messages/StartATCMsg.d \
./src/Messages/StartCTAMsg.d \
./src/Messages/StartDATCMsg.d \
./src/Messages/StopMsg.d 


# Each subdirectory must supply rules for building sources it contributes
src/Messages/%.o: ../src/Messages/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src" -I/opt/local/include -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src/ASN.1" -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src/MultimediaSystem/includes" -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -g2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


