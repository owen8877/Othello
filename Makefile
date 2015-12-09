#Project Othello

DIR = bin
BIN = $(DIR)/Othello
OBJ = $(DIR)/main.o $(DIR)/ai.o $(DIR)/display.o $(DIR)/element.o $(DIR)/game.o $(DIR)/io.o $(DIR)/model.o $(DIR)/player.o
LIB = -Lfreeglut -Wl,-Bstatic -lglut -lGLU -Wl,-Bdynamic -lGL -lX11 -lXxf86vm -lXi
CXX = g++
#CXXFLAGS = -Wall -g3 -std=c++0x
CXXFLAGS = -Wall -O2 -std=c++0x

.PHONY: test clean cleanbackup cb win

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
	rm -vf $(BIN) $(OBJ)
