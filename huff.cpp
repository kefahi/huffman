#include <stdio.h>
#include <stdlib.h>
#include "iobs.h"
#include "huffman.h"

int main(int argc, char ** argv) {

	if(argc<2) {
		printf("Usage: %s uncompressedfile compressedfile \n", argv[0]);
		return 0;
	}

	//int filehandler;
	FILE* pfile = NULL;
	ULONG ulFileLen=0;
	ULONG i;
	BYTE byte;
	ULONG aulFrequency[256];

	// fill aulFrequency array
	for(i=0; i<256; i++)
		aulFrequency[i]=0;
	
	pfile=fopen(argv[1], "rb");
	
	if( pfile != NULL ); {
		printf("Building the aulFrequency table ");
		fseek(pfile, 0, SEEK_END);
		if(ftell(pfile) <= 0) {
			printf("Error : main : Invalid file size: %s.\n", argv[1]);
			exit(1);
		}
		
		ulFileLen = ftell(pfile);
		rewind(pfile);
		
		for(i=0; i< ulFileLen; i++) {
			if(fread(&byte, 1, 1, pfile)!=1) {
					printf("Error in reading from file.\n");
					exit(1);
			}

			aulFrequency[byte]++;
			if(i%10000==0)
				printf(".");
		}
		//fclose(pfile);
		printf("[Done]\n");

		printf("Building Huffman binary tree ");
		HuffmanList huffman_list(aulFrequency);
		printf("[Done]\n");

		IOBS obs(argv[2], IOBS::OUTPUT);
		// save Frequency
		for (i=0; i<256; i++) {
			BitsCode bc;
			bc.ulCode=huffman_list.aulFrequency[i];
			bc.ulBits=32;
			obs.Write(bc);
		}

		rewind(pfile);

		// save file length
		BitsCode bc;
		bc.ulCode=ulFileLen;
		bc.ulBits=32;
		obs.Write(bc);

		printf("Compressing file: \n");
		
		// now , Compress
		for(i=0; i< ulFileLen; i++) {
			if( fread(&byte, 1, 1, pfile)!=1) {
				printf("Error in reading file.\n");
				exit(1);
			}
				
			BitsCode bcTemp;
			bcTemp = huffman_list.Ascii2Code(byte);
			obs.Write(bcTemp);
				
			if(i%10000==0)
				printf("o");
		}
		
		fclose(pfile);
		printf("[Done]\n");
	}

	return 0;
}
