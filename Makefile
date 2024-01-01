RPI_VERSION ?= 3

# where the generated files will be located
BOOTMNT ?= /media/ubuntu/bootfs

ARMGNU ?= aarch64-linux-gnu

# C options 
COPS = -DRPI_VERSION=$(RPI_VERSION) -Wall -nostdlib -nostartfiles -ffreestanding \
		-Iinclude -mgeneral-regs-only

# ASM options
ASMOPS = -Iinclude

# Kernel directories
BUILD_DIR = build
SRC_DIR = src

#ARM stub directories
ARM_BUILD_DIR = armstub/build
ARM_SRC_DIR = armstub/src

# for all we create the kernel image
all : kernel8.img armstub

#for clean we want to clean build directory
clean :
	rm -rf $(BUILD_DIR) *.img
	rm -rf $(ARM_BUILD_DIR)  *.bin
ifneq ("$(wildcard $(BOOTMNT)/armstub-new.bin)","")
	@echo ""
	@echo "armstub-new.bin Exits"
	@echo "Removing armstub-new.bin.."
	rm $(BOOTMNT)/armstub-new.bin
	@echo "Done."
	@echo ""
endif
	
ifneq ("$(wildcard $(BOOTMNT)/kernel8-rpi3.img)","")
	@echo ""
	@echo "kernel8-rpi3.img Exits"
	@echo "Removing kernel8-rpi3.img.."
	rm $(BOOTMNT)/kernel8-rpi3.img
	@echo "Done."
	@echo ""
endif
	

# Pattern rule for building object file fron C files
# Create build directory if doesn't exist
# -MMD : to generate dependency file with the extension ".d" that Makefile understand, it contains relation between source file and included header file
# -c $< : First dependency which is the %.c files
# -o $@ : Target which is the %_c.o files
# Same for ASM files

$(BUILD_DIR)/%_c.o : $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

$(BUILD_DIR)/%_s.o : $(SRC_DIR)/%.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

# Variable that contains C files we will be compiling
C_FILES = $(wildcard $(SRC_DIR)/*.c)

# Variable that contains ASM files we will be compiling
ASM_FILES = $(wildcard $(SRC_DIR)/*.S)

# Variable that contains Object files
OBJ_FILES = $(C_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%_c.o)
OBJ_FILES += $(ASM_FILES:$(SRC_DIR)/%.S=$(BUILD_DIR)/%_s.o)

# Variable that contains Dependencies files
DEP_FILES = $(OBJ_FILES:%.o=%.d)

-include $(DEP_FILES)

kernel8.img: $(SRC_DIR)/linker.ld $(OBJ_FILES)
	@echo "Building for RPI $(value RPI_VERSION)"
	@echo "Deploy to $(value BOOTMNT)"
	@echo ""
	$(ARMGNU)-ld -T $(SRC_DIR)/linker.ld -o $(BUILD_DIR)/kernel8.elf $(OBJ_FILES)
	$(ARMGNU)-objcopy $(BUILD_DIR)/kernel8.elf -O binary kernel8.img
ifeq ($(RPI_VERSION), 3)
	cp kernel8.img $(BOOTMNT)/kernel8-rpi3.img
else
	cp kernel8.img $(BOOTMNT)/
endif
	cp config.txt $(BOOTMNT)/
	sync

# Compile the armstub.S file and obj file will be in armstub build repos. 
$(ARM_BUILD_DIR)/armstub_s.o: $(ARM_SRC_DIR)/armstub.S
	mkdir -p $(@D)
	$(ARMGNU)-gcc $(COPS) -MMD -c $< -o $@

# Link the armstub to the top of .text section
armstub: $(ARM_BUILD_DIR)/armstub_s.o 
	@echo ""
	$(ARMGNU)-ld --section-start=.text=0 -o armstub/build/armstub.elf armstub/build/armstub_s.o
	$(ARMGNU)-objcopy $(ARM_BUILD_DIR)/armstub.elf -O binary armstub-new.bin
	cp armstub-new.bin $(BOOTMNT)/
	sync
