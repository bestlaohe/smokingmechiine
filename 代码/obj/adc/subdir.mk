################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adc/adc.c 

OBJS += \
./adc/adc.o 

C_DEPS += \
./adc/adc.d 


# Each subdirectory must supply rules for building sources it contributes
adc/%.o: ../adc/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\12630\Desktop\SmokingMachine\����\Debug" -I"C:\Users\12630\Desktop\SmokingMachine\����\Core" -I"C:\Users\12630\Desktop\SmokingMachine\����\User" -I"C:\Users\12630\Desktop\SmokingMachine\����\Peripheral\inc" -I"C:\Users\12630\Desktop\SmokingMachine\����\pwm" -I"C:\Users\12630\Desktop\SmokingMachine\����\screen" -I"C:\Users\12630\Desktop\SmokingMachine\����\encode" -I"C:\Users\12630\Desktop\SmokingMachine\����\Gpio" -I"C:\Users\12630\Desktop\SmokingMachine\����\Time" -I"C:\Users\12630\Desktop\SmokingMachine\����\adc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

