TARGET = car
BUILD_DIR = build

######################################
# Sources & Includes
######################################
C_SOURCES = \
main.c \
bluetooth.c \
motor.c \
system_stm32f0xx.c \
syscalls.c \
sysmem.c 

STARTUP = startup_stm32f030x8.s 

# Fixed: Added 'I' to make it a proper Include flag
C_INCLUDES = -IInc

######################################
# Toolchain & Flags
######################################
PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size

MCU = -mcpu=cortex-m0 -mthumb -std=gnu11
CFLAGS = $(MCU) $(C_INCLUDES) --specs=nosys.specs

LDSCRIPT = STM32F030C8Tx_FLASH.ld
# Added --specs=nosys.specs here to prevent linker errors
LDFLAGS = $(MCU) -T$(LDSCRIPT) --specs=nosys.specs -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--gc-sections

######################################
# Build Rules
######################################
final : $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

# Generalized assembly rule
$(BUILD_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) -c $(MCU) -x assembler-with-cpp $< -o $@

# Automatic collection via $^
$(BUILD_DIR)/$(TARGET).elf: $(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.o)) $(BUILD_DIR)/$(STARTUP:.s=.o)
	$(CC) $(LDFLAGS) -nostdlib $^ -o $@ -lc -lgcc
	$(SZ) $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(CP) -O binary $< $@ [cite: 2]

clean:
	rm -rf $(BUILD_DIR)