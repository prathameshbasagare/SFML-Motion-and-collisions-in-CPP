# Command: make run

CXX := g++
OUTPUT := sfmlgame
CXX_FLAGS := -O3 -std=c++20 -Wno-unused-result -Wno-deprecated-declarations
INCLUDES := -I ./src -I ./src/imgui -I ./src/imgui-sfml/
LDFLAGS  := -O3 -lsfml-window -lsfml-graphics -lsfml-system -lsfml-audio -lGL

SRC_FILES := $(wildcard ./src/*.cpp ./src/imgui/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

DEP_FILES := $(SRC_FILES:.cpp=.d)
-include $(DEP_FILES)

all: $(OUTPUT)

$(OUTPUT): $(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

.ccp.o:
	$(CXX) -MMD -MP -c $(CXX_FLAGS) $(INCLUDES) -o $< -o $@

clean:
	rm -f $(OBJ_FILES) $(DEP_FILES) ./bin/$(OUTPUT)

run: $(OUTPUT)
	cd bin && ./$(OUTPUT) && cd ..