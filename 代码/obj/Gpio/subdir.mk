################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Gpio/gpio.c 

OBJS += \
./Gpio/gpio.o 

C_DEPS += \
./Gpio/gpio.d 


# Each subdirectory must supply rules for building sources it contributes
Gpio/%.o: ../Gpio/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\12630\Desktop\newmsg\����\Debug" -I"C:\Users\12630\Desktop\newmsg\����\Core" -I"C:\Users\12630\Desktop\newmsg\����\User" -I"C:\Users\12630\Desktop\newmsg\����\Peripheral\inc" -I"C:\Users\12630\Desktop\newmsg\����\pwm" -I"C:\Users\12630\Desktop\newmsg\����\screen" -I"C:\Users\12630\Desktop\newmsg\����\encode" -I"C:\Users\12630\Desktop\newmsg\����\Gpio" -I"C:\Users\12630\Desktop\newmsg\����\Time" -I"C:\Users\12630\Desktop\newmsg\����\adc" -I"C:\Users\12630\Desktop\newmsg\����\Lora" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

