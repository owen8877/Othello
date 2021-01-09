# Project Othello

DIREM = bin-em
BINEM = $(DIREM)/Othello.html
OBJEM = $(DIREM)/main.o $(DIREM)/ai.o $(DIREM)/display.o $(DIREM)/element.o $(DIREM)/game.o $(DIREM)/io.o $(DIREM)/model.o $(DIREM)/player.o
EMXX = em++
EMXXFLAGS = -Wall -O2
EMLINKFLAGS = -s FULL_ES2=1 -s FULL_ES3=1 -s USE_GLFW=3 -s LLD_REPORT_UNDEFINED -s WASM=1 --preload-file resources --preload-file render --preload-file Settings

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

$(DIREM)/main.o : main.cpp core/element.h base.h control/io.h core/ai.h core/game.h render/display.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/ai.o : core/ai.cpp core/ai.h core/game.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/display.o : render/display.cpp render/display.h core/element.h base.h core/game.h control/model.h render render/camera.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/element.o : core/element.cpp core/element.h base.h core/game.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/game.o : core/game.cpp core/game.h control/io.h control/model.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/io.o : control/io.cpp core/element.h core/game.h base.h control/io.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/model.o : control/model.cpp core/game.h base.h control/model.h core/element.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

$(DIREM)/player.o : core/player.cpp core/player.h base.h core/ai.h control/io.h core/element.h
	$(EMXX) $(EMXXFLAGS) -c $< -o $@

# Native version

$(BIN): $(DIR) $(OBJ)
	$(CXX) $(OBJ) -o $(BIN) $(LIB)

$(DIR):
	mkdir $(DIR)

test : $(BIN)
	./$(BIN)

$(DIR)/main.o : main.cpp core/element.h base.h control/io.h core/ai.h core/game.h render/display.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/ai.o : core/ai.cpp core/ai.h core/game.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/display.o : render/display.cpp render/display.h core/element.h base.h core/game.h control/model.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/element.o : core/element.cpp core/element.h base.h core/game.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/game.o : core/game.cpp core/game.h control/io.h control/model.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/io.o : control/io.cpp core/element.h core/game.h base.h control/io.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/model.o : control/model.cpp core/game.h base.h control/model.h core/element.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/player.o : core/player.cpp core/player.h base.h core/ai.h control/io.h core/element.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

cleanbackup :
	rm -vf *~ *.orig .*~

cb : cleanbackup

clean :
	rm -vf $(BIN) $(OBJ)  $(BINEM) $(OBJEM)

