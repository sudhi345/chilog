#This Makefile is written by Sudheendra H, Team Chimera. (c)2020, Team Chimera. All rights reserved
INCLUDEDIRS = ./include
C_SRC_DIRS  = ./
C_SRC	    = ./src/chilog.c
C_SRC	    += test.c

GCC_PREFIX := 

CC      := $(GCC_PREFIX)gcc
GDB     := $(GCC_PREFIX)gdb
SIZE    := $(GCC_PREFIX)size

MK := mkdir -p
RM := rm -rf

OUTPUT_NAME = test
OUTPUT_DIR = build
OBJ_DIR = obj

# Sorting removes duplicates
BUILD_DIRS := $(sort $(OBJ_DIR) $(OUTPUT_DIR) )

# Make a list of source paths
C_SRC_DIRS  += ./src

ASSEMBLY_SRC_DIRS       = ./

# Object files
C_OBJ                   = $(addprefix $(OBJ_DIR)/, $(C_SRC:.c=.o))
ASSEMBLY_OBJ		= $(addprefix $(OBJ_DIR)/, $(ASSEMBLY_SRC:.S=.o))

# Set source lookup paths
vpath %.c $(C_SRC_DIRS)
vpath %.S $(ASSEMBLY_SRC_DIRS)

# Include automatically previously generated dependencies
-include $(addprefix $(OBJ_DIR)/, $(C_OBJ:.o=.d))

CFLAGS = -g

.PHONY: all clean clean_all 

clean_all: clean
	$(MAKE) all -j

all: $(OUTPUT_DIR)/$(OUTPUT_NAME).bin

clean:
	$(RM) $(OUTPUT_DIR)/*
	$(RM) $(OBJ_DIR)/*

$(BUILD_DIRS) : 
	@echo 
	@echo "Creating directories"
	- $(MK) $@

# Create objects from C source files
$(OBJ_DIR)/%.o : %.c
	@echo "CC " $<
	$(CC) $(CFLAGS) -M $< -MF "$(@:.o=.d)" -MT $@       
	$(CC) $(CFLAGS) -c -o $@ $<

## Assemble .S files
#$(OBJ_DIR)/%.o : %.S
#	@echo "CC " $<
#	$(Q)$(CC) $(CFLAGS) -c -o $@ $<

## Link C and assembler objects to an .elf file
$(OUTPUT_DIR)/$(OUTPUT_NAME).elf : $(BUILD_DIRS) $(C_OBJ)
	@echo
	@echo "LD $(OUTPUT_DIR)/$(OUTPUT_NAME).elf" 
	$(CC) $(LDFLAGS) $(C_OBJ) $(ASSEMBLY_OBJ) -o $(OUTPUT_DIR)/$(OUTPUT_NAME).elf

## Create binary .bin file from the .elf file
$(OUTPUT_DIR)/$(OUTPUT_NAME).bin : $(OUTPUT_DIR)/$(OUTPUT_NAME).elf
	@echo
	@echo "Create binary(.bin) file from: " $<
	$(OBJCOPY) -O binary $(OUTPUT_DIR)/$(OUTPUT_NAME).elf $(OUTPUT_DIR)/$(OUTPUT_NAME).bin

## Create binary .hex file from the .elf file
#$(OUTPUT_DIR)/$(OUTPUT_NAME).hex : $(OUTPUT_DIR)/$(OUTPUT_NAME).elf
#	@echo "Create hex file from: " $<
#	$(Q)$(OBJCOPY) -O ihex $(OUTPUT_DIR)/$(OUTPUT_NAME).elf $(OUTPUT_DIR)/$(OUTPUT_NAME).hex
#	@if [ "$(SD_USED_LC)" != "blank" ]; then\
#		echo "Create combined $(SD_USED) and $(APPLN).hex file $(OUTPUT_FLASHING).hex "; \
#		srec_cat $(PWD)/$(SOFTDEVICE) -Intel $(PWD)/$(OUTPUT_DIR)/$(OUTPUT_NAME).hex -Intel -O $(PWD)/$(OUTPUT_DIR)/$(OUTPUT_FLASHING).hex -Intel --l$
#	fi
