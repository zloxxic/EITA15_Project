################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

S_SRCS += \
../src/microBlaze.s 

C_SRCS += \
../src/Board.c \
../src/Tile.c \
../src/displays.c \
../src/main.c \
../src/vga.c 

OBJS += \
./src/Board.o \
./src/Tile.o \
./src/displays.o \
./src/main.o \
./src/microBlaze.o \
./src/vga.o 

C_DEPS += \
./src/Board.d \
./src/Tile.d \
./src/displays.d \
./src/main.d \
./src/vga.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../EDI610_VGA_Demo_bsp/microblaze_0/include -mlittle-endian -mcpu=v9.6 -mxl-soft-mul -Wl,--no-relax -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc assembler'
	mb-as -mlittle-endian -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


