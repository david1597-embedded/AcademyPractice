################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DHT11.c \
../Core/Src/adc.c \
../Core/Src/button.c \
../Core/Src/buzzer.c \
../Core/Src/delay.c \
../Core/Src/door.c \
../Core/Src/elevator.c \
../Core/Src/fnd.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/i2c_lcd.c \
../Core/Src/led.c \
../Core/Src/main.c \
../Core/Src/photo.c \
../Core/Src/spi.c \
../Core/Src/stepper.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/ultrasonic.c \
../Core/Src/usart.c \
../Core/Src/water.c 

OBJS += \
./Core/Src/DHT11.o \
./Core/Src/adc.o \
./Core/Src/button.o \
./Core/Src/buzzer.o \
./Core/Src/delay.o \
./Core/Src/door.o \
./Core/Src/elevator.o \
./Core/Src/fnd.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/i2c_lcd.o \
./Core/Src/led.o \
./Core/Src/main.o \
./Core/Src/photo.o \
./Core/Src/spi.o \
./Core/Src/stepper.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/ultrasonic.o \
./Core/Src/usart.o \
./Core/Src/water.o 

C_DEPS += \
./Core/Src/DHT11.d \
./Core/Src/adc.d \
./Core/Src/button.d \
./Core/Src/buzzer.d \
./Core/Src/delay.d \
./Core/Src/door.d \
./Core/Src/elevator.d \
./Core/Src/fnd.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/i2c_lcd.d \
./Core/Src/led.d \
./Core/Src/main.d \
./Core/Src/photo.d \
./Core/Src/spi.d \
./Core/Src/stepper.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/ultrasonic.d \
./Core/Src/usart.d \
./Core/Src/water.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DHT11.cyclo ./Core/Src/DHT11.d ./Core/Src/DHT11.o ./Core/Src/DHT11.su ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/button.cyclo ./Core/Src/button.d ./Core/Src/button.o ./Core/Src/button.su ./Core/Src/buzzer.cyclo ./Core/Src/buzzer.d ./Core/Src/buzzer.o ./Core/Src/buzzer.su ./Core/Src/delay.cyclo ./Core/Src/delay.d ./Core/Src/delay.o ./Core/Src/delay.su ./Core/Src/door.cyclo ./Core/Src/door.d ./Core/Src/door.o ./Core/Src/door.su ./Core/Src/elevator.cyclo ./Core/Src/elevator.d ./Core/Src/elevator.o ./Core/Src/elevator.su ./Core/Src/fnd.cyclo ./Core/Src/fnd.d ./Core/Src/fnd.o ./Core/Src/fnd.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.cyclo ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/i2c_lcd.cyclo ./Core/Src/i2c_lcd.d ./Core/Src/i2c_lcd.o ./Core/Src/i2c_lcd.su ./Core/Src/led.cyclo ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/photo.cyclo ./Core/Src/photo.d ./Core/Src/photo.o ./Core/Src/photo.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stepper.cyclo ./Core/Src/stepper.d ./Core/Src/stepper.o ./Core/Src/stepper.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/ultrasonic.cyclo ./Core/Src/ultrasonic.d ./Core/Src/ultrasonic.o ./Core/Src/ultrasonic.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/water.cyclo ./Core/Src/water.d ./Core/Src/water.o ./Core/Src/water.su

.PHONY: clean-Core-2f-Src

