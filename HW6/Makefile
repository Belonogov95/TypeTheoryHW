

CXXFLAGS= -std=c++11 -Wno-unused-result -D_GLIBCXX_DEBUG -fsanitize=address
#CXXFLAGS= -std=c++11 
HWID=6
path=../LambdaParser/
all: $(HWID)

$(HWID): $(path)lambdaParser.cpp 6.cpp termParser.cpp 
	g++ $(CXXFLAGS) -o bin/$(HWID) $(path)lambdaParser.cpp 6.cpp termParser.cpp 

clean:
	rm -f *.o 
	rm -f bin/*
