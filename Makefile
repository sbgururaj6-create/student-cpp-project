# Makefile for Unix (Linux, macOS) - builds the Student grades program.
#
#   make         compile everything into output/main
#   make run     compile if needed, then start the program inside output/
#   make clean   delete the compiled files
#
# Only files that changed are compiled again. -MMD -MP make the compiler note which
# headers every .cpp file uses, so changing a header recompiles the right files too.

CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra

BUILD    := build
OUT      := output
TARGET   := $(OUT)/main

SOURCES  := main.cpp Student.cpp StudentIO.cpp Processing.cpp
OBJECTS  := $(SOURCES:%.cpp=$(BUILD)/%.o)

all: $(TARGET)

# Link the object files into the program.
$(TARGET): $(OBJECTS) | $(OUT)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@

# Compile one .cpp file into a .o file.
$(BUILD)/%.o: %.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD) $(OUT):
	mkdir -p $@

# The program creates and reads its data files in the folder it runs in.
run: $(TARGET)
	cd $(OUT) && ./main

clean:
	rm -rf $(BUILD) $(TARGET)

-include $(OBJECTS:.o=.d)

.PHONY: all run clean
