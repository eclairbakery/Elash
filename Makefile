CC ?= cc
BUILD ?= release

ifeq ($(OS),Windows_NT)
	PLATFORM := windows
else
	PLATFORM := linux
endif

# directories
SRC_DIR     := src
TEST_DIR    := tests
INCLUDE_DIR := include

# Object and Dependency file root directories
OBJ_ROOT_DIR := build/$(BUILD)/obj
DEP_ROOT_DIR := build/$(BUILD)/dep

# Output directories for binaries and libraries
OUT_DIR     := out/$(BUILD)
LIB_DIR     := $(OUT_DIR)/lib
BIN_DIR     := $(OUT_DIR)

# outputs
LIB_NAME    := elash
LIB_STATIC  := $(LIB_DIR)/lib$(LIB_NAME).a
LIB_SHARED  := $(LIB_DIR)/lib$(LIB_NAME).so

TARGET      := $(BIN_DIR)/elc
TEST_BIN    := $(BIN_DIR)/el-tests

EXE_EXT :=
ifeq ($(PLATFORM),windows)
	EXE_EXT := .exe
endif

TARGET   := $(TARGET)$(EXE_EXT)
TEST_BIN := $(TEST_BIN)$(EXE_EXT)

# flags
CSTD     := -std=c11
WARNINGS := -Wall -Wextra
PIC_CFLAGS := -fPIC # Position-Independent Code for shared libraries

COMMON_CFLAGS := $(CSTD) $(WARNINGS) -I$(INCLUDE_DIR)

ifeq ($(BUILD),debug)
	CFLAGS := $(COMMON_CFLAGS) -O0 -g -DEL_DEBUG -fsanitize=address,undefined
	LDFLAGS := -fsanitize=address,undefined
else ifeq ($(BUILD),release)
	CFLAGS := $(COMMON_CFLAGS) -O3 -DNDEBUG
	LDFLAGS :=
else
	$(error Unknown BUILD=$(BUILD))
endif

# sources - generalized for both platforms
ifeq ($(PLATFORM),linux)
	ALL_C_SRCS := $(shell find $(SRC_DIR) $(TEST_DIR) -name "*.c")
else
	ALL_C_SRCS := $(shell powershell -NoProfile -Command "Get-ChildItem -Path '$(SRC_DIR)','$(TEST_DIR)' -Recurse -Include *.c | ForEach-Object { $_.FullName -replace '\\\\','/' }")
endif

MAIN_C_SRC := $(SRC_DIR)/main.c
LIB_C_SRCS := $(filter $(SRC_DIR)/%, $(filter-out $(MAIN_C_SRC), $(ALL_C_SRCS))) # All source files in SRC_DIR except main.c
TEST_C_SRCS := $(filter $(TEST_DIR)/%, $(ALL_C_SRCS))

# Object and Dependency file paths
MAIN_OBJ := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(MAIN_C_SRC))
LIB_OBJ_STATIC := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(LIB_C_SRCS))
LIB_OBJ_SHARED := $(patsubst %.c,$(OBJ_ROOT_DIR)/shared/%.o,$(LIB_C_SRCS)) # Separate objects for shared lib
TEST_OBJ := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(TEST_C_SRCS))

# Aggregate all dependency files for inclusion
DEPS := $(patsubst %.c,$(DEP_ROOT_DIR)/%.d,$(ALL_C_SRCS)) \
        $(patsubst %.c,$(DEP_ROOT_DIR)/shared/%.d,$(LIB_C_SRCS)) # Dependencies for shared objects too

# jobs
JFLAG := $(filter -j%,$(MAKEFLAGS))
ifeq ($(JFLAG),)
	JFLAG := -j1
endif

.PHONY: all dirs clean run tests sharedlib

# targets
all: dirs $(TARGET) $(LIB_STATIC) $(LIB_SHARED) $(TEST_BIN)

dirs:
	@mkdir -p $(LIB_DIR) $(BIN_DIR) $(OBJ_ROOT_DIR) $(DEP_ROOT_DIR) $(OBJ_ROOT_DIR)/shared $(DEP_ROOT_DIR)/shared

# static library
$(LIB_STATIC): $(LIB_OBJ_STATIC)
	ar rcs $@ $^

# shared library
$(LIB_SHARED): $(LIB_OBJ_SHARED)
	$(CC) -shared $^ $(LDFLAGS) -o $@

# production binary
$(TARGET): $(LIB_STATIC) $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) $(LIB_STATIC) $(LDFLAGS) -o $@

# test binary
$(TEST_BIN): $(LIB_STATIC) $(TEST_OBJ)
	$(CC) $(TEST_OBJ) $(LIB_STATIC) $(LDFLAGS) -o $@

# Generic rule for building non-PIC object files and their dependencies
$(OBJ_ROOT_DIR)/%.o: %.c
	@mkdir -p $(dir $@) $(DEP_ROOT_DIR)/$(dir $<)
	$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_ROOT_DIR)/$*.d -c $< -o $@

# Rule for building PIC object files (for shared library) and their dependencies
$(OBJ_ROOT_DIR)/shared/%.o: %.c
	@mkdir -p $(dir $@) $(DEP_ROOT_DIR)/shared/$(dir $<)
	$(CC) $(CFLAGS) $(PIC_CFLAGS) -MMD -MP -MF $(DEP_ROOT_DIR)/shared/$*.d -c $< -o $@

# test target
tests: dirs $(TEST_BIN)
	$(TEST_BIN)

run: all
	$(TARGET)

sharedlib: dirs $(LIB_SHARED)

# deps
-include $(DEPS)

# clean
ifeq ($(PLATFORM),linux)
clean:
	rm -rf build out
else
clean:
	if exist build rmdir /S /Q build
	if exist out rmdir /S /Q out
endif

