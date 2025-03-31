################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../screen/GUI_Paint.c \
../screen/drive_screen.c \
../screen/font12.c \
../screen/font16.c \
../screen/font24.c \
../screen/font8.c \
../screen/page.c \
../screen/screen_api.c 

OBJS += \
./screen/GUI_Paint.o \
./screen/drive_screen.o \
./screen/font12.o \
./screen/font16.o \
./screen/font24.o \
./screen/font8.o \
./screen/page.o \
./screen/screen_api.o 

C_DEPS += \
./screen/GUI_Paint.d \
./screen/drive_screen.d \
./screen/font12.d \
./screen/font16.d \
./screen/font24.d \
./screen/font8.d \
./screen/page.d \
./screen/screen_api.d 


# Each subdirectory must supply rules for building sources it contributes
screen/%.o: ../screen/%.c
	@	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"C:\Users\12630\Desktop\newmsg\代码\Debug" -I"C:\Users\12630\Desktop\newmsg\代码\Core" -I"C:\Users\12630\Desktop\newmsg\代码\User" -I"C:\Users\12630\Desktop\newmsg\代码\Peripheral\inc" -I"C:\Users\12630\Desktop\newmsg\代码\pwm" -I"C:\Users\12630\Desktop\newmsg\代码\screen" -I"C:\Users\12630\Desktop\newmsg\代码\encode" -I"C:\Users\12630\Desktop\newmsg\代码\Gpio" -I"C:\Users\12630\Desktop\newmsg\代码\Time" -I"C:\Users\12630\Desktop\newmsg\代码\adc" -I"C:\Users\12630\Desktop\newmsg\代码\Lora" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

