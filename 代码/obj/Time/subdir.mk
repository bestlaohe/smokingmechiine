################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
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
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\12630\Desktop\newmsg\代码\Debug" -I"C:\Users\12630\Desktop\newmsg\代码\Core" -I"C:\Users\12630\Desktop\newmsg\代码\User" -I"C:\Users\12630\Desktop\newmsg\代码\Peripheral\inc" -I"C:\Users\12630\Desktop\newmsg\代码\pwm" -I"C:\Users\12630\Desktop\newmsg\代码\screen" -I"C:\Users\12630\Desktop\newmsg\代码\encode" -I"C:\Users\12630\Desktop\newmsg\代码\Gpio" -I"C:\Users\12630\Desktop\newmsg\代码\Time" -I"C:\Users\12630\Desktop\newmsg\代码\adc" -I"C:\Users\12630\Desktop\newmsg\代码\Lora" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

