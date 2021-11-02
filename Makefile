# Project settings
TARG := rtos
TEST_TARG := yktsat-rtos
ARCH_DEFAULT := avr
MCU_DEFAULT := atmega128

# Libraries & includes
INCLUDES := -I"./include"

# Project directories
SRCDIR := kernel
TESTDIR := tests
BUILDDIR := build
TARGDIR := bin

# Compiler & linker settings
CC := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe"
ASM := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe"
AR := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-ar.exe"

# AVR-specific tool settings
OBJCOPY_CMD := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe"
OBJDUMP_CMD := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe"
AVR_SIZE_CMD := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe"

# Host platform settings
RM_CMD := C:/Tools/Coreutils/bin/rm -rf
MKDIR_CMD := C:/Tools/Coreutils/bin/mkdir.exe -p
COPY_CMD := C:/Tools/Coreutils/bin/cp.exe

VERSION_SEMANTIC := 0.0.1
VERSION_GIT_HASH := `git rev-parse HEAD | cut -c1-8`
VERSION_GIT_BRANCH := `git rev-parse --abbrev-ref HEAD`
 
# Compiler & linker flags
CFLAGS := -x c -DDEBUG $(INCLUDES) -Os -g2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -c -std=gnu99 -Wall -Wextra
ASMFLAGS := -Wa,-gdwarf2 -x assembler-with-cpp -c -B -DDEBUG $(INCLUDES) -Os -g2 -mrelax -Wall -Wextra
LDFLAGS := -Wl,-static -Wl,-Map="$(TARGDIR)/$(TARG).map" -Wl,--gc-sections -mrelax -Wl,-section-start=.bootloader=0x3c000 -lm -Wall -Wextra

# AVR-specific tool flags
OBJCOPY_HEX_FLAGS := -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures
OBJCOPY_EEP_FLAGS := -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex
OBJCOPY_SIGN_FLAGS := -j .user_signatures --set-section-flags=.user_signatures=alloc,load --change-section-lma .user_signatures=0 --no-change-warnings -O ihex
OBJCOPY_SREC_FLAGS := -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures
OBJDUMP_LSS_FLAGS := -h -S

#-------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#-------------------------------------------------------------------------------

# Recursive makefile wildcard
# https://stackoverflow.com/a/12959764
rwildcard=$(wildcard $(addsuffix $2, $1)) $(foreach d,$(wildcard $(addsuffix *, $1)),$(call rwildcard,$d/,$2))

# Automatically find all source files
SRCS := $(call rwildcard, $(SRCDIR)/,*.c)
TEST_SRCS := $(call rwildcard, $(TESTDIR)/,*.c)
ASM_SRCS := $(call rwildcard, $(SRCDIR)/,*.S)

# Generate build directory structure
DIRS := $(addprefix $(BUILDDIR)/,$(dir $(SRCS)))
TEST_DIRS := $(addprefix $(BUILDDIR)/,$(dir $(TEST_SRCS)))

# Generate list of objects
OBJS := $(addprefix $(BUILDDIR)/,$(SRCS:.c=.o)) $(addprefix $(BUILDDIR)/,$(ASM_SRCS:.S=.o))
TEST_OBJS := $(addprefix $(BUILDDIR)/,$(TEST_SRCS:.c=.o))

# Generate dependencies
DEPS := $(OBJS:.o=.d)
CFLAGS += -MMD -MP

VERSION_STRING := $(VERSION_SEMANTIC)-git-$(VERSION_GIT_BRANCH)-$(VERSION_GIT_HASH)

ifndef $(arch)
arch := $(ARCH_DEFAULT)
else
ifeq ($(strip $(arch)),)
	$(arch) := $(ARCH_DEFAULT)
endif
endif

ifndef $(mcu)
mcu := $(MCU_DEFAULT)
else
ifeq ($(strip $(mcu)),)
	$(mcu) := $(MCU_DEFAULT)
endif
endif
 
