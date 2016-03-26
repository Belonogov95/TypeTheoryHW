

CXXFLAGS= -std=c++11 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address
#CXXFLAGS= -std=c++11 

all: 1

ordinalParser.o: lambdaParser.cpp lambdaParser.h main.h
	g++ $(CXXFLAGS) -c $?

1.o: 1.cpp main.h
	g++ $(CXXFLAGS) -c $?

1: 1.o lambdaParser.o 
	g++ $(CXXFLAGS) -o bin/1 1.o lambdaParser.o

clean:
	rm -f *.o 

