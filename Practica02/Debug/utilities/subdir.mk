################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_debug_console.c 

OBJS += \
./utilities/fsl_debug_console.o 

C_DEPS += \
./utilities/fsl_debug_console.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -DCR_INTEGER_PRINTF -DFREEDOM -DFRDM_KL25Z -DDEBUG -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -I"C:\Users\Manuel\Desktop\Practica03\Practica02\board" -I"C:\Users\Manuel\Desktop\Practica03\Practica02\source" -I"C:\Users\Manuel\Desktop\Practica03\Practica02" -I"C:\Users\Manuel\Desktop\Practica03\Practica02\drivers" -I"C:\Users\Manuel\Desktop\Practica03\Practica02\utilities" -I"C:\Users\Manuel\Desktop\Practica03\Practica02\CMSIS" -I"C:\Users\Manuel\Desktop\Practica03\Practica02\startup" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


