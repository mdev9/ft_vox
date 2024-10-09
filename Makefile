CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lglfw -lGL -lGLEW -lm -I.

TARGET = ft_vox
SRC = main.cpp Camera.cpp stb_image.h

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
