################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Time/time_app.c 

OBJS += \
./Time/time_app.o 

C_DEPS += \
./Time/time_app.d 


# Each subdirectory must supply rules for building sources it contributes
Time/%.o: ../Time/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\12630\Desktop\newmsg\����\Debug" -I"C:\Users\12630\Desktop\newmsg\����\Core" -I"C:\Users\12630\Desktop\newmsg\����\User" -I"C:\Users\12630\Desktop\newmsg\����\Peripheral\inc" -I"C:\Users\12630\Desktop\newmsg\����\pwm" -I"C:\Users\12630\Desktop\newmsg\����\screen" -I"C:\Users\12630\Desktop\newmsg\����\encode" -I"C:\Users\12630\Desktop\newmsg\����\Gpio" -I"C:\Users\12630\Desktop\newmsg\����\Time" -I"C:\Users\12630\Desktop\newmsg\����\adc" -I"C:\Users\12630\Desktop\newmsg\����\Lora" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

