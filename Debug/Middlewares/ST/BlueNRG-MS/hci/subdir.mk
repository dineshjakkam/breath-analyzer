################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/BlueNRG-MS/hci/hci_le.c 

OBJS += \
./Middlewares/ST/BlueNRG-MS/hci/hci_le.o 

C_DEPS += \
./Middlewares/ST/BlueNRG-MS/hci/hci_le.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/BlueNRG-MS/hci/hci_le.o: ../Middlewares/ST/BlueNRG-MS/hci/hci_le.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -g3 -DUSE_HAL_DRIVER -DSTM32F401xE -DDEBUG -c -I../Middlewares/ST/BlueNRG-MS/hci/hci_tl_patterns/Basic -I../Middlewares/ST/BlueNRG-MS/includes -I../Drivers/CMSIS/Include -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Core/Inc -I../BlueNRG-MS/Target -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/ST/BlueNRG-MS/utils -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/BlueNRG-MS/hci/hci_le.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

