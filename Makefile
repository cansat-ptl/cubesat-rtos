# Recursive makefile wildcard
# https://stackoverflow.com/a/12959764
rwildcard=$(wildcard $(addsuffix $2, $1)) $(foreach d,$(wildcard $(addsuffix *, $1)),$(call rwildcard,$d/,$2))

# Project settings
TARG = yktsat-rtos
MCU=atmega128

# Libraries & includes
INCLUDES = -I"./include"

# Project directories
SRCDIR = kernel
BUILDDIR = build
TARGDIR = bin

# Compiler & linker settings
CC = avr-gcc
ASM = avr-gcc

# AVR-specific tool settings
OBJCOPY_CMD = avr-objcopy
OBJDUMP_CMD = avr-objdump
AVR_SIZE_CMD = avr-size 

# Host platform settings
RM_CMD = C:\Tools\Coreutils\bin\rm -f
MKDIR_CMD = C:\Tools\Coreutils\bin\mkdir.exe -p
 
# Compiler & linker flags
CFLAGS = -x c -funsigned-char -funsigned-bitfields -DDEBUG $(INCLUDES) -O2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall -mmcu=$(MCU) -c -std=gnu99
ASMFLAGS = -Wa,-gdwarf2 -x assembler-with-cpp -c -B -DDEBUG $(INCLUDES) -O2 -mrelax -g2 -mmcu=$(MCU) 
LDFLAGS = -Wl,-static -Wl,-Map="$(TARGDIR)/$(TARG).map" -Wl,-u,vfprintf -Wl,--start-group -Wl,-lm	-Wl,--end-group -Wl,--gc-sections -mrelax -Wl,-section-start=.bootloader=0x3c000 -mmcu=$(MCU) -Wl,-u,vfprintf -lprintf_flt -lm

# AVR-specific tool flags
OBJCOPY_HEX_FLAGS = -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures
OBJCOPY_EEP_FLAGS = -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex
OBJCOPY_SIGN_FLAGS = -j .user_signatures --set-section-flags=.user_signatures=alloc,load --change-section-lma .user_signatures=0 --no-change-warnings -O ihex
OBJCOPY_SREC_FLAGS = -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures
OBJDUMP_LSS_FLAGS = -h -S

#-------------------------------------------------------------------------------
#							DO NOT EDIT BELOW THIS LINE
#-------------------------------------------------------------------------------

# Automatically find all source files
SRCS = $(call rwildcard, $(SRCDIR)/,*.c) main.c
ASM_SRCS = $(call rwildcard, $(SRCDIR)/,*.S)

# Generate build directory structure
DIRS = $(addprefix $(BUILDDIR)/,$(dir $(SRCS)))

# Generate list of objects
OBJS = $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o)) $(addprefix $(BUILDDIR)/,$(ASM_SRCS:.S=.o))
 
all: dirs $(TARG)

# Create build directories
dirs:
	$(MKDIR_CMD) $(BUILDDIR)
	$(MKDIR_CMD) $(TARGDIR)
	$(MKDIR_CMD) $(DIRS)

# Compile target
$(TARG): $(OBJS)
	$(CC) -o $(TARGDIR)/$@.elf $(OBJS) $(LDFLAGS)
	$(OBJCOPY_CMD)  $(OBJCOPY_HEX_FLAGS) "$(TARGDIR)/$(TARG).elf" "$(TARGDIR)/$(TARG).hex"
	$(OBJCOPY_CMD)  $(OBJCOPY_EEP_FLAGS) "$(TARGDIR)/$(TARG).elf" "$(TARGDIR)/$(TARG).eep" || exit 0
	$(OBJDUMP_CMD)  $(OBJDUMP_LSS_FLAGS) "$(TARGDIR)/$(TARG).elf" > "$(TARGDIR)/$(TARG).lss"
	$(OBJCOPY_CMD)  $(OBJCOPY_SREC_FLAGS) "$(TARGDIR)/$(TARG).elf" "$(TARGDIR)/$(TARG).srec"
	$(OBJCOPY_CMD)  $(OBJCOPY_SIGN_FLAGS) "$(TARGDIR)/$(TARG).elf" "$(TARGDIR)/$(TARG).usersignatures" || exit 0
	$(AVR_SIZE_CMD) "$(TARGDIR)/$(TARG).elf"

# Compile objects
$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: %.S
	$(ASM) $(ASMFLAGS) -c -o $@ $<

# Clean
cleanobjs:
	$(RM_CMD) $(OBJS)

cleanmisc:
	$(RM_CMD) $(TARGDIR)/$(TARG).map $(TARGDIR)/$(TARG).usersignatures $(TARGDIR)/$(TARG).srec $(TARGDIR)/$(TARG).lss $(TARGDIR)/$(TARG).eep

clean: cleanobjs cleanmisc
	$(RM_CMD) $(TARGDIR)/$(TARG).elf $(TARGDIR)/$(TARG).bin $(TARGDIR)/$(TARG).hex