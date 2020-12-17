# Make does not offer a recursive wildcard function, so here's one:
rwildcard=$(wildcard $(addsuffix $2, $1)) $(foreach d,$(wildcard $(addsuffix *, $1)),$(call rwildcard,$d/,$2))

# How to recursively find all files that match a pattern
#ALL_C_FILES := $(call rwildcard,$(SRCDIR)/,*.c)

TARG = yktsat-rtos
 
CC = "D:\BuildTools\avr-gcc-9.2.0-x64-mingw\bin\avr-gcc.exe"
OBJCOPY = "D:\BuildTools\avr-gcc-9.2.0-x64-mingw\bin\avr-objcopy.exe"
OBJDUMP = "D:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe"
ATMEL_DFP = "D:\Program Files (x86)\Atmel\Studio\7.0\Packs\Atmel\ATmega_DFP\1.4.346\include"

AVR_SIZE_CMD = "D:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "$(TARG).elf"
 
INC = -I"include"
SRCDIR = kernel
SRCS = $(call rwildcard, $(SRCDIR)/,*.c)
 
OBJS = $(SRCS:.c=.o)

MCU=atmega128
 
CFLAGS = -x c -funsigned-char -funsigned-bitfields -DDEBUG	-I$(ATMEL_DFP) $(INC) -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=$(MCU) -c -std=gnu99

LDFLAGS = -Wl,-static -Wl,-Map="yktsat-rtos.map" -Wl,-u,vfprintf -Wl,--start-group -Wl,-lm	-Wl,--end-group -Wl,--gc-sections -mrelax -Wl,-section-start=.bootloader=0x3c000   -mmcu=$(MCU)	 -Wl,-u,vfprintf -lprintf_flt -lm

OBJCOPY_HEX_FLAGS = -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures

OBJCOPY_EEP_FLAGS = -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex

OBJCOPY_SIGN_FLAGS = -j .user_signatures --set-section-flags=.user_signatures=alloc,load --change-section-lma .user_signatures=0 --no-change-warnings -O ihex

OBJCOPY_SREC_FLAGS = -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures

OBJDUMP_LSS_FLAGS = -h -S
 
all: $(TARG)
	
$(TARG): $(OBJS)
	$(CC) -o $@.elf $(OBJS) $(LDFLAGS)
	$(OBJCOPY)  $(OBJCOPY_HEX_FLAGS) "$(TARG).elf" "$(TARG).hex"
	$(OBJCOPY)  $(OBJCOPY_EEP_FLAGS) "$(TARG).elf" "$(TARG).eep" || exit 0
	$(OBJDUMP)  $(OBJDUMP_LSS_FLAGS) "$(TARG).elf" > "$(TARG).lss"
	$(OBJCOPY)  $(OBJCOPY_SREC_FLAGS) "$(TARG).elf" "$(TARG).srec"
	$(OBJCOPY)  $(OBJCOPY_SIGN_FLAGS) "$(TARG).elf" "$(TARG).usersignatures" || exit 0
	$(AVR_SIZE_CMD)
 
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
 
clean:
	rm -f $(TARG).elf $(TARG).bin $(TARG).hex  $(OBJS) $(TARG).map $(TARG).usersignatures $(TARG).srec $(TARG).lss $(TARG).eep 