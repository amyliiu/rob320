CC = gcc
CXX = g++
CXXFLAGS = -std=c++11

PROGRAMS = dec_to_hex memory_layout packed padded cpp_oop c_oop callback

all: $(PROGRAMS)

dec_to_hex: dec_to_hex.c
	$(CC) -o dec_to_hex dec_to_hex.c

memory_layout: memory_layout.c
	$(CC) -o memory_layout memory_layout.c

packed: packed.c
	$(CC) -o packed packed.c

padded: padded.c
	$(CC) -o padded padded.c

cpp_oop: cpp_oop.cpp
	$(CXX) $(CXXFLAGS) -o cpp_oop cpp_oop.cpp

c_oop: c_oop.c
	$(CC) -o c_oop c_oop.c

callback: callback.c
	$(CC) -o callback callback.c

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean