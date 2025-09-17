################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
e:/智能车代码/Project/libraries/sdk/Core/core_riscv.c 

C_DEPS += \
./sdk/Core/core_riscv.d 

OBJS += \
./sdk/Core/core_riscv.o 


EXPANDS += \
./sdk/Core/core_riscv.c.234r.expand 



# Each subdirectory must supply rules for building sources it contributes
sdk/Core/core_riscv.o: e:/智能车代码/Project/libraries/sdk/Core/core_riscv.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

