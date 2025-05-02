################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/littlefs/lfs.c \
../app/littlefs/lfs_util.c 

OBJS += \
./app/littlefs/lfs.o \
./app/littlefs/lfs_util.o 

C_DEPS += \
./app/littlefs/lfs.d \
./app/littlefs/lfs_util.d 


# Each subdirectory must supply rules for building sources it contributes
app/littlefs/%.o app/littlefs/%.su app/littlefs/%.cyclo: ../app/littlefs/%.c app/littlefs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U031xx -c -I../Core/Inc -I"C:/Users/Codey/OneDrive/Documentos/SEMB/fw-rfid-semb-01/rfid_semb/app" -I../Drivers/STM32U0xx_HAL_Driver/Inc -I../Drivers/STM32U0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U0xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app-2f-littlefs

clean-app-2f-littlefs:
	-$(RM) ./app/littlefs/lfs.cyclo ./app/littlefs/lfs.d ./app/littlefs/lfs.o ./app/littlefs/lfs.su ./app/littlefs/lfs_util.cyclo ./app/littlefs/lfs_util.d ./app/littlefs/lfs_util.o ./app/littlefs/lfs_util.su

.PHONY: clean-app-2f-littlefs

