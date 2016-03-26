

CXXFLAGS= -std=c++11 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address
#CXXFLAGS= -std=c++11 
HWID=3

all: $(HWID)

ordinalParser.o: lambdaParser.cpp lambdaParser.h main.h
	g++ $(CXXFLAGS) -c $?

$(HWID).o: $(HWID).cpp main.h
	g++ $(CXXFLAGS) -c $?

$(HWID): $(HWID).o lambdaParser.o 
	g++ $(CXXFLAGS) -o bin/$(HWID) $(HWID).o lambdaParser.o

clean:
	rm -f *.o 

