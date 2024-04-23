CC := gcc
AR := ar
CFLAGS := -Wall -Wextra -I./include
LDFLAGS := -L./lib
LIBS := -lmap -lpthread

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
LIB_DIR := lib

ifeq ($(OS),Windows_NT)
    MKDIR_P := if not exist "$(OBJ_DIR)" mkdir "$(OBJ_DIR)"
    MKDIR_BIN := if not exist "$(BIN_DIR)" mkdir "$(BIN_DIR)"
    RM := del /Q /F
else
    MKDIR_P := @mkdir -p
    MKDIR_BIN := @mkdir -p $(BIN_DIR)
    RM := @rm -rf
endif

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))
EXECUTABLE := $(BIN_DIR)/btree
LIBRARY := $(LIB_DIR)/libbtree.a

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) | $(LIBRARY)
	$(MKDIR_BIN)
	$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

$(LIBRARY): $(OBJECTS)
	$(MKDIR_P) $(@D)
	$(AR) rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(MKDIR_P) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ_DIR)/* $(BIN_DIR)/* $(LIB_DIR)/*
