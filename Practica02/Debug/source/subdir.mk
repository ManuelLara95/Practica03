################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/main_P02.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/main_P02.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/main_P02.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DPRINTF_FLOAT_ENABLE=0 -DCR_INTEGER_PRINTF -DFREEDOM -DFRDM_KL25Z -DDEBUG -DSDK_DEBUGCONSOLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -I"C:\Users\Manuel\Desktop\Practicas\Practica02\board" -I"C:\Users\Manuel\Desktop\Practicas\Practica02\source" -I"C:\Users\Manuel\Desktop\Practicas\Practica02" -I"C:\Users\Manuel\Desktop\Practicas\Practica02\drivers" -I"C:\Users\Manuel\Desktop\Practicas\Practica02\utilities" -I"C:\Users\Manuel\Desktop\Practicas\Practica02\CMSIS" -I"C:\Users\Manuel\Desktop\Practicas\Practica02\startup" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


