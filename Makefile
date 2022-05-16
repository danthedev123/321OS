# User controllable target architecture. Currently only x86_64 is supported
arch ?= x86_64
# Target kernel executable
kernel := build/kernel-$(arch).bin
# Target ISO (cd-rom) file
iso := build/321OS-$(arch).iso

# Target compiler. User controllable.
cxx ?= clang++

INTERNALLDFLAGS :=  \
	-Tsrc/arch/$(arch)/linker.ld \
	-nostdlib                \
	-zmax-page-size=0x1000   \
	-static

INTERNALCFLAGS :=     \
	-std=gnu++20               \
	-ffreestanding             \
	-fno-exceptions            \
	-fno-stack-protector       \
	-fno-omit-frame-pointer    \
	-fno-pic                   \
	-mabi=sysv                 \
	-mno-80387                 \
	-mno-mmx                   \
	-mno-3dnow                 \
	-mno-sse                   \
	-mno-sse2                  \
	-mno-red-zone              \
	-mcmodel=kernel			   \
	-Werror					   \
	-target x86_64-pc-none-elf \
	
linker_script := src/arch/$(arch)/linker.ld
asm_source_files = $(shell find src/arch/$(arch)/ -type f -name '*.asm')
asm_object_files := $(patsubst src/arch/$(arch)/%.asm, \
	build/arch/$(arch)/%_asm.o, $(asm_source_files))
# Architecture specific C source files
arch_c_source_files = $(shell find src/arch/$(arch)/ -type f -name '*.cpp')
arch_c_object_files := $(patsubst src/arch/$(arch)/%.cpp, \
	build/arch/$(arch)/%.o, $(arch_c_source_files))

# C source files
kernel_c_source_files = $(shell find src/kernel/ -type f -name '*.cpp')
kernel_c_object_files := $(patsubst src/kernel/%.cpp, \
	build/kernel/%.o, $(kernel_c_source_files))

.PHONY: all clean run iso


all: $(kernel)

clean:
	@rm -r build

run: $(iso)
	@qemu-system-x86_64 -cdrom $(iso) -M q35 -m 500M -serial stdio -soundhw pcspk -d int -D kernel.log -no-shutdown -no-reboot -machine smm=off
iso: $(iso)

$(iso): $(kernel) limine
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
	@ld.lld $(INTERNALLDFLAGS) -o $(kernel) $(asm_object_files) $(arch_c_object_files) $(kernel_c_object_files)

build/arch/$(arch)/%_asm.o: src/arch/$(arch)/%.asm
	@printf "AS: $<\n"
	@mkdir -p $(shell dirname $@)
	@nasm -felf64 $< -o $@


build/arch/$(arch)/%.o: src/arch/$(arch)/%.cpp
	@printf "CXX: $<\n"
	@mkdir -p $(shell dirname $@)
	@$(cxx) -I src/include/ -c $(INTERNALCFLAGS) $< -o $@ -g

build/kernel/%.o: src/kernel/%.cpp
	@printf "CXX: $<\n"
	@mkdir -p $(shell dirname $@)
	@$(cxx) -I src/include/ -c $(INTERNALCFLAGS) $< -o $@ -g

limine:
	git clone https://github.com/limine-bootloader/limine.git --branch=v2.0-branch-binary --depth=1
	make -C limine
