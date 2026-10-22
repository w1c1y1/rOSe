ASM   = nasm
CC    = gcc
LD    = ld
OBJC  = objcopy
QEMU  = qemu-system-i386

BUILD = build

LDS        = src/kernel.ld
ELF        = $(BUILD)/os.elf
BIN        = $(BUILD)/os.bin
IMG        = $(BUILD)/boot.img
RES        = $(BUILD)/bar
KERNEL_BIN = $(BUILD)/kernel.bin

GCC_INC  := $(shell $(CC) -print-file-name=include)
INC_DIRS := $(sort $(dir $(shell find include utils -name '*.h')))

CFLAGS   = -m32 -std=c99 -ffreestanding -fno-pic -fno-pie \
           -fno-stack-protector -fno-builtin -nostdlib -nostdinc \
           -O2 -Wall -Wextra \
           -isystem $(GCC_INC) \
           $(addprefix -I,$(INC_DIRS))

ASMFLAGS = -f elf32
LDFLAGS  = -m elf_i386 -T $(LDS) -nostdlib
OCFLAGS  = -O binary
QEMU_FLAGS = -m 128 -fda $(IMG) -monitor stdio -no-reboot

C_SRCS   := $(shell find src utils -name '*.c')
ASM_SRCS := $(filter-out src/boot.asm,$(shell find src utils -name '*.asm'))
C_OBJS   := $(patsubst %.c,$(BUILD)/%.o,$(C_SRCS))
ASM_OBJS := $(patsubst %.asm,$(BUILD)/%.o,$(ASM_SRCS))

KERNEL_OBJS := $(ASM_OBJS) $(C_OBJS)
OBJS        := $(BUILD)/src/boot.o $(KERNEL_OBJS)

.PHONY: all run dump clean

all: $(IMG)

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "CC    $<"
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD)/%.o: %.asm
	@mkdir -p $(dir $@)
	@echo "ASM   $<"
	@$(ASM) $(ASMFLAGS) $< -o $@

$(KERNEL_BIN): $(KERNEL_OBJS) $(LDS)
	@mkdir -p $(dir $@)
	@$(LD) $(LDFLAGS) --defsym=endless_loop=0x7C00 $(KERNEL_OBJS) -o $(BUILD)/kernel-size.elf
	@$(OBJC) $(OCFLAGS) $(BUILD)/kernel-size.elf $@

$(BUILD)/src/boot.o: src/boot.asm $(KERNEL_BIN)
	@mkdir -p $(dir $@)
	@SIZE=$$(stat -c%s $(KERNEL_BIN)); \
	 echo "ASM   $< (PAYLOAD_SIZE=$$SIZE)"; \
	 $(ASM) $(ASMFLAGS) -D PAYLOAD_SIZE=$$SIZE $< -o $@

$(ELF): $(OBJS) $(LDS)
	@mkdir -p $(dir $@)
	@echo "LD    $@"
	@$(LD) $(LDFLAGS) $(OBJS) -o $@

$(BIN): $(ELF)
	@mkdir -p $(dir $@)
	@echo "OBJ   $@"
	@$(OBJC) $(OCFLAGS) $< $@

$(IMG): $(BIN)
	@mkdir -p $(dir $@)
	@echo "IMG   $@"
	@dd if=/dev/zero of=$@ bs=1024 count=1440 status=none
	@dd if=$(BIN) of=$@ bs=512 conv=notrunc status=none

-include $(OBJS:.o=.d)

run: $(IMG)
	@$(QEMU) $(QEMU_FLAGS)

dump: $(IMG)
	@( sleep 3; echo "pmemsave 0x7E00 512 $(RES)"; echo "quit" ) | $(QEMU) $(QEMU_FLAGS)

clean:
	@rm -rf $(BUILD)