################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/AT45DB021E.c \
../app/app.c \
../app/database.c \
../app/hw.c \
../app/log.c \
../app/rfid.c 

OBJS += \
./app/AT45DB021E.o \
./app/app.o \
./app/database.o \
./app/hw.o \
./app/log.o \
./app/rfid.o 

C_DEPS += \
./app/AT45DB021E.d \
./app/app.d \
./app/database.d \
./app/hw.d \
./app/log.d \
./app/rfid.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o app/%.su app/%.cyclo: ../app/%.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U031xx -c -I../Core/Inc -I../Drivers/STM32U0xx_HAL_Driver/Inc -I../Drivers/STM32U0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U0xx/Include -I../Drivers/CMSIS/Include -I../app/littlefs -I../app -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app

clean-app:
	-$(RM) ./app/AT45DB021E.cyclo ./app/AT45DB021E.d ./app/AT45DB021E.o ./app/AT45DB021E.su ./app/app.cyclo ./app/app.d ./app/app.o ./app/app.su ./app/database.cyclo ./app/database.d ./app/database.o ./app/database.su ./app/hw.cyclo ./app/hw.d ./app/hw.o ./app/hw.su ./app/log.cyclo ./app/log.d ./app/log.o ./app/log.su ./app/rfid.cyclo ./app/rfid.d ./app/rfid.o ./app/rfid.su

.PHONY: clean-app

