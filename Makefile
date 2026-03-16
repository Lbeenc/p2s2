CXX = g++
CXXFLAGS = -Wall -g -std=c++17
TARGET = P2
OBJS = main.o scanner.o testScanner.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp scanner.h testScanner.h
	$(CXX) $(CXXFLAGS) -c main.cpp

scanner.o: scanner.cpp scanner.h token.h
	$(CXX) $(CXXFLAGS) -c scanner.cpp

testScanner.o: testScanner.cpp testScanner.h scanner.h token.h
	$(CXX) $(CXXFLAGS) -c testScanner.cpp

clean:
	rm -f $(TARGET) $(OBJS)
