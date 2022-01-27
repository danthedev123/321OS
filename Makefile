# User controllable target architecture. Currently only x86_64 is supported
arch ?= x86_64
# Target kernel executable
kernel := build/kernel-$(arch).bin
# Target ISO (cd-rom) file
iso := build/321OS-$(arch).iso

linker_script := src/arch/$(arch)/linker.ld
grub_cfg := src/arch/$(arch)/grub.cfg
asm_source_files = $(shell find src/arch/$(arch)/ -type f -name '*.asm')
asm_object_files := $(patsubst src/arch/$(arch)/%.asm, \
	build/arch/$(arch)/%_asm.o, $(asm_source_files))
# Architecture specific C source files
arch_c_source_files = $(shell find src/arch/$(arch)/ -type f -name '*.c')
arch_c_object_files := $(patsubst src/arch/$(arch)/%.c, \
	build/arch/$(arch)/%.o, $(arch_c_source_files))

# C source files
kernel_c_source_files = $(shell find src/kernel/ -type f -name '*.c')
kernel_c_object_files := $(patsubst src/kernel/%.c, \
	build/kernel/%.o, $(kernel_c_source_files))

.PHONY: all clean run iso

all: $(kernel)

clean:
	@rm -r build

run: $(iso)
	@qemu-system-x86_64 -cdrom $(iso) -m 1G

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/boot/grub
	@cp $(kernel) build/isofiles/boot/kernel.bin
	@cp $(grub_cfg) build/isofiles/boot/grub
	@grub-mkrescue -o $(iso) build/isofiles 2> /dev/null
	@rm -r build/isofiles

$(kernel): $(asm_object_files) $(arch_c_object_files) $(kernel_c_object_files) $(linker_script)
	@ld -n -T $(linker_script) -o $(kernel) $(asm_object_files) $(arch_c_object_files) $(kernel_c_object_files)

build/arch/$(arch)/interrupt_handlers.o: src/arch/$(arch)/interrupt_handlers.c
	@printf "CC: $<\n"
	@mkdir -p $(shell dirname $@)
	@x86_64-elf-gcc -c -mgeneral-regs-only -mno-red-zone -ffreestanding $< -o $@

build/arch/$(arch)/%_asm.o: src/arch/$(arch)/%.asm
	@printf "AS: $<\n"
	@mkdir -p $(shell dirname $@)
	@nasm -felf64 $< -o $@


build/arch/$(arch)/%.o: src/arch/$(arch)/%.c
	@printf "CC: $<\n"
	@mkdir -p $(shell dirname $@)
	@x86_64-elf-gcc -c -ffreestanding $< -o $@

build/kernel/%.o: src/kernel/%.c
	@printf "CC: $<\n"
	@mkdir -p $(shell dirname $@)
	@x86_64-elf-gcc -c -ffreestanding $< -o $@
