################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
src/usr/ff09b/src/ff.obj: ../src/usr/ff09b/src/ff.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv5/tools/compiler/arm_5.1.1/include" --include_path="C:/ti/TivaWare_C_Series-2.1.0.12573--" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/sys" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff09b/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/src/usr/ff11a/src" --include_path="F:/Dropbox/myShared/Engenharia/Codigos/Tiva-C/PSO/inc/usr/ulink" -g --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="src/usr/ff09b/src/ff.pp" --obj_directory="src/usr/ff09b/src" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


