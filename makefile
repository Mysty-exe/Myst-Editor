CXX = g++
CXXFLAGS = -std=c++20 -Iinclude -Ilib -Wall -Wextra -O2

TARGET = build/myst

SRC_CPP = src/main.cpp src/app.cpp src/editor.cpp src/status.cpp src/history.cpp src/file.cpp src/menu.cpp src/buttons.cpp src/settings.cpp src/scanner.cpp
SRC_C   = lib/ini.c

OBJ_DIR = build/obj

OBJ_CPP = $(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRC_CPP))
OBJ_C   = $(patsubst lib/%.c,  $(OBJ_DIR)/%.o, $(SRC_C))

OBJ_FILES = $(OBJ_CPP) $(OBJ_C)

all: build_dirs $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(TARGET) -lncurses

# Compile cpp → build/obj/*.o
$(OBJ_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile c → build/obj/*.o
$(OBJ_DIR)/%.o: lib/%.c
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build_dirs:
	mkdir -p build
	mkdir -p $(OBJ_DIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf build