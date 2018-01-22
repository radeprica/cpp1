#for nova should be g++-5.3.0
COMP = g++
#put all your object files here
OBJS = GroupPuzzlePiecesIterator.o Exceptions.o GroupPuzzlePieces.o 
#The executabel filename DON'T CHANGE
EXEC = ex4
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG -g -O0 -pthread

$(EXEC): Main.cpp $(OBJS)
	$(COMP) Main.cpp $(OBJS) $(CPP_COMP_FLAG) -o $@
#a rule for building a simple c++ source file
#use g++ -MM main.cpp to see dependencies
GroupPuzzlePieces.o: GroupPuzzlePieces.cpp GroupPuzzlePiecesIterator.o
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
GroupPuzzlePiecesIterator.o: GroupPuzzlePiecesIterator.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp
Exceptions.o: Exceptions.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

clean:
	rm -f $(OBJS) $(EXEC)
