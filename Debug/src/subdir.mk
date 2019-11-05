################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Command.cpp \
../src/Debugger.cpp \
../src/Hardware.cpp \
../src/Runtime.cpp \
../src/Utils.cpp 

OBJS += \
./src/Command.o \
./src/Debugger.o \
./src/Hardware.o \
./src/Runtime.o \
./src/Utils.o 

CPP_DEPS += \
./src/Command.d \
./src/Debugger.d \
./src/Hardware.d \
./src/Runtime.d \
./src/Utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


