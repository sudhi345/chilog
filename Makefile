################################################################################
#teamchimera.in
################################################################################
INCLUDEDIRS 	= include/
C_SRC_DIRS  	= ./src 
C_SRC_DIRS	+= ./
C_SRC	    	= chilog.c
C_SRC	    	+= CAN_logger.c
OUTPUT_NAME 	= CAN_logger

GCC_PREFIX := 

#if CC is not defined i.e. environment is not set using script provided by the BSP, then compile for host system
ifndef CC
CC      := $(GCC_PREFIX)-gcc
endif

ifndef OBJCOPY
OBJCOPY      := $(GCC_PREFIX)-objcopy
endif

MK := mkdir -p
RM := rm -rf

OUTPUT_DIR = build
OBJ_DIR = obj

# Sorting removes duplicates
BUILD_DIRS := $(sort $(OBJ_DIR) $(OUTPUT_DIR) )

# Object files
C_OBJ                   = $(addprefix $(OBJ_DIR)/, $(C_SRC:.c=.o))

# Set source lookup paths
vpath %.c $(C_SRC_DIRS)

CFLAGS 	= -g
CFLAGS += $(patsubst %,-I%, $(INCLUDEDIRS))

.PHONY: all clean clean_all 

clean_all: clean
	$(MAKE) all -j

all: $(OUTPUT_DIR)/$(OUTPUT_NAME).elf

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

# Link C objects to an .elf file
$(OUTPUT_DIR)/$(OUTPUT_NAME).elf : $(BUILD_DIRS) $(C_OBJ)
	@echo
	@echo "LD $(OUTPUT_DIR)/$(OUTPUT_NAME).elf" 
	$(CC) $(LDFLAGS) $(C_OBJ) -o $(OUTPUT_DIR)/$(OUTPUT_NAME).elf

# Create binary .bin file from the .elf file
#$(OUTPUT_DIR)/$(OUTPUT_NAME).bin : $(OUTPUT_DIR)/$(OUTPUT_NAME).elf
#	@echo
#	@echo "Create binary(.bin) file from: " $<
#	$(OBJCOPY) -O binary $(OUTPUT_DIR)/$(OUTPUT_NAME).elf $(OUTPUT_DIR)/$(OUTPUT_NAME).bin

