################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/src/FreeRTOSCommonHooks.c \
../FreeRTOS/src/croutine.c \
../FreeRTOS/src/event_groups.c \
../FreeRTOS/src/heap_5.c \
../FreeRTOS/src/list.c \
../FreeRTOS/src/port.c \
../FreeRTOS/src/queue.c \
../FreeRTOS/src/stream_buffer.c \
../FreeRTOS/src/tasks.c \
../FreeRTOS/src/timers.c 

OBJS += \
./FreeRTOS/src/FreeRTOSCommonHooks.o \
./FreeRTOS/src/croutine.o \
./FreeRTOS/src/event_groups.o \
./FreeRTOS/src/heap_5.o \
./FreeRTOS/src/list.o \
./FreeRTOS/src/port.o \
./FreeRTOS/src/queue.o \
./FreeRTOS/src/stream_buffer.o \
./FreeRTOS/src/tasks.o \
./FreeRTOS/src/timers.o 

C_DEPS += \
./FreeRTOS/src/FreeRTOSCommonHooks.d \
./FreeRTOS/src/croutine.d \
./FreeRTOS/src/event_groups.d \
./FreeRTOS/src/heap_5.d \
./FreeRTOS/src/list.d \
./FreeRTOS/src/port.d \
./FreeRTOS/src/queue.d \
./FreeRTOS/src/stream_buffer.d \
./FreeRTOS/src/tasks.d \
./FreeRTOS/src/timers.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/src/%.o: ../FreeRTOS/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__LPC17XX__ -DCR_INTEGER_PRINTF -D__USE_LPCOPEN -DNO_BOARD_LIB -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_lpc_board_nxp_lpcxpresso_1769\inc" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_lpc_chip_175x_6x\inc" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_lpc_chip_175x_6x\inc\usbd" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_lpc_chip_175x_6x\Release" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_lpc_board_nxp_lpcxpresso_1769\Release" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\FreeRTOS" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\FreeRTOS\inc" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\FreeRTOS\FreeRTOS_Trace" -Og -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


