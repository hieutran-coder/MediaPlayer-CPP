# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -std=c++17 -Wall -lstdc++fs $(shell pkg-config --cflags libvlc)

# Libraries
LIBS = -lSDL2 -lSDL2_mixer -ltag -lavformat -lavcodec -lavutil -lncurses $(shell pkg-config --libs libvlc)

# Source Files
SRC = main.cpp musicplayer.cpp musicmodel.cpp song.cpp musiccontroller.cpp playlistcontroller.cpp playlist.cpp video.cpp videomodel.cpp videocontroller.cpp videoplayer.cpp

# Header files
HEADERS = musicplayer.h musicmodel.h song.h musiccontroller.h playlistcontroller.h playlist.h video.h videomodel.h videocontroller.h videoplayer.h

# Output executable
TARGET = main

# Build target
all: $(TARGET)

# Link objects and libraries to create the executable
$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

# Clean up build files
clean:
	rm -f $(TARGET)  # This line should start with a tab

