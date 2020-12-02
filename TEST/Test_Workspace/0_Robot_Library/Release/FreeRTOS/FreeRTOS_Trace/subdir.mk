################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FreeRTOS/FreeRTOS_Trace/trcHardwarePort.c \
../FreeRTOS/FreeRTOS_Trace/trcKernelPort.c \
../FreeRTOS/FreeRTOS_Trace/trcSnapshotRecorder.c 

OBJS += \
./FreeRTOS/FreeRTOS_Trace/trcHardwarePort.o \
./FreeRTOS/FreeRTOS_Trace/trcKernelPort.o \
./FreeRTOS/FreeRTOS_Trace/trcSnapshotRecorder.o 

C_DEPS += \
./FreeRTOS/FreeRTOS_Trace/trcHardwarePort.d \
./FreeRTOS/FreeRTOS_Trace/trcKernelPort.d \
./FreeRTOS/FreeRTOS_Trace/trcSnapshotRecorder.d 


# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/FreeRTOS_Trace/%.o: ../FreeRTOS/FreeRTOS_Trace/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -D__NEWLIB__ -DCORE_M3 -D__LPC17XX__ -DCR_INTEGER_PRINTF -D__USE_LPCOPEN -DNO_BOARD_LIB -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\0_lpc_board\inc" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\0_lpc_chip\inc" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\0_lpc_chip\inc\usbd" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\0_lpc_chip\Release" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\0_lpc_board\Release" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\FreeRTOS" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\FreeRTOS\inc" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\FreeRTOS\FreeRTOS_Trace" -I"C:\Robot\Robotic-System.git\trunk\TEST\Test_Workspace\0_Robot_Library\inc" -Og -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m3 -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


