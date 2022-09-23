# Directories where CPP-Files are located
CPP_DIR := . registers registers/model bus bus/device bus/model core instruction io io/device io/model

# Default locations to the the object files and binaries
OUTPUT_DIR := ../build
BIN_DIR := ../bin
DEFAULT:= $(BIN_DIR)/main

# Flags for the building process
GPP := g++
OPT := -O0
DEPS := -lncurses
CPP_FLAGS := -MD $(OPT) $(foreach D, $(CPP_DIR), -I$(D))

# Filter variables to select certain files
BUILD_DIR_OUT := $(foreach D, $(CPP_DIR), $(OUTPUT_DIR)/$(D))
CPP_FILES := $(foreach D,$(CPP_DIR), $(wildcard $(D)/*cpp))
HEAD_FILES := $(patsubst %.cpp, $(OUTPUT_DIR)/%.d, $(CPP_FILES))
OBJ_FILES := $(patsubst %.cpp, $(OUTPUT_DIR)/%.o, $(CPP_FILES))

# First step:
#	Create Binary Directory
#	Link & Build the binary
$(DEFAULT): $(OBJ_FILES)
	mkdir -p $(BIN_DIR)
	$(GPP) $^ $(DEPS) -o $@

# Second step:
#	Create the directory for the object file
#	Compile the file
$(OUTPUT_DIR)/%.o:%.cpp
	mkdir -p $(@D)
	$(GPP) $(CPP_FLAGS) -c $< -o $@

# Include Headers
-include $(HEAD_FILES)

.PHONY: clean
clean:
	rm $(OUTPUT_DIR)/* -r
	rm $(BIN_DIR)/* -r