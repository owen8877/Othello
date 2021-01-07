# Project Othello

DIREM = bin-em
BINEM = $(DIREM)/Othello.html
OBJEM = $(DIREM)/main.o $(DIREM)/ai.o $(DIREM)/display.o $(DIREM)/element.o $(DIREM)/game.o $(DIREM)/io.o $(DIREM)/model.o $(DIREM)/player.o
EMXX = em++
EMXXFLAGS = -Wall -O2
EMLINKFLAGS = -s FULL_ES2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s LLD_REPORT_UNDEFINED -s WASM=1 --preload-file resources --preload-file shader
#EMLINKFLAGS = -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -s USE_GLFW=3 -s LLD_REPORT_UNDEFINED -s WASM=1 --preload-file resources --preload-file shader

DIR = bin
BIN = $(DIR)/Othello
OBJ = $(DIR)/main.o $(DIR)/ai.o $(DIR)/display.o $(DIR)/element.o $(DIR)/game.o $(DIR)/io.o $(DIR)/model.o $(DIR)/player.o
LIB = -Lfreeglut -Wl,-Bdynamic -lglut -lGLU -lGL -lX11 -lXxf86vm -lXi -lpthread
CXX = g++
CXXFLAGS = -Wall -O3 -std=c++0x

.PHONY: test clean cleanbackup cb win

# Wasm version

$(BINEM): $(DIREM) $(OBJEM) Makefile
	$(EMXX) $(OBJEM) -o $(BINEM) $(EMLINKFLAGS)

$(DIREM):
	mkdir $(DIREM)

$(DIREM)/main.o : main.cpp element.h base.h io.h ai.h game.h display.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/ai.o : ai.cpp ai.h game.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/display.o : display.cpp display.h element.h base.h game.h model.h shader/shader.h camera.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/element.o : element.cpp element.h base.h game.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/game.o : game.cpp game.h io.h model.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/io.o : io.cpp element.h game.h base.h io.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/model.o : model.cpp game.h base.h model.h element.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/player.o : player.cpp player.h base.h ai.h io.h element.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

# Native version

$(BIN): $(DIR) $(OBJ)
	$(CXX) $(OBJ) -o $(BIN) $(LIB)

$(DIR):
	mkdir $(DIR)

test : $(BIN)
	./$(BIN)

$(DIR)/main.o : main.cpp element.h base.h io.h ai.h game.h display.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/ai.o : ai.cpp ai.h game.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/display.o : display.cpp display.h element.h base.h game.h model.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/element.o : element.cpp element.h base.h game.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/game.o : game.cpp game.h io.h model.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/io.o : io.cpp element.h game.h base.h io.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/model.o : model.cpp game.h base.h model.h element.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/player.o : player.cpp player.h base.h ai.h io.h element.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

cleanbackup :
	rm -vf *~ *.orig .*~

cb : cleanbackup

clean :
	rm -vf $(BIN) $(OBJ)  $(BINEM) $(OBJEM)

