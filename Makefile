# source: https://github.com/bztsrc/raspi3-tutorial

ARMGNU ?= arm-none-eabi

SRC_DIR = src/
BUILD_DIR = bin/

CFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostartfiles

TARGET = kernel.img

LINKER = linker.ld

ELF = $(BUILD_DIR)kernel.elf

C_OBJS := $(patsubst $(SRC_DIR)%.c,$(BUILD_DIR)%.o,$(wildcard $(SRC_DIR)*.c))

ASM_START = $(SRC_DIR)start.s
ASM_START_OBJ = $(BUILD_DIR)start.o

all: $(TARGET)

$(ASM_START_OBJ): $(ASM_START) $(BUILD_DIR)
	$(ARMGNU)-gcc $(CFLAGS) -c $(ASM_START) -o $(ASM_START_OBJ)

$(BUILD_DIR)%.o: $(SRC_DIR)%.c
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(TARGET): $(ASM_START_OBJ) $(C_OBJS)
	$(ARMGNU)-ld -nostartfiles $(ASM_START_OBJ) $(C_OBJS) -T $(LINKER) -o $(ELF)
	$(ARMGNU)-objcopy -O binary $(ELF) $(TARGET)

$(BUILD_DIR):
	mkdir $@

clean:
	rm -f $(TARGET)
	rm -f $(BUILD_DIR)*.o
	rm -f $(BUILD_DIR)*.elf
