#Project Othello

DIR = bin
BIN = $(DIR)/Othello
OBJ = $(DIR)/main.o $(DIR)/element.o $(DIR)/game.o
LIB = -Lfreeglut -Wl,-Bstatic -lglut -Wl,-Bdynamic -lGL -lX11 -lXxf86vm -g3 -lXi
CXX = g++
CXXFLAGS = -I"freeglut_l/include" -Wall -g3 -std=c++0x

.PHONY: test clean cleanbackup cb win

$(BIN): $(DIR) $(OBJ)
	$(CXX) $(OBJ) -o $(BIN) $(LIB)

$(DIR):
	mkdir $(DIR)

test : $(BIN)
	./$(BIN)

$(DIR)/main.o : main.cpp element.h base.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/element.o : element.cpp element.h base.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(DIR)/game.o : game.cpp game.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

cleanbackup :
	rm -vf *~ *.orig .*~

cb : cleanbackup

clean :
	rm -vf $(BIN) $(OBJ)
