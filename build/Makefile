# to build a target from the examples directory: $ make demo TARGET=[name of example file]
DEFAULT_TARGET = simple_blink
TARGET = $(DEFAULT_TARGET)

DEBUG = 0

OPTIMIZATION = -O0

LD_SCRIPT = linker.ld

TOOLCHAIN   = arm-none-eabi
COMPILER    = $(TOOLCHAIN)-gcc
LINKER      = $(TOOLCHAIN)-ld
OBJECT_COPY = $(TOOLCHAIN)-objcopy
OBJECT_DUMP = $(TOOLCHAIN)-objdump
OBJECT_SIZE = $(TOOLCHAIN)-size

ASM_FLAGS += -c
ASM_FLAGS += -x 
ASM_FLAGS += assembler-with-cpp
ASM_FLAGS += $(OPTIMIZATION)
ASM_FLAGS += $(CPU)
ASM_FLAGS += -Wall

C_FLAGS += -c
C_FLAGS += $(CPU)
C_FLAGS += -Wall
C_FLAGS += $(OPTIMIZATION)
ifeq ($(DEBUG), 1)
C_FLAGS += -g -gdwarf-2
endif
C_FLAGS += -I$(INCLUDE_DIR)
C_FLAGS += -ffreestanding 
C_FLAGS += -nostdinc 
C_FLAGS += -nostartfiles

L_FLAGS += $(CPU)
L_FLAGS += -Wall
L_FLAGS += -nostartfiles
L_FLAGS += -lgcc
L_FLAGS += -T./$(LD_SCRIPT)

OBJ_COPY_FLAGS += -S
OBJ_COPY_FLAGS += -O 
OBJ_COPY_FLAGS += binary

ROOT_DIR     = ../
SRC_DIR      = $(ROOT_DIR)src/
INCLUDE_DIR  = $(ROOT_DIR)include/
EXAMPLES_DIR = $(ROOT_DIR)examples/
BIN_DIR      = $(ROOT_DIR)bin/

IMAGE = $(ROOT_DIR)kernel.img
ELF = $(BIN_DIR)kernel.elf

C_OBJECT_FILES := $(patsubst $(SRC_DIR)%.c,$(BIN_DIR)%.o,$(wildcard $(SRC_DIR)*.c))
ASM_OBJECT_FILES := $(patsubst $(SRC_DIR)%.s,$(BIN_DIR)%.o,$(wildcard $(SRC_DIR)*.s))

# all makes the default demo application
.PHONY: all
all: demo

# compile the demo target, then build the rest as normal
.PHONY: demo
demo: $(C_OBJECT_FILES) $(BIN_DIR)
	$(COMPILER) $(C_FLAGS) $(EXAMPLES_DIR)$(TARGET).c -o $(BIN_DIR)$(TARGET).o
	make $(IMAGE)

# compile the user provided application c source files
$(BIN_DIR)%.o: $(SRC_DIR)%.c
	$(COMPILER) $(C_FLAGS) $< -o $@

# compile the user provided application assembly source files
$(BIN_DIR)%.o: $(SRC_DIR)%.s
	$(COMPILER) $(ASM_FLAGS) $< -o $@

# link the object files together into the ELF file
$(ELF): $(ASM_OBJECT_FILES) $(C_OBJECT_FILES) $(BIN_DIR)$(TARGET).o
	$(COMPILER) $^ $(L_FLAGS) -o $@

# make the kernel.img image, as well as a listing and print info about the size
$(IMAGE): $(ELF)
	$(OBJECT_COPY) $(OBJ_COPY_FLAGS) $< $@
	$(OBJECT_DUMP) -D $(ELF) > $(BIN_DIR)asm_dump.list
	$(OBJECT_SIZE) $<

# create the bin dir if it does not exist
$(BIN_DIR):
	mkdir $@

clean:
	rm -f $(IMAGE)
	rm -f $(BIN_DIR)*.o
	rm -f $(BIN_DIR)*.elf
	rm -f $(BIN_DIR)*.list