all: dirs $(TARG) $(TEST_TARG)

# Create build directories
dirs:
	$(MKDIR_CMD) $(BUILDDIR)
	$(MKDIR_CMD) $(TARGDIR)
ifneq ($(strip $(DIRS)),)
	$(MKDIR_CMD) $(DIRS)
endif
ifneq ($(strip $(TEST_DIRS)),)
	$(MKDIR_CMD) $(TEST_DIRS)
endif

# Compile main target
$(TARG): $(OBJS)
	$(AR) rcs "$(TARGDIR)/lib$(TARG).a" $(OBJS)
	$(COPY_CMD) "$(TARGDIR)/lib$(TARG).a" "./lib$(TARG).a"

# Compile test target
$(TEST_TARG): $(TARG) $(TEST_OBJS)
	$(CC) -o $(TARGDIR)/$@.elf $(OBJS) $(TEST_OBJS) $(LDFLAGS) -DVERSION_STRING="\"$(VERSION_STRING)\"" -DKERNEL_ARCH_$(arch) -DKERNEL_MCU_$(mcu) -mmcu=$(mcu) -L. -l$(TARG)
	$(OBJCOPY_CMD)  $(OBJCOPY_HEX_FLAGS) "$(TARGDIR)/$(TEST_TARG).elf" "$(TARGDIR)/$(TEST_TARG).hex"
	$(OBJCOPY_CMD)  $(OBJCOPY_EEP_FLAGS) "$(TARGDIR)/$(TEST_TARG).elf" "$(TARGDIR)/$(TEST_TARG).eep" || exit 0
	$(OBJDUMP_CMD)  $(OBJDUMP_LSS_FLAGS) "$(TARGDIR)/$(TEST_TARG).elf" > "$(TARGDIR)/$(TEST_TARG).lss"
	$(OBJCOPY_CMD)  $(OBJCOPY_SREC_FLAGS) "$(TARGDIR)/$(TEST_TARG).elf" "$(TARGDIR)/$(TEST_TARG).srec"
	$(OBJCOPY_CMD)  $(OBJCOPY_SIGN_FLAGS) "$(TARGDIR)/$(TEST_TARG).elf" "$(TARGDIR)/$(TEST_TARG).usersignatures" || exit 0
	$(AVR_SIZE_CMD) "$(TARGDIR)/$(TEST_TARG).elf"
	$(COPY_CMD) "$(TARGDIR)/$(TEST_TARG).elf" "./$(TEST_TARG).elf"

# Compile objects
$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -DVERSION_STRING="\"$(VERSION_STRING)\"" -DKERNEL_ARCH_$(arch) -DKERNEL_MCU_$(mcu) -mmcu=$(mcu) -c -o $@ $<

$(BUILDDIR)/%.o: %.S
	$(ASM) $(ASMFLAGS) -DVERSION_STRING="\"$(VERSION_STRING)\"" -DKERNEL_ARCH_$(arch) -DKERNEL_MCU_$(mcu) -mmcu=$(mcu) -c -o $@ $<

# Clean
cleandirs:
	$(RM_CMD) $(TARGDIR) $(BUILDDIR)

cleanobjs:
	$(RM_CMD) $(OBJS) $(DEPS) $(TEST_OBJS)

cleanmisc:
	$(RM_CMD) $(TARGDIR)/$(TEST_TARG).map $(TARGDIR)/$(TEST_TARG).usersignatures $(TARGDIR)/$(TEST_TARG).srec $(TARGDIR)/$(TEST_TARG).lss $(TARGDIR)/$(TEST_TARG).eep

clean: cleanobjs cleanmisc cleandirs
	$(RM_CMD) $(TARGDIR)/$(TEST_TARG).elf $(TEST_TARG).elf $(TARGDIR)/$(TEST_TARG).bin $(TARGDIR)/$(TEST_TARG).hex lib$(TARG).a $(TARGDIR)/lib$(TARG).a

-include $(DEPS)