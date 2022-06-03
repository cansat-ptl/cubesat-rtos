# Project settings
TARG := rtos
TEST_TARG := rtostest
ARCH_DEFAULT := avr
MCU_DEFAULT := atmega128
ARCH_TEST := stub
MCU_TEST := generic

# Libraries & includes
INCLUDES := -I"./include"

# Project directories
SRCDIR := kernel
TESTDIR := tests
UNITTESTDIR := unit
HWTESTDIR := platform
BUILDDIR := build
TARGDIR := bin

# Compiler & linker settings
CC := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe"
CPP = "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-g++.exe"
ASM := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe"
AR := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-ar.exe"

TEST_CC := gcc
TEST_CPP := g++
TEST_ASM := gcc
TEST_AR := ar

# AVR-specific tool settings
OBJCOPY_CMD := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe"
OBJDUMP_CMD := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe"
AVR_SIZE_CMD := "C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe"

# Host platform settings
RM_CMD := C:/Tools/Coreutils/bin/rm -rf
MKDIR_CMD := C:/Tools/Coreutils/bin/mkdir.exe -p
COPY_CMD := C:/Tools/Coreutils/bin/cp.exe

VERSION_SEMANTIC := 0.0.1
VERSION_GIT_HASH := $(shell git rev-parse --short HEAD)
VERSION_GIT_BRANCH := $(shell git rev-parse --abbrev-ref HEAD)
 
# Compiler & linker flags
CFLAGS = -x c -DDEBUG $(INCLUDES) -Os -g2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -c -std=gnu99 -Wall -Wextra -mmcu=$(mcu)
CPPFLAGS = -x c++ -DDEBUG $(INCLUDES) -Os -g2 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -c -std=c++11 -Wall -Wextra -mmcu=$(mcu)
ASMFLAGS = -Wa,-gdwarf2 -x assembler-with-cpp -c -B -DDEBUG $(INCLUDES) -Os -g2 -mrelax -Wall -Wextra -mmcu=$(mcu)
LDFLAGS = -Wl,-static -Wl,-Map="$(TARGDIR)/$(TARG).map" -Wl,--gc-sections -mrelax -Wl,-section-start=.bootloader=0x3c000 -lm -Wall -Wextra -mmcu=$(mcu)

TEST_CFLAGS := -x c -DDEBUG $(INCLUDES) -Os -g2 -c -std=gnu99 -Wall -Wextra
TEST_CPPFLAGS := -x c++ -DDEBUG $(INCLUDES) -Os -g2 -c -std=c++11 -Wall -Wextra 
TEST_ASMFLAGS := -Wa,-gdwarf2 -x assembler-with-cpp -c -B -DDEBUG $(INCLUDES) -Os -g2 -Wall -Wextra
TEST_LDFLAGS :=  -lm -Wall -Wextra -Wl,-undefined,dynamic_lookup

# CppUTest settings
CPPUTEST_HOME := "C:\Development\Sources\C\cpputest-3.8\cpputest_build"
TEST_CPPFLAGS += -I$(CPPUTEST_HOME)/include
TEST_LDFLAGS += -L$(CPPUTEST_HOME)/lib -lCppUTest -lCppUTestExt

# AVR-specific tool flags
OBJCOPY_HEX_FLAGS := -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures
OBJCOPY_EEP_FLAGS := -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex
OBJCOPY_SIGN_FLAGS := -j .user_signatures --set-section-flags=.user_signatures=alloc,load --change-section-lma .user_signatures=0 --no-change-warnings -O ihex
OBJCOPY_SREC_FLAGS := -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures
OBJDUMP_LSS_FLAGS := -h -S

#-------------------------------------------------------------------------------
# DO NOT EDIT BELOW THIS LINE
#-------------------------------------------------------------------------------

# Set default arch if not specified
ifndef $(arch)
arch := $(ARCH_DEFAULT)
else
ifeq ($(strip $(arch)),)
	$(arch) := $(ARCH_DEFAULT)
endif
endif

# Set default MCU if not specified
ifndef $(mcu)
mcu := $(MCU_DEFAULT)
else
ifeq ($(strip $(mcu)),)
	$(mcu) := $(MCU_DEFAULT)
endif
endif

