################################################################################
# MRS Version: 1.9.2
# �Զ����ɵ��ļ�����Ҫ�༭��
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/ch32v00x_it.c \
../User/main.c \
../User/system_ch32v00x.c 

OBJS += \
./User/ch32v00x_it.o \
./User/main.o \
./User/system_ch32v00x.o 

C_DEPS += \
./User/ch32v00x_it.d \
./User/main.d \
./User/system_ch32v00x.d 


# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\12630\Desktop\SmokingMachine\����\Debug" -I"C:\Users\12630\Desktop\SmokingMachine\����\Core" -I"C:\Users\12630\Desktop\SmokingMachine\����\User" -I"C:\Users\12630\Desktop\SmokingMachine\����\Peripheral\inc" -I"C:\Users\12630\Desktop\SmokingMachine\����\pwm" -I"C:\Users\12630\Desktop\SmokingMachine\����\screen" -I"C:\Users\12630\Desktop\SmokingMachine\����\encode" -I"C:\Users\12630\Desktop\SmokingMachine\����\Gpio" -I"C:\Users\12630\Desktop\SmokingMachine\����\Time" -I"C:\Users\12630\Desktop\SmokingMachine\����\adc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

