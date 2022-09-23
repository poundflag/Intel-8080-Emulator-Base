GPP := g++
OPT := -O0
DEPS := -lncurses
CPP_FLAGS := $(OPT) $(DEPS)

OUTPUT_DIR := ../build
BIN_DIR := ../bin

DEFAULT:= $(BIN_DIR)/main

# Directory where CPP-Files are located
CPP_DIR := . registers registers/model bus bus/device bus/model core instruction io io/device io/model
BUILD_DIR_OUT := $(foreach D, $(CPP_DIR), $(OUTPUT_DIR)/$(D))
CPP_FILES := $(foreach D,$(CPP_DIR), $(wildcard $(D)/*cpp))
OBJ_FILES := $(patsubst %.cpp, $(OUTPUT_DIR)/%.o, $(CPP_FILES))

$(DEFAULT): $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(GPP) $(OPT) -o $@ $^ $(CPP_FLAGS)

.PHONY: clean

$(OUTPUT_DIR)/%.o:%.cpp
	mkdir -p $(@D)
	$(GPP) -c -o $@ $^

clean:
	rm $(OUTPUT_DIR)/* -r
	rm $(BIN_DIR)/* -r