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

.PHONY: all clean run iso limine

all: $(kernel)

clean:
	@rm -r build

run: $(iso)
	@qemu-system-x86_64 -cdrom $(iso) -M q35

iso: $(iso)

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p build/isofiles/
	@cp $(kernel) build/isofiles/kernel.elf
	@cp src/arch/$(arch)/limine.cfg build/isofiles
	cp limine/limine.sys limine/limine-cd.bin limine/limine-eltorito-efi.bin build/isofiles
	xorriso -as mkisofs -b limine-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-eltorito-efi.bin \
		-efi-boot-part --efi-boot-image --protective-msdos-label \
		build/isofiles -o $(iso)

	limine/limine-install $(iso)
	@rm -rf build/isofiles

$(kernel): $(asm_object_files) $(arch_c_object_files) $(kernel_c_object_files) $(linker_script)
	@ld -n -T $(linker_script) -o $(kernel) $(asm_object_files) $(arch_c_object_files) $(kernel_c_object_files)

build/arch/$(arch)/interrupt_handlers.o: src/arch/$(arch)/interrupt_handlers.c
	@printf "CC: $<\n"
	@mkdir -p $(shell dirname $@)
	@x86_64-unknown-elf-gcc -I src/include/ -c -mgeneral-regs-only -mno-red-zone -ffreestanding $< -o $@ -g -std=gnu11 -fno-stack-protector -fno-pic -fpie -mgeneral-regs-only -mno-red-zone

build/arch/$(arch)/%_asm.o: src/arch/$(arch)/%.asm
	@printf "AS: $<\n"
	@mkdir -p $(shell dirname $@)
	@nasm -felf64 $< -o $@


build/arch/$(arch)/%.o: src/arch/$(arch)/%.c
	@printf "CC: $<\n"
	@mkdir -p $(shell dirname $@)
	@x86_64-unknown-elf-gcc -I src/include/ -c -ffreestanding $< -o $@ -g -std=gnu11 -fno-stack-protector -fno-pic -fpie -mgeneral-regs-only -mno-red-zone

build/kernel/%.o: src/kernel/%.c
	@printf "CC: $<\n"
	@mkdir -p $(shell dirname $@)
	@x86_64-unknown-elf-gcc -I src/include/ -c -ffreestanding $< -o $@ -g -std=gnu11 -fno-stack-protector -fno-pic -fpie -mgeneral-regs-only -mno-red-zone