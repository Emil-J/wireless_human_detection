################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/cmsis_os2.c \
../Core/Src/croutine.c \
../Core/Src/event_groups.c \
../Core/Src/freertos.c \
../Core/Src/gpio.c \
../Core/Src/heap_4.c \
../Core/Src/list.c \
../Core/Src/main.c \
../Core/Src/port.c \
../Core/Src/queue.c \
../Core/Src/spi.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_hal_timebase_tim.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/stream_buffer.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/tasks.c \
../Core/Src/tim.c \
../Core/Src/timers.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/cmsis_os2.o \
./Core/Src/croutine.o \
./Core/Src/event_groups.o \
./Core/Src/freertos.o \
./Core/Src/gpio.o \
./Core/Src/heap_4.o \
./Core/Src/list.o \
./Core/Src/main.o \
./Core/Src/port.o \
./Core/Src/queue.o \
./Core/Src/spi.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_hal_timebase_tim.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/stream_buffer.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/tasks.o \
./Core/Src/tim.o \
./Core/Src/timers.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/cmsis_os2.d \
./Core/Src/croutine.d \
./Core/Src/event_groups.d \
./Core/Src/freertos.d \
./Core/Src/gpio.d \
./Core/Src/heap_4.d \
./Core/Src/list.d \
./Core/Src/main.d \
./Core/Src/port.d \
./Core/Src/queue.d \
./Core/Src/spi.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_hal_timebase_tim.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/stream_buffer.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/tasks.d \
./Core/Src/tim.d \
./Core/Src/timers.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/cmsis_os2.cyclo ./Core/Src/cmsis_os2.d ./Core/Src/cmsis_os2.o ./Core/Src/cmsis_os2.su ./Core/Src/croutine.cyclo ./Core/Src/croutine.d ./Core/Src/croutine.o ./Core/Src/croutine.su ./Core/Src/event_groups.cyclo ./Core/Src/event_groups.d ./Core/Src/event_groups.o ./Core/Src/event_groups.su ./Core/Src/freertos.cyclo ./Core/Src/freertos.d ./Core/Src/freertos.o ./Core/Src/freertos.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/heap_4.cyclo ./Core/Src/heap_4.d ./Core/Src/heap_4.o ./Core/Src/heap_4.su ./Core/Src/list.cyclo ./Core/Src/list.d ./Core/Src/list.o ./Core/Src/list.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/port.cyclo ./Core/Src/port.d ./Core/Src/port.o ./Core/Src/port.su ./Core/Src/queue.cyclo ./Core/Src/queue.d ./Core/Src/queue.o ./Core/Src/queue.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_hal_timebase_tim.cyclo ./Core/Src/stm32l4xx_hal_timebase_tim.d ./Core/Src/stm32l4xx_hal_timebase_tim.o ./Core/Src/stm32l4xx_hal_timebase_tim.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/stream_buffer.cyclo ./Core/Src/stream_buffer.d ./Core/Src/stream_buffer.o ./Core/Src/stream_buffer.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/tasks.cyclo ./Core/Src/tasks.d ./Core/Src/tasks.o ./Core/Src/tasks.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/timers.cyclo ./Core/Src/timers.d ./Core/Src/timers.o ./Core/Src/timers.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

