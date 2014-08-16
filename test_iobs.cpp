#include "iobs.h"
#include <stdio.h>
#include <string.h>
#include "huffman.h"


int main(int argc, char** argv) {
	{
		IOBS iobs(argv[1], IOBS::OUTPUT);

		BitsCode bc;
		bc.ulCode = *(ULONG*)(PCHAR)"1234";
		bc.ulBits = 32;

		printf("Writing : ulCode %ld, ulBits %ld.\n", bc.ulCode, bc.ulBits);
	
		iobs.Write(bc);
	}

	{
		IOBS iobs(argv[2], IOBS::INPUT);

		BitsCode bc;
		
		bc = iobs.Read(32);
		CHAR ch[5];
		memcpy(ch, (PCHAR)&(bc.ulCode), 4);
		ch[4] = '\0';
		
		printf("Value read:%s\n", ch);
		printf("Read: ulCode %ld, ulBits %ld.\n", bc.ulCode, bc.ulBits);	
	}
	
	return 0;
}	
