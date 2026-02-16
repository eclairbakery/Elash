CC ?= cc
BUILD ?= release

ifeq ($(OS),Windows_NT)
	PLATFORM := windows
else
	PLATFORM := posix
endif

SRC_DIR     := src
TEST_DIR    := tests
INCLUDE_DIR := include

OBJ_ROOT_DIR := build/$(BUILD)/obj
DEP_ROOT_DIR := build/$(BUILD)/dep

OUT_DIR     := out/$(BUILD)
LIB_DIR     := $(OUT_DIR)/lib
BIN_DIR     := $(OUT_DIR)/bin

LIB_NAME    := elash
LIB_STATIC  := $(LIB_DIR)/lib$(LIB_NAME).a
LIB_SHARED  := $(LIB_DIR)/lib$(LIB_NAME).so

EXE_EXT :=
ifeq ($(PLATFORM),windows)
	EXE_EXT := .exe
else ifeq ($(PLATFORM),posix)
	EXE_EXT := .elf
endif
TARGET := $(BIN_DIR)/elc$(EXE_EXT)

CSTD     := -std=c11
WARNINGS := -Wall -Wextra
PIC_CFLAGS := -fPIC

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

ifeq ($(PLATFORM),windows)
	CMD_MKDIR_P = powershell -NoProfile -Command "New-Item -ItemType Directory -Force -Path '$(subst /,\,$(1))'"
	CMD_RM_RF = powershell -NoProfile -Command "Remove-Item -Recurse -Force -Path '$(subst /,\,$(1))'"
else
	CMD_MKDIR_P = mkdir -p "$(1)"
	CMD_RM_RF = rm -rf "$(1)"
endif

ifeq ($(PLATFORM),posix)
	ALL_C_SRCS := $(shell find $(SRC_DIR) -name "*.c")
else
	ALL_C_SRCS := $(shell powershell -NoProfile -Command "Get-ChildItem -Path '$(SRC_DIR)' -Recurse -Include *.c | ForEach-Object { $_.FullName -replace '\\\\','/' }")
endif

MAIN_C_SRC := $(SRC_DIR)/main.c
LIB_C_SRCS := $(filter $(SRC_DIR)/%, $(filter-out $(MAIN_C_SRC), $(ALL_C_SRCS)))

MAIN_OBJ := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(MAIN_C_SRC))
LIB_OBJ_STATIC := $(patsubst %.c,$(OBJ_ROOT_DIR)/%.o,$(LIB_C_SRCS))
LIB_OBJ_SHARED := $(patsubst %.c,$(OBJ_ROOT_DIR)/shared/%.o,$(LIB_C_SRCS))

DEPS := $(patsubst %.c,$(DEP_ROOT_DIR)/%.d,$(ALL_C_SRCS)) \
        $(patsubst %.c,$(DEP_ROOT_DIR)/shared/%.d,$(LIB_C_SRCS))

JFLAG := $(filter -j%,$(MAKEFLAGS))
ifeq ($(JFLAG),)
	JFLAG := -j1
endif

.PHONY: all dirs clean run tests sharedlib

all: dirs $(TARGET) $(LIB_STATIC) $(LIB_SHARED) $(TEST_BIN)

dirs:
	@$(call CMD_MKDIR_P,$(LIB_DIR))
	@$(call CMD_MKDIR_P,$(BIN_DIR))
	@$(call CMD_MKDIR_P,$(OBJ_ROOT_DIR))
	@$(call CMD_MKDIR_P,$(DEP_ROOT_DIR))
	@$(call CMD_MKDIR_P,$(OBJ_ROOT_DIR)/shared)
	@$(call CMD_MKDIR_P,$(DEP_ROOT_DIR)/shared)

$(LIB_STATIC): $(LIB_OBJ_STATIC)
	ar rcs $@ $^

$(LIB_SHARED): $(LIB_OBJ_SHARED)
	$(CC) -shared $^ $(LDFLAGS) -o $@

$(TARGET): $(LIB_STATIC) $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) $(LIB_STATIC) $(LDFLAGS) -o $@

$(OBJ_ROOT_DIR)/%.o: %.c
	@$(call CMD_MKDIR_P,$(dir $@))
	@$(call CMD_MKDIR_P,$(DEP_ROOT_DIR)/$(dir $<))
	$(CC) $(CFLAGS) -MMD -MP -MF $(DEP_ROOT_DIR)/$*.d -c $< -o $@

$(OBJ_ROOT_DIR)/shared/%.o: %.c
	@$(call CMD_MKDIR_P,$(dir $@))
	@$(call CMD_MKDIR_P,$(DEP_ROOT_DIR)/shared/$(dir $<))
	$(CC) $(CFLAGS) $(PIC_CFLAGS) -MMD -MP -MF $(DEP_ROOT_DIR)/shared/$*.d -c $< -o $@

run: all
	$(TARGET)

sharedlib: dirs $(LIB_SHARED)

-include $(DEPS)

clean:
	@$(call CMD_RM_RF,build)
	@$(call CMD_RM_RF,out)
