CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3

all: ZipCraft

ZipCraft: src/main.o src/Huffman.o
	$(CXX) $(CXXFLAGS) src/main.o src/Huffman.o -o ZipCraft

src/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o src/main.o

src/Huffman.o: src/Huffman.cpp
	$(CXX) $(CXXFLAGS) -c src/Huffman.cpp -o src/Huffman.o

clean:
	rm -f src/*.o ZipCraft *.txt *.bin

	