# Recursive makefile wildcard
# https://stackoverflow.com/a/12959764
rwildcard = $(wildcard $(addsuffix $2, $1)) $(foreach d,$(wildcard $(addsuffix *, $1)),$(call rwildcard,$d/,$2))

TARG_BUILDDIR := $(BUILDDIR)/$(arch)/$(mcu)
TEST_BUILDDIR := $(BUILDDIR)/$(ARCH_TEST)/$(MCU_TEST)

# Automatically find all source files
C_SRCS := $(call rwildcard, $(SRCDIR)/,*.c)
CPP_SRCS := $(call rwildcard, $(SRCDIR)/,*.cpp) 
ASM_SRCS := $(call rwildcard, $(SRCDIR)/,*.S)

UNITTEST_CPP_SRCS := $(call rwildcard, $(TESTDIR)/$(UNITTESTDIR)/,*.cpp)
HWTEST_CPP_SRCS :=  $(call rwildcard, $(TESTDIR)/$(HWTESTDIR)/,*.cpp) 

# Generate build directory structure
TARG_OBJDIRS := $(addprefix $(TARG_BUILDDIR)/,$(dir $(C_SRCS))) $(addprefix $(TARG_BUILDDIR)/,$(dir $(CPP_SRCS)))
HWTEST_OBJDIRS :=  $(addprefix $(TARG_BUILDDIR)/,$(dir $(HWTEST_CPP_SRCS)))
UNITTEST_OBJDIRS := $(addprefix $(TEST_BUILDDIR)/,$(dir $(UNITTEST_CPP_SRCS))) $(addprefix $(TEST_BUILDDIR)/,$(dir $(C_SRCS))) $(addprefix $(TEST_BUILDDIR)/,$(dir $(CPP_SRCS)))

# Generate list of objects
TARG_OBJS := $(addprefix $(TARG_BUILDDIR)/,$(C_SRCS:.c=.o)) $(addprefix $(TARG_BUILDDIR)/,$(CPP_SRCS:.cpp=.o)) $(addprefix $(TARG_BUILDDIR)/,$(ASM_SRCS:.S=.o))
HWTEST_OBJS := $(addprefix $(TARG_BUILDDIR)/,$(HWTEST_CPP_SRCS:.cpp=.o))
UNITTEST_OBJS := $(addprefix $(TEST_BUILDDIR)/,$(UNITTEST_CPP_SRCS:.cpp=.o)) 
UNITTEST_OBJS += $(addprefix $(TEST_BUILDDIR)/,$(C_SRCS:.c=.o)) $(addprefix $(TEST_BUILDDIR)/,$(CPP_SRCS:.cpp=.o)) $(addprefix $(TEST_BUILDDIR)/,$(ASM_SRCS:.S=.o))

HWTEST_TARG := $(TARGDIR)/$(arch)/$(mcu)/$(TEST_TARG)

# Generate dependencies
DEPS := $(TARG_OBJS:.o=.d) $(HWTEST_OBJS:.o=.d) $(UNITTEST_OBJS:.o=.d)
CFLAGS += -MMD -MP
CPPFLAGS += -MMD -MP
TEST_CFLAGS += -MMD -MP
TEST_CPPFLAGS += -MMD -MP

VERSION_STRING := $(VERSION_SEMANTIC)-git-$(VERSION_GIT_BRANCH)-$(VERSION_GIT_HASH)
VERSION_FLAGS := -DVERSION_STRING="\"$(VERSION_STRING)\"" -DKERNEL_ARCH_$(arch) -DKERNEL_MCU_$(mcu)
TEST_VERSION_FLAGS := -DVERSION_STRING="\"$(VERSION_STRING)\"" -DKERNEL_ARCH_$(ARCH_TEST) -DKERNEL_MCU_$(MCU_TEST)

all: dirs $(TARG) $(TEST_TARG)

# Create build directories
dirs:
	$(MKDIR_CMD) $(BUILDDIR)
	$(MKDIR_CMD) $(TARGDIR)/$(arch)/$(mcu)
	$(MKDIR_CMD) $(TARGDIR)/$(ARCH_TEST)/$(MCU_TEST)
ifneq ($(strip $(TARG_OBJDIRS)),)
	$(MKDIR_CMD) $(TARG_OBJDIRS)
endif
ifneq ($(strip $(HWTEST_OBJDIRS)),)
	$(MKDIR_CMD) $(HWTEST_OBJDIRS)
