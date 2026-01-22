CC      := cc
LD      := cc

INCLUDE := include
SRC     := src
VENDOR  := vendor
BUILD   := build
OBJ     := $(BUILD)/obj
BUILD_TYPE ?= debug

COMMON_FLAGS := -Wall -Wextra -Wpedantic -std=gnu11 \
                -I$(INCLUDE) -I$(VENDOR) \
                $(shell pkg-config --cflags libcurl) \
                $(shell pkg-config --cflags sqlite3)

DEBUG_FLAGS := -g -O0 \
               -fsanitize=address,undefined \
               -fno-omit-frame-pointer

RELEASE_FLAGS := -O2 -DNDEBUG

ifeq ($(BUILD_TYPE),debug)
    CCFLAGS := $(COMMON_FLAGS) $(DEBUG_FLAGS)
    LDFLAGS := $(DEBUG_FLAGS) \
               $(shell pkg-config --libs libcurl) \
               $(shell pkg-config --libs sqlite3)
else ifeq ($(BUILD_TYPE),release)
    CCFLAGS := $(COMMON_FLAGS) $(RELEASE_FLAGS)
    LDFLAGS := $(shell pkg-config --libs libcurl) \
               $(shell pkg-config --libs sqlite3)
else
    $(error Unknown BUILD_TYPE '$(BUILD_TYPE)')
endif

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
