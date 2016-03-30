

CXXFLAGS= -std=c++11 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address
#CXXFLAGS= -std=c++11 
HWID=5

all: $(HWID)

termParser.o: termParser.cpp termParser.h main.h
	g++ $(CXXFLAGS) -c $?

$(HWID).o: $(HWID).cpp main.h
	g++ $(CXXFLAGS) -c $?

$(HWID): $(HWID).o termParser.o 
	g++ $(CXXFLAGS) -o bin/$(HWID) $(HWID).o termParser.o

clean:
	rm -f *.o 