endif
ifneq ($(strip $(UNITTEST_OBJDIRS)),)
	$(MKDIR_CMD) $(UNITTEST_OBJDIRS)
endif

test: dirs $(TARG) $(UNITTEST_OBJS)
	$(TEST_CPP) $(UNITTEST_OBJS) $(TEST_LDFLAGS) $(VERSION_FLAGS) -o $(TARGDIR)/$(ARCH_TEST)/$(MCU_TEST)/$@

# Compile main target
$(TARG): $(TARG_OBJS)
	$(AR) rcs "$(TARGDIR)/$(arch)/$(mcu)/lib$(TARG).a" $(TARG_OBJS)
	$(COPY_CMD) "$(TARGDIR)/$(arch)/$(mcu)/lib$(TARG).a" "./lib$(TARG).a"

# Compile test target
$(TEST_TARG): $(TARG) $(HWTEST_OBJS)
	$(CPP) -o $(HWTEST_TARG).elf $(TARG_OBJS) $(HWTEST_OBJS) $(LDFLAGS) $(VERSION_FLAGS) -mmcu=$(mcu) -L./$(TARGDIR)/$(arch)/$(mcu) -l$(TARG)
	$(OBJCOPY_CMD)  $(OBJCOPY_HEX_FLAGS) "$(HWTEST_TARG).elf" "$(HWTEST_TARG).hex"
	$(OBJCOPY_CMD)  $(OBJCOPY_EEP_FLAGS) "$(HWTEST_TARG).elf" "$(HWTEST_TARG).eep" || exit 0
	$(OBJDUMP_CMD)  $(OBJDUMP_LSS_FLAGS) "$(HWTEST_TARG).elf" > "$(HWTEST_TARG).lss"
	$(OBJCOPY_CMD)  $(OBJCOPY_SREC_FLAGS) "$(HWTEST_TARG).elf" "$(HWTEST_TARG).srec"
	$(OBJCOPY_CMD)  $(OBJCOPY_SIGN_FLAGS) "$(HWTEST_TARG).elf" "$(HWTEST_TARG).usersignatures" || exit 0
	$(AVR_SIZE_CMD) "$(HWTEST_TARG).elf"
	$(COPY_CMD) "$(HWTEST_TARG).elf" "./$(TEST_TARG).elf"


# Compile objects
$(TARG_BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) $(VERSION_FLAGS) -c -o $@ $<

$(TARG_BUILDDIR)/%.o: %.cpp
	$(CPP) $(CPPFLAGS) $(VERSION_FLAGS) -c -o $@ $<

$(TARG_BUILDDIR)/%.o: %.S
	$(ASM) $(ASMFLAGS) $(VERSION_FLAGS) -c -o $@ $<

$(TEST_BUILDDIR)/%.o: %.c
	$(TEST_CC) $(TEST_CFLAGS) $(TEST_VERSION_FLAGS) -c -o $@ $<

$(TEST_BUILDDIR)/%.o: %.cpp
	$(TEST_CPP) $(TEST_CPPFLAGS) $(TEST_VERSION_FLAGS) -c -o $@ $<

$(TEST_BUILDDIR)/%.o: %.S
	$(TEST_ASM) $(TEST_ASMFLAGS) $(TEST_VERSION_FLAGS) -c -o $@ $<

# Clean
cleandirs:
	$(RM_CMD) $(TARGDIR) $(BUILDDIR)

cleanobjs:
	$(RM_CMD) $(TARG_OBJS) $(DEPS) $(HWTEST_OBJS) $(UNITTEST_OBJS)

cleanmisc:
	$(RM_CMD) $(TARGDIR)/$(TEST_TARG).map $(TARGDIR)/$(TEST_TARG).usersignatures $(TARGDIR)/$(TEST_TARG).srec $(TARGDIR)/$(TEST_TARG).lss $(TARGDIR)/$(TEST_TARG).eep

clean: cleanobjs cleanmisc cleandirs
	$(RM_CMD) $(TARGDIR)/$(TEST_TARG).elf $(TEST_TARG).elf $(TARGDIR)/$(TEST_TARG).bin $(TARGDIR)/$(TEST_TARG).hex lib$(TARG).a $(TARGDIR)/lib$(TARG).a

-include $(DEPS)