# Test files from the test directory
CPP_DIR := . bus bus/device registers registers/model
TEST_OUT := ../build/test
BIN_OUT := ../bin
CPP_FILES := $(foreach D, $(CPP_DIR), $(wildcard $(D)/*cpp))
OBJ_FILES := $(patsubst %.cpp, $(TEST_OUT)/%.o, $(CPP_FILES))

# Source files to link with the test-classes
SRC_OUT_DIR := ../build/src
SRC_OBJ_FILES := $(filter-out ../build/src/main.o, $(shell find $(SRC_OUT_DIR) -name "*.o"))
# SRC_OBJ_FILES := $(filter-out ../build/src/main.o, $(wildcard $(SRC_OUT_DIR)/*.o))

# Googletest flags
GTEST_INCLUDE := -I"/home/robin/Dokumente/GitHub/Intel-8080-Emulator-Base/googletest/googletest/include" -I"/home/robin/Dokumente/GitHub/Intel-8080-Emulator-Base/googletest/googlemock/include"
GTEST_ARCHIVE := /usr/src/googletest/*.a

.PHONY: default
default: $(OBJ_FILES)
	echo $(SRC_OBJ_FILES)
	g++ -lncurses $(GTEST_INCLUDE) $^ $(SRC_OBJ_FILES) $(GTEST_ARCHIVE) -o $(BIN_OUT)/test

$(TEST_OUT)/%.o: %.cpp
	mkdir -p $(@D)
	g++ $(GTEST_INCLUDE) -c $^ $(GTEST_ARCHIVE) -o $@