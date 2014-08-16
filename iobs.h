#ifndef _IOBS_H_
#define _IOBS_H_

#include <stdio.h>

#define ULONG 	unsigned long int
#define BYTE 	unsigned char
#define PBYTE 	BYTE*
#define CHAR 	char
#define PCHAR 	CHAR*
#define BOOL	bool
#define TRUE	true
#define FALSE	false

#include "bitscode.h"

// Input Output Bit Stream  
class IOBS {
public:
	enum eOpenMode{ INPUT, OUTPUT };
	IOBS(PCHAR, eOpenMode);
	BOOL Read(); // reads a single bit
	BitsCode Read(ULONG); // reads  a number of bits
	void Write(BOOL); //writes a single bit
	void Write(BitsCode&);
	~IOBS();
protected:
	eOpenMode m_eMode; // INPUT or OUTPUT
	FILE * m_pfile;
	ULONG m_ulBytePosition;
	ULONG m_ulLength;
	PCHAR m_pchFileName;
	BitsCode m_bcResdue;
};

#endif // _IOBS_H_
