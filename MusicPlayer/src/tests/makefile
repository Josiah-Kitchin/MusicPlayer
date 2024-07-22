# Compiler
CXX := g++

# Directories
SRC_DIR := ../
OBJ_DIR := ../../objs

# Source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)  $(wildcard *.cpp)
SRCS:= $(filter-out Main.cpp, $(SRCS)) 

# Object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

# Compiler flags
CXXFLAGS := -O2 $(shell pkg-config --cflags gtkmm-3.0 )

# Linker flags
LDFLAGS := -lmpg123 -lao -lsndfile -ltag $(shell pkg-config --libs gtkmm-3.0 )

# Executable name
TARGET := test

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

# Compile each source file into an object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(PLAYER_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

# Ensure the objs directory exists
$(shell mkdir -p $(OBJ_DIR))
