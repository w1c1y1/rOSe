ASM   = nasm
CC    = gcc
LD    = ld
OBJC  = objcopy
QEMU  = qemu-system-i386

LDS  = kernel.ld
ELF  = os.elf
BIN  = os.bin
IMG  = boot.img
RES  = bar

GCC_INC  := $(shell $(CC) -print-file-name=include)
INC_DIRS := $(sort $(dir $(shell find utils -name '*.h')))

CFLAGS   = -m32 -std=c99 -ffreestanding -fno-pic -fno-pie \
           -fno-stack-protector -fno-builtin -nostdlib -nostdinc \
           -O2 -Wall -Wextra \
           -isystem $(GCC_INC) \
           $(addprefix -I,$(INC_DIRS))

ASMFLAGS = -f elf32
LDFLAGS  = -m elf_i386 -T $(LDS) -nostdlib
OCFLAGS  = -O binary
QEMU_FLAGS = -m 128 -fda $(IMG) -monitor stdio -no-reboot

C_SRCS   := $(shell find . -name '*.c')
ASM_SRCS := $(filter-out ./boot.asm,$(shell find . -name '*.asm'))
C_OBJS   := $(C_SRCS:.c=.o)
ASM_OBJS := $(ASM_SRCS:.asm=.o)

OBJS     := boot.o $(ASM_OBJS) $(C_OBJS)

.PHONY: all run dump clean

all: $(IMG)

%.o: %.c
	@echo "CC    $<"
	@$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

%.o: %.asm
	@echo "ASM   $<"
	@$(ASM) $(ASMFLAGS) $< -o $@

$(ELF): $(OBJS) $(LDS)
	@echo "LD    $@"
	@$(LD) $(LDFLAGS) $(OBJS) -o $@

$(BIN): $(ELF)
	@echo "OBJ   $@"
	@$(OBJC) $(OCFLAGS) $< $@

$(IMG): $(BIN)
	@echo "IMG   $@"
	@dd if=/dev/zero of=$@ bs=1024 count=1440 status=none
	@dd if=$(BIN) of=$@ bs=512 conv=notrunc status=none

-include $(OBJS:.o=.d)

run: $(IMG)
	@$(QEMU) $(QEMU_FLAGS)

dump: $(IMG)
	@( sleep 3; echo "pmemsave 0x7E00 512 $(RES)"; echo "quit" ) | $(QEMU) $(QEMU_FLAGS)

clean:
	@find . -name '*.o' -delete
	@find . -name '*.d' -delete
	@rm -f $(ELF) $(BIN) $(IMG) $(RES)