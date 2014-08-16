


all: huff unhuff

huff: huff.cpp huffman.cpp iobs.cpp huffman.h iobs.h bitscode.h
	g++ -Wall -gstabs+ -o huff huff.cpp iobs.cpp huffman.cpp

unhuff: unhuff.cpp huffman.cpp iobs.cpp huffman.h iobs.h bitscode.h
	g++ -Wall -gstabs+ -o unhuff unhuff.cpp iobs.cpp huffman.cpp



clean:
	rm -f huff unhuff 
