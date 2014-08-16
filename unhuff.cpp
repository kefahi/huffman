#include <stdio.h>
#include "iobs.h"
#include "huffman.h"

int main(int argc, char ** argv) {

	if(argc<2) {
		printf("Usage: %s compressedfile uncompressedfile \n", argv[0]);
		return 0;
	}


	IOBS  ibs(argv[1], IOBS::INPUT);

	ULONG i;

	ULONG frequency[256];
	// Load Frequency
	// ! check if the loading is correct!
	printf("Re-building the frequency table : ");

	for( i=0; i< 256; i++) {
		BitsCode bc;
		bc=ibs.Read(32);
		frequency[i]=bc.ulCode;
	}
	printf(" [Done]\n");

	ULONG filelength;
	BitsCode bc;
	bc=ibs.Read(32);
	filelength=bc.ulCode;

	printf("Re-building Huffman binary tree : ");

	HuffmanList huffman_list(frequency);
	printf(" [Done]\n");

	// start Decompression
	FILE * file;
	if( (file=fopen(argv[2], "wb"))!=NULL) {
		//unsigned char byte;

		printf("Uncompressing file : \n");
		for(i=0; i< filelength; i++) {
			BYTE byte;
			//BOOL b;
			//do
			//{
			//	b = ibs.Read();
			//	byte = huffman_list.Code2Ascii(b);
			//}while (byte == (BYTE) -1);
			
			while(huffman_list.Code2Ascii(ibs.Read(),byte)==FALSE) {}
			fwrite(&byte,1 ,1, file); // check that the lower byte is saved
			if(i%10000==0)
				printf(".");
		}

		printf(" [Done]\n");

		fclose(file);
	}

	return 0;
}
