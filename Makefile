CC      := cc
LD      := cc

INCLUDE := include
SRC     := src
BUILD   := build
OBJ     := $(BUILD)/obj

CCFLAGS := -Wextra -Wall -c -I$(INCLUDE)
LDFLAGS := # \(0-0)/

C_SRC   := $(wildcard $(SRC)/*.c)
C_OBJ := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(C_SRC))

BIN := $(BUILD)/panik

$(OBJ)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) $^ -o $@

$(BIN): $(C_OBJ)
	mkdir -p $(dir $@)
	$(LD) $(LDFLAGS) -o $@ $<

# The frontend

build: $(BIN)
clean:
	rm -rf $(BUILD)
