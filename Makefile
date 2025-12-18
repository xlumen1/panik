CC      := cc
LD      := cc

INCLUDE := include
SRC     := src
VENDOR  := vendor
BUILD   := build
OBJ     := $(BUILD)/obj

CCFLAGS := -Wall -Wextra -std=c11 -I$(INCLUDE) -I$(VENDOR) $(shell pkg-config --cflags libcurl)
LDFLAGS := $(shell pkg-config --libs libcurl)

# Project sources
ALL_SRC   := $(shell find $(SRC) -name "*.c") $(VENDOR)/tomlc17/toml.c
ALL_OBJ := $(patsubst %,$(OBJ)/%,$(ALL_SRC:.c=.o))

BIN := $(BUILD)/panik

# Project compilation
$(OBJ)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CCFLAGS)

# Linking
$(BIN): $(ALL_OBJ)
	mkdir -p $(dir $@)
	$(LD) -o $@ $^ $(LDFLAGS)

# Frontend
build: $(BIN)

clean:
	rm -rf $(BUILD)

.PHONY: build clean
