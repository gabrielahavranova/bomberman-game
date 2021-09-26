CXX=g++
CXXFLAGS=-Wall -pedantic
BUILD_DIR = build

LIBRARIES=-lSDL2 -lSDL2_image -lSDL2_ttf


compile: build/main.o build/Application.o build/Game.o build/Map.o build/TextureManager.o build/Explosion.o build/Player.o build/FileManager.o build/HumanPlayerArrows.o build/HumanPlayerWASD.o build/ComputerPlayer.o
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o havragab $(LIBRARIES)


run: 
	./havragab 

clean: 
	rm -rf $(BUILD_DIR)
	rm -f havragab

doc:
	


build/Application.o: src/Application.cpp src/./include/Application.h \
 src/./include/Game.h src/./include/Map.h src/./include/TextureManager.h \
 src/./include/Explosion.h src/./include/Player.h \
 src/./include/Configuration.h src/./include/FileManager.h \
 src/./include/HumanPlayerArrows.h src/./include/HumanPlayerWASD.h \
 src/./include/ComputerPlayer.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/ComputerPlayer.o: src/ComputerPlayer.cpp src/./include/ComputerPlayer.h \
 src/./include/Player.h src/./include/TextureManager.h \
 src/./include/Map.h src/./include/Explosion.h \
 src/./include/FileManager.h src/./include/Configuration.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/Explosion.o: src/Explosion.cpp src/./include/Explosion.h \
 src/./include/Player.h src/./include/TextureManager.h \
 src/./include/Map.h src/./include/FileManager.h \
 src/./include/Configuration.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/FileManager.o: src/FileManager.cpp src/./include/FileManager.h \
 src/./include/Map.h src/./include/TextureManager.h \
 src/./include/Explosion.h src/./include/Player.h \
 src/./include/Configuration.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/Game.o: src/Game.cpp src/./include/Game.h src/./include/Map.h \
 src/./include/TextureManager.h src/./include/Explosion.h \
 src/./include/Player.h src/./include/Configuration.h \
 src/./include/FileManager.h src/./include/HumanPlayerArrows.h \
 src/./include/HumanPlayerWASD.h src/./include/ComputerPlayer.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/HumanPlayerArrows.o: src/HumanPlayerArrows.cpp \
 src/./include/HumanPlayerArrows.h src/./include/Player.h \
 src/./include/TextureManager.h src/./include/Map.h \
 src/./include/Explosion.h src/./include/FileManager.h \
 src/./include/Configuration.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/HumanPlayerWASD.o: src/HumanPlayerWASD.cpp \
 src/./include/HumanPlayerWASD.h src/./include/Player.h \
 src/./include/TextureManager.h src/./include/Map.h \
 src/./include/Explosion.h src/./include/FileManager.h \
 src/./include/Configuration.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/main.o: src/main.cpp src/./include/Application.h src/./include/Game.h \
 src/./include/Map.h src/./include/TextureManager.h \
 src/./include/Explosion.h src/./include/Player.h \
 src/./include/Configuration.h src/./include/FileManager.h \
 src/./include/HumanPlayerArrows.h src/./include/HumanPlayerWASD.h \
 src/./include/ComputerPlayer.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/Map.o: src/Map.cpp src/./include/Map.h src/./include/TextureManager.h \
 src/./include/Explosion.h src/./include/Player.h \
 src/./include/Configuration.h src/./include/FileManager.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/Player.o: src/Player.cpp src/./include/Player.h \
 src/./include/TextureManager.h src/./include/Map.h \
 src/./include/Explosion.h src/./include/FileManager.h \
 src/./include/Configuration.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/TextureManager.o: src/TextureManager.cpp src/./include/TextureManager.h
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@


