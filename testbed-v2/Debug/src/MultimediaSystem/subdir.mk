################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MultimediaSystem/Briskola.cpp \
../src/MultimediaSystem/CodecParams.cpp \
../src/MultimediaSystem/ImageAcquisition.cpp \
../src/MultimediaSystem/VisualFeatureDecoding.cpp \
../src/MultimediaSystem/VisualFeatureEncoding.cpp \
../src/MultimediaSystem/VisualFeatureExtraction.cpp 

OBJS += \
./src/MultimediaSystem/Briskola.o \
./src/MultimediaSystem/CodecParams.o \
./src/MultimediaSystem/ImageAcquisition.o \
./src/MultimediaSystem/VisualFeatureDecoding.o \
./src/MultimediaSystem/VisualFeatureEncoding.o \
./src/MultimediaSystem/VisualFeatureExtraction.o 

CPP_DEPS += \
./src/MultimediaSystem/Briskola.d \
./src/MultimediaSystem/CodecParams.d \
./src/MultimediaSystem/ImageAcquisition.d \
./src/MultimediaSystem/VisualFeatureDecoding.d \
./src/MultimediaSystem/VisualFeatureEncoding.d \
./src/MultimediaSystem/VisualFeatureExtraction.d 


# Each subdirectory must supply rules for building sources it contributes
src/MultimediaSystem/%.o: ../src/MultimediaSystem/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src" -I/opt/local/include -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src/ASN.1" -I"/Users/lucabaroffio/Documents/workspace_GREENEYES/visual-sensor-node-testbed/src/MultimediaSystem/includes" -I/usr/local/include -O0 -g3 -Wall -c -fmessage-length=0 -g2 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


