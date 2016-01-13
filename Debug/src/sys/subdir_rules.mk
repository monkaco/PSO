################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/sys/adc.obj: ../src/sys/adc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573--" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/sys" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff09b/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff11a/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr/ulink" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/sys/adc.pp" --obj_directory="src/sys" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/sys/crc.obj: ../src/sys/crc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573--" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/sys" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff09b/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff11a/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr/ulink" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/sys/crc.pp" --obj_directory="src/sys" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/sys/flash.obj: ../src/sys/flash.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573--" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/sys" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff09b/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff11a/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr/ulink" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/sys/flash.pp" --obj_directory="src/sys" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/sys/gpio.obj: ../src/sys/gpio.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573--" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/sys" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff09b/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff11a/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr/ulink" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/sys/gpio.pp" --obj_directory="src/sys" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/sys/pwm.obj: ../src/sys/pwm.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573--" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/sys" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff09b/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff11a/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr/ulink" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/sys/pwm.pp" --obj_directory="src/sys" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/sys/systick.obj: ../src/sys/systick.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573--" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/sys" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff09b/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff11a/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr/ulink" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/sys/systick.pp" --obj_directory="src/sys" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/sys/tm4c123gh6pm_startup_ccs.obj: ../src/sys/tm4c123gh6pm_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573--" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/sys" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff09b/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff11a/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr/ulink" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/sys/tm4c123gh6pm_startup_ccs.pp" --obj_directory="src/sys" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/sys/uart.obj: ../src/sys/uart.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573--" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/sys" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff09b/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff11a/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr/ulink" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/sys/uart.pp" --obj_directory="src/sys" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/sys/watchdog.obj: ../src/sys/watchdog.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573--" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/sys" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff09b/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff11a/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr/ulink" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/sys/watchdog.pp" --obj_directory="src/sys" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


