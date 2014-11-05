################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/Tasks/subdir.mk
-include src/TaskManager/subdir.mk
-include src/S2GInterface/subdir.mk
-include src/RadioSystem/WiFi/ASN1/subdir.mk
-include src/RadioSystem/WiFi/subdir.mk
-include src/RadioSystem/Telosb/subdir.mk
-include src/RadioSystem/subdir.mk
-include src/NodeManager/subdir.mk
-include src/MultimediaSystem/thirdparty/coder/subdir.mk
-include src/MultimediaSystem/thirdparty/agast/src/subdir.mk
-include src/MultimediaSystem/subdir.mk
-include src/Messages/subdir.mk
-include src/ASN.1/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: testbed-v2

# Tool invocations
testbed-v2: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MacOS X C++ Linker'
	arm-linux-gnueabihf-g++ -L/home/jordi/opencv-arm/lib -L/home/jordi/lpsolve-arm -L/home/jordi/gsl-arm/lib -L/home/jordi/boost-arm/lib -Xlinker -rpath='/home/debian/boost-arm/lib' -Xlinker -rpath='/home/debian/opencv-arm/lib' -o "testbed-v2" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) testbed-v2
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
