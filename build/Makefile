
ARMGNU ?= arm-none-eabi

ROOT_DIR  = ../

API_DIR = $(ROOT_DIR)include/
SRC_DIR = $(ROOT_DIR)src/
BIN_DIR = $(ROOT_DIR)bin/

CFLAGS = -Wall -O2 -I$(API_DIR) -ffreestanding -nostdinc -nostartfiles

TARGET = $(ROOT_DIR)kernel.img

LINKER = linker.ld

ELF = $(BIN_DIR)kernel.elf

C_OBJS := $(patsubst $(SRC_DIR)%.c,$(BIN_DIR)%.o,$(wildcard $(SRC_DIR)*.c))

ASM_START = $(SRC_DIR)start.s
ASM_START_OBJ = $(BIN_DIR)start.o

all: $(TARGET)

$(ASM_START_OBJ): $(ASM_START) $(BIN_DIR)
	$(ARMGNU)-gcc $(CFLAGS) -c $(ASM_START) -o $(ASM_START_OBJ)

$(BIN_DIR)%.o: $(SRC_DIR)%.c
	$(ARMGNU)-gcc $(CFLAGS) -c $< -o $@

$(TARGET): $(ASM_START_OBJ) $(C_OBJS)
	$(ARMGNU)-ld -nostartfiles $(ASM_START_OBJ) $(C_OBJS) -T $(LINKER) -o $(ELF)
	$(ARMGNU)-objcopy -O binary $(ELF) $(TARGET)

$(BIN_DIR):
	mkdir $@

clean:
	rm -f $(TARGET)
	rm -f $(BIN_DIR)*.o
	rm -f $(BIN_DIR)*.elf
