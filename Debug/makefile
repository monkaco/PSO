################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/arm_5.1.1

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./src/sys/watchdog.obj" \
"./src/sys/uart.obj" \
"./src/sys/tm4c123gh6pm_startup_ccs.obj" \
"./src/sys/systick.obj" \
"./src/sys/pwm.obj" \
"./src/sys/gpio.obj" \
"./src/sys/flash.obj" \
"./src/sys/crc.obj" \
"./src/sys/adc.obj" \
"./src/usr/ff09b/mmc-dk-tm4c123g.obj" \
"./src/usr/ff09b/src/ff.obj" \
"./src/usr/ff09b/src/option/unicode.obj" \
"./src/usr/ff09b/src/option/syscall.obj" \
"./src/usr/ulink_bridge.obj" \
"./src/usr/ulink.obj" \
"./src/usr/pso_uart.obj" \
"./src/usr/pso_pwm.obj" \
"./src/usr/pso_led.obj" \
"./src/usr/pso_isr.obj" \
"./src/usr/pso_init.obj" \
"./src/usr/pso_data.obj" \
"./src/usr/main.obj" \
"../library/driverlib.lib" \
"../cmd/tm4c123gh6pm.cmd" \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include src/usr/subdir_vars.mk
-include src/usr/ff09b/src/option/subdir_vars.mk
-include src/usr/ff09b/src/subdir_vars.mk
-include src/usr/ff09b/subdir_vars.mk
-include src/sys/subdir_vars.mk
-include library/subdir_vars.mk
-include cmd/subdir_vars.mk
-include src/usr/subdir_rules.mk
-include src/usr/ff09b/src/option/subdir_rules.mk
-include src/usr/ff09b/src/subdir_rules.mk
-include src/usr/ff09b/subdir_rules.mk
-include src/sys/subdir_rules.mk
-include library/subdir_rules.mk
-include cmd/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: PSO.out

# Tool invocations
PSO.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off -z -m"PSO.map" --heap_size=800 --stack_size=800 -i"C:/ti/ccsv5/tools/compiler/arm_5.1.1/lib" -i"C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --reread_libs --verbose_diagnostics --warn_sections --display_error_number --diag_wrap=off --xml_link_info="PSO_linkInfo.xml" --rom_model -o "PSO.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(TMS470_EXECUTABLE_OUTPUTS__QUOTED) "PSO.out"
	-$(RM) "src\usr\main.pp" "src\usr\pso_data.pp" "src\usr\pso_init.pp" "src\usr\pso_isr.pp" "src\usr\pso_led.pp" "src\usr\pso_pwm.pp" "src\usr\pso_uart.pp" "src\usr\ulink.pp" "src\usr\ulink_bridge.pp" "src\usr\ff09b\src\option\syscall.pp" "src\usr\ff09b\src\option\unicode.pp" "src\usr\ff09b\src\ff.pp" "src\usr\ff09b\mmc-dk-tm4c123g.pp" "src\sys\adc.pp" "src\sys\crc.pp" "src\sys\flash.pp" "src\sys\gpio.pp" "src\sys\pwm.pp" "src\sys\systick.pp" "src\sys\tm4c123gh6pm_startup_ccs.pp" "src\sys\uart.pp" "src\sys\watchdog.pp" 
	-$(RM) "src\usr\main.obj" "src\usr\pso_data.obj" "src\usr\pso_init.obj" "src\usr\pso_isr.obj" "src\usr\pso_led.obj" "src\usr\pso_pwm.obj" "src\usr\pso_uart.obj" "src\usr\ulink.obj" "src\usr\ulink_bridge.obj" "src\usr\ff09b\src\option\syscall.obj" "src\usr\ff09b\src\option\unicode.obj" "src\usr\ff09b\src\ff.obj" "src\usr\ff09b\mmc-dk-tm4c123g.obj" "src\sys\adc.obj" "src\sys\crc.obj" "src\sys\flash.obj" "src\sys\gpio.obj" "src\sys\pwm.obj" "src\sys\systick.obj" "src\sys\tm4c123gh6pm_startup_ccs.obj" "src\sys\uart.obj" "src\sys\watchdog.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

