ROOT_DIR := .
TARGET_NAME ?= game
INC_DIR := $(ROOT_DIR)/include
SRC_DIR := $(ROOT_DIR)/src
OBJ_DIR := $(ROOT_DIR)/obj
INC_DIRS := $(shell find $(INC_DIR) -type d)
SRC_DIRS := $(shell find $(SRC_DIR) -type d)
CC := g++
LINKS := -lm -lncursesw -lSDL2
CFLAGS := $(addprefix -I, $(INC_DIRS)) -Wall -Wextra -g -Werror
DEPS := $(wildcard $(addsuffix /*.h, $(INC_DIRS)))
SRCS := $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
TARGET := $(TARGET_NAME)

$(shell mkdir -p $(OBJ_DIR))
$(shell mkdir -p $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC_DIRS)))

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LINKS)

.PHONY: clean data cleandata

clean:
	rm -fr $(OBJ_DIR)
	rm -f $(TARGET)
	rm -f $(ROOT_DIR)/map.png
	rm -f $(ROOT_DIR)/map.ppm

cleandata:
	rm -fr db/$(OBJ_DIR)
	rm -f  $(ROOT_DIR)/data

data: 
	make ROOT_DIR=db TARGET_NAME=data

all: $(TARGET)