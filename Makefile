# Variables
CXX = g++
CXXFLAGS = -std=c++11 -Wall -I.
LDFLAGS = -lglfw -lGL -lGLEW -lm
SRC = main.cpp Camera.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = ft_vox

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compiling .cpp files to .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJ)

# Rebuild target
re: clean all

# Phony targets
.PHONY: all clean re
