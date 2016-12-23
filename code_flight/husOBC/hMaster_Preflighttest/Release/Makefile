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
../ax25.c \
../functions.c \
../gps.c \
../hm.c \
../hMaster_Preflighttest.c \
../mag.c \
../peripherals.c \
../slave_comm.c \
../spi.c \
../sun.c \
../timer.c \
../twi.c \
../uart.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
ax25.o \
functions.o \
gps.o \
hm.o \
hMaster_Preflighttest.o \
mag.o \
peripherals.o \
slave_comm.o \
spi.o \
sun.o \
timer.o \
twi.o \
uart.o

OBJS_AS_ARGS +=  \
ax25.o \
functions.o \
gps.o \
hm.o \
hMaster_Preflighttest.o \
mag.o \
peripherals.o \
slave_comm.o \
spi.o \
sun.o \
timer.o \
twi.o \
uart.o

C_DEPS +=  \
ax25.d \
functions.d \
gps.d \
hm.d \
hMaster_Preflighttest.d \
mag.d \
peripherals.d \
slave_comm.d \
spi.d \
sun.d \
timer.d \
twi.d \
uart.d

C_DEPS_AS_ARGS +=  \
ax25.d \
functions.d \
gps.d \
hm.d \
hMaster_Preflighttest.d \
mag.d \
peripherals.d \
slave_comm.d \
spi.d \
sun.d \
timer.d \
twi.d \
uart.d

OUTPUT_FILE_PATH +=hMaster_Preflighttest.elf

OUTPUT_FILE_PATH_AS_ARGS +=hMaster_Preflighttest.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

# AVR32/GNU C Compiler



























./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 3.4.2
	$(QUOTE)E:\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -funsigned-char -funsigned-bitfields -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -mmcu=atmega128 -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<"
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
	@echo Invoking: AVR/GNU Linker : 3.4.2
	$(QUOTE)E:\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="hMaster_Preflighttest.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=atmega128 
	@echo Finished building target: $@
	"E:\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature  "hMaster_Preflighttest.elf" "hMaster_Preflighttest.hex"
	"E:\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "hMaster_Preflighttest.elf" "hMaster_Preflighttest.eep" || exit 0
	"E:\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "hMaster_Preflighttest.elf" > "hMaster_Preflighttest.lss"
	"E:\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature  "hMaster_Preflighttest.elf" "hMaster_Preflighttest.srec"
	"E:\Atmel Toolchain\AVR8 GCC\Native\3.4.2.939\avr8-gnu-toolchain\bin\avr-size.exe" "hMaster_Preflighttest.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "hMaster_Preflighttest.elf" "hMaster_Preflighttest.a" "hMaster_Preflighttest.hex" "hMaster_Preflighttest.lss" "hMaster_Preflighttest.eep" "hMaster_Preflighttest.map" "hMaster_Preflighttest.srec"
	