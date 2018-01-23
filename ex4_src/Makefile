#for nova should be g++-5.3.0
COMP = g++

OBJS = Exceptions.o

EXEC = ex4
CPP_COMP_FLAG = -std=c++14 -Wall -Wextra \
-Werror -pedantic-errors -DNDEBUG -g -O0

$(EXEC): Main.cpp $(OBJS)
	$(COMP) Main.cpp $(OBJS) $(CPP_COMP_FLAG) -o $@
Exceptions.o: Exceptions.cpp
	$(COMP) $(CPP_COMP_FLAG) -c $*.cpp

clean:
	rm -f $(OBJS) $(EXEC)
