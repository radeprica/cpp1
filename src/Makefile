#for nova should be g++-5.3.0
COMP = g++
#put all your object files here
OBJS = Logger.o Exceptions.o Piece.o Puzzle.o PieceOrganizer.o ConditionalPieceIterator.o
#The executabel filename DON'T CHANGE
EXEC = ex3
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG -g -O0

$(EXEC): Main.cpp $(OBJS)
	$(COMP) Main.cpp $(OBJS) $(CPP_COMP_FLAG) -o $@ -pthread
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
Puzzle.o: Puzzle.cpp PieceOrganizer.o Piece.o Exceptions.o
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
ConditionalPieceIterator.o: ConditionalPieceIterator.cpp PieceOrganizer.o
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
PieceOrganizer.o: PieceOrganizer.cpp Piece.o
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Piece.o: Piece.cpp Exceptions.o
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Exceptions.o: Exceptions.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Logger.o: Logger.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp


clean:
	rm -f $(OBJS) $(EXEC)
