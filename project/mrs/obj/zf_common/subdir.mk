################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
e:/智能车代码/Project/libraries/zf_common/zf_common_clock.c \
e:/智能车代码/Project/libraries/zf_common/zf_common_debug.c \
e:/智能车代码/Project/libraries/zf_common/zf_common_fifo.c \
e:/智能车代码/Project/libraries/zf_common/zf_common_font.c \
e:/智能车代码/Project/libraries/zf_common/zf_common_function.c \
e:/智能车代码/Project/libraries/zf_common/zf_common_interrupt.c 

C_DEPS += \
./zf_common/zf_common_clock.d \
./zf_common/zf_common_debug.d \
./zf_common/zf_common_fifo.d \
./zf_common/zf_common_font.d \
./zf_common/zf_common_function.d \
./zf_common/zf_common_interrupt.d 

OBJS += \
./zf_common/zf_common_clock.o \
./zf_common/zf_common_debug.o \
./zf_common/zf_common_fifo.o \
./zf_common/zf_common_font.o \
./zf_common/zf_common_function.o \
./zf_common/zf_common_interrupt.o 


EXPANDS += \
./zf_common/zf_common_clock.c.234r.expand \
./zf_common/zf_common_debug.c.234r.expand \
./zf_common/zf_common_fifo.c.234r.expand \
./zf_common/zf_common_font.c.234r.expand \
./zf_common/zf_common_function.c.234r.expand \
./zf_common/zf_common_interrupt.c.234r.expand 



# Each subdirectory must supply rules for building sources it contributes
zf_common/zf_common_clock.o: e:/智能车代码/Project/libraries/zf_common/zf_common_clock.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_common/zf_common_debug.o: e:/智能车代码/Project/libraries/zf_common/zf_common_debug.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_common/zf_common_fifo.o: e:/智能车代码/Project/libraries/zf_common/zf_common_fifo.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_common/zf_common_font.o: e:/智能车代码/Project/libraries/zf_common/zf_common_font.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_common/zf_common_function.o: e:/智能车代码/Project/libraries/zf_common/zf_common_function.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_common/zf_common_interrupt.o: e:/智能车代码/Project/libraries/zf_common/zf_common_interrupt.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

