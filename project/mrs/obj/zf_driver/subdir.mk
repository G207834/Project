################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_adc.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_delay.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_dvp.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_encoder.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_exti.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_flash.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_gpio.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_iic.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_pit.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_pwm.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_soft_iic.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_soft_spi.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_spi.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_timer.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_uart.c \
e:/智能车代码/Project/libraries/zf_driver/zf_driver_usb_cdc.c 

C_DEPS += \
./zf_driver/zf_driver_adc.d \
./zf_driver/zf_driver_delay.d \
./zf_driver/zf_driver_dvp.d \
./zf_driver/zf_driver_encoder.d \
./zf_driver/zf_driver_exti.d \
./zf_driver/zf_driver_flash.d \
./zf_driver/zf_driver_gpio.d \
./zf_driver/zf_driver_iic.d \
./zf_driver/zf_driver_pit.d \
./zf_driver/zf_driver_pwm.d \
./zf_driver/zf_driver_soft_iic.d \
./zf_driver/zf_driver_soft_spi.d \
./zf_driver/zf_driver_spi.d \
./zf_driver/zf_driver_timer.d \
./zf_driver/zf_driver_uart.d \
./zf_driver/zf_driver_usb_cdc.d 

OBJS += \
./zf_driver/zf_driver_adc.o \
./zf_driver/zf_driver_delay.o \
./zf_driver/zf_driver_dvp.o \
./zf_driver/zf_driver_encoder.o \
./zf_driver/zf_driver_exti.o \
./zf_driver/zf_driver_flash.o \
./zf_driver/zf_driver_gpio.o \
./zf_driver/zf_driver_iic.o \
./zf_driver/zf_driver_pit.o \
./zf_driver/zf_driver_pwm.o \
./zf_driver/zf_driver_soft_iic.o \
./zf_driver/zf_driver_soft_spi.o \
./zf_driver/zf_driver_spi.o \
./zf_driver/zf_driver_timer.o \
./zf_driver/zf_driver_uart.o \
./zf_driver/zf_driver_usb_cdc.o 


EXPANDS += \
./zf_driver/zf_driver_adc.c.234r.expand \
./zf_driver/zf_driver_delay.c.234r.expand \
./zf_driver/zf_driver_dvp.c.234r.expand \
./zf_driver/zf_driver_encoder.c.234r.expand \
./zf_driver/zf_driver_exti.c.234r.expand \
./zf_driver/zf_driver_flash.c.234r.expand \
./zf_driver/zf_driver_gpio.c.234r.expand \
./zf_driver/zf_driver_iic.c.234r.expand \
./zf_driver/zf_driver_pit.c.234r.expand \
./zf_driver/zf_driver_pwm.c.234r.expand \
./zf_driver/zf_driver_soft_iic.c.234r.expand \
./zf_driver/zf_driver_soft_spi.c.234r.expand \
./zf_driver/zf_driver_spi.c.234r.expand \
./zf_driver/zf_driver_timer.c.234r.expand \
./zf_driver/zf_driver_uart.c.234r.expand \
./zf_driver/zf_driver_usb_cdc.c.234r.expand 



# Each subdirectory must supply rules for building sources it contributes
zf_driver/zf_driver_adc.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_adc.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_delay.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_delay.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_dvp.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_dvp.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_encoder.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_encoder.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_exti.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_exti.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_flash.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_flash.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_gpio.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_gpio.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_iic.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_iic.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_pit.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_pit.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_pwm.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_pwm.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_soft_iic.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_soft_iic.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_soft_spi.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_soft_spi.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_spi.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_spi.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_timer.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_timer.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_uart.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_uart.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
zf_driver/zf_driver_usb_cdc.o: e:/智能车代码/Project/libraries/zf_driver/zf_driver_usb_cdc.c
	@	riscv-none-embed-gcc -march=rv32imafc -mabi=ilp32f -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -pedantic -Wunused -Wuninitialized -Wall -g -I"e:/智能车代码/Project/Libraries/doc" -I"e:/智能车代码/Project/libraries/zf_components" -I"e:/智能车代码/Project/libraries/sdk/Core" -I"e:/智能车代码/Project/libraries/sdk/Ld" -I"e:/智能车代码/Project/libraries/sdk/Peripheral" -I"e:/智能车代码/Project/libraries/sdk/Startup" -I"e:/智能车代码/Project/project/user/inc" -I"e:/智能车代码/Project/libraries/zf_common" -I"e:/智能车代码/Project/libraries/zf_device" -I"e:/智能车代码/Project/project/code" -I"e:/智能车代码/Project/libraries/zf_driver" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

