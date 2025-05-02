################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/app.c \
../app/database.c \
../app/hw.c \
../app/rfid.c 

OBJS += \
./app/app.o \
./app/database.o \
./app/hw.o \
./app/rfid.o 

C_DEPS += \
./app/app.d \
./app/database.d \
./app/hw.d \
./app/rfid.d 


# Each subdirectory must supply rules for building sources it contributes
app/%.o app/%.su app/%.cyclo: ../app/%.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U031xx -c -I../Core/Inc -I"C:/Users/Codey/OneDrive/Documentos/SEMB/fw-rfid-semb-01/rfid_semb/app" -I../Drivers/STM32U0xx_HAL_Driver/Inc -I../Drivers/STM32U0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app

clean-app:
	-$(RM) ./app/app.cyclo ./app/app.d ./app/app.o ./app/app.su ./app/database.cyclo ./app/database.d ./app/database.o ./app/database.su ./app/hw.cyclo ./app/hw.d ./app/hw.o ./app/hw.su ./app/rfid.cyclo ./app/rfid.d ./app/rfid.o ./app/rfid.su

.PHONY: clean-app

