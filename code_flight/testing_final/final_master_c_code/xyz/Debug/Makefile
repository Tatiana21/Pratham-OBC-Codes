################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../comm.c \
../controller.c \
../frame.c \
../gps.c \
../hm.c \
../igrf.c \
../mag.c \
../mathutil.c \
../peripherals.c \
../propagator.c \
../quest.c \
../spi.c \
../sun.c \
../timer.c \
../uart.c \
../xyz.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
comm.o \
controller.o \
frame.o \
gps.o \
hm.o \
igrf.o \
mag.o \
mathutil.o \
peripherals.o \
propagator.o \
quest.o \
spi.o \
sun.o \
timer.o \
uart.o \
xyz.o

OBJS_AS_ARGS +=  \
comm.o \
controller.o \
frame.o \
gps.o \
hm.o \
igrf.o \
mag.o \
mathutil.o \
peripherals.o \
propagator.o \
quest.o \
spi.o \
sun.o \
timer.o \
uart.o \
xyz.o

C_DEPS +=  \
comm.d \
controller.d \
frame.d \
gps.d \
hm.d \
igrf.d \
mag.d \
mathutil.d \
peripherals.d \
propagator.d \
quest.d \
spi.d \
sun.d \
timer.d \
uart.d \
xyz.d

C_DEPS_AS_ARGS +=  \
comm.d \
controller.d \
frame.d \
gps.d \
hm.d \
igrf.d \
mag.d \
mathutil.d \
peripherals.d \
propagator.d \
quest.d \
spi.d \
sun.d \
timer.d \
uart.d \
xyz.d

OUTPUT_FILE_PATH +=xyz_6_2.elf

OUTPUT_FILE_PATH_AS_ARGS +=xyz_6_2.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler

































./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=atmega128 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.8.1
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="xyz_6_2.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mrelax -mmcu=atmega128  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "xyz_6_2.elf" "xyz_6_2.hex"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "xyz_6_2.elf" "xyz_6_2.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "xyz_6_2.elf" > "xyz_6_2.lss"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "xyz_6_2.elf" "xyz_6_2.srec"
	"C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\bin\avr-size.exe" "xyz_6_2.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "xyz_6_2.elf" "xyz_6_2.a" "xyz_6_2.hex" "xyz_6_2.lss" "xyz_6_2.eep" "xyz_6_2.map" "xyz_6_2.srec" "xyz_6_2.usersignatures"
	