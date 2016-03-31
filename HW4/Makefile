

#CXXFLAGS= -std=c++11 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address -Wall
CXXFLAGS= -std=c++11 -O3 -Wno-unused-result -Wall
#CXXFLAGS= -std=c++11 -pg -Wno-unused_resultt
HWID=4
path=../LambdaParser/
all: $(HWID)

lambdaParser.o: $(path)lambdaParser.cpp $(path)lambdaParser.h $(path)main.h
	g++ $(CXXFLAGS) -c $?

$(HWID).o: $(HWID).cpp $(path)main.h
	g++ $(CXXFLAGS) -c $?

$(HWID): $(HWID).o lambdaParser.o 
	g++ $(CXXFLAGS) -o bin/$(HWID) $(HWID).o lambdaParser.o

clean:
	rm -f *.o 
	rm -f bin/*

