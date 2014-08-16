#include "iobs.h"
#include <stdlib.h>
#include <assert.h>


IOBS::IOBS(PCHAR pchFileName, eOpenMode eMode) {
	m_pchFileName = pchFileName;
	m_eMode = eMode;
	if(m_pchFileName == NULL || m_pchFileName[0] == '\0') {
			printf("Error : IOBS::IOBS Empty file name passed!");
			exit(1);
	}
	
	m_ulLength = 0;
	m_ulBytePosition = 0;
	m_bcResdue.ulCode = 0;
	m_bcResdue.ulBits = 0;

	if(m_eMode == INPUT) {
		if( (m_pfile = fopen(m_pchFileName, "rb")) <= 0 ) {
			printf("Error : IOBS::IOBS File %s is not found! \n", m_pchFileName) ;
			exit(1);
		}

		// Get file size.
		fseek(m_pfile, 0, SEEK_END);
		if(ftell(m_pfile) <= 0) {
				printf("ERROR : IOBS::IOBS Invalid file size %s.\n", m_pchFileName);
				exit(1);
		}

		m_ulLength= (ULONG)ftell(m_pfile);
		rewind (m_pfile);
	} else if(m_eMode == OUTPUT) {
		if( (m_pfile = fopen(m_pchFileName,"wb") ) == NULL ) {
			printf("Error : IOBS::IOBS Cannot creat File %s for write.\n", m_pchFileName);
			exit(1);
		}
	}
}


BOOL IOBS::Read() {
	assert (m_pfile != NULL);
	assert (m_eMode == INPUT);

	if( (m_ulBytePosition==m_ulLength) && (m_bcResdue.ulBits==0) ) {
		fclose(m_pfile);
		m_pfile = NULL;
		printf("Error : IOBS::Read : File ended prematurly.\n");
		exit(1);
	}


	if(m_bcResdue.ulBits==0) {
		BYTE o;
		m_ulBytePosition++;
		
		if(fread(&o,1,1,m_pfile) != 1) {
			printf("Error : IOBS::Read : While reading from file.\n");
			exit(1);
		}

		m_bcResdue.ulCode=o;
		m_bcResdue.ulBits=8;
	}

	m_bcResdue.ulBits--;
	return m_bcResdue.ulCode & 1<<m_bcResdue.ulBits;
}



BitsCode IOBS::Read(ULONG uln) {
	BitsCode bc;
	for(ULONG uli=0; uli < uln ;uli++)
		bc.AddBit(Read());

	return bc;
}

void IOBS::Write(BOOL b) {
	assert (m_pfile != NULL);
	assert (m_eMode == OUTPUT);

	m_bcResdue.AddBit(b);
	if(m_bcResdue.ulBits/8 >0) {
		unsigned char byte=m_bcResdue.ulCode;
		fwrite(&byte,1,1,m_pfile);
		m_ulLength++;
		m_bcResdue.ulCode=0;
		m_bcResdue.ulBits=0;
	}

	return;
}


void IOBS::Write( BitsCode& bc) {
	 for(ULONG uli=bc.ulBits; uli>0; uli--) {
		 //Write( check_bit(bc.Code, 32-i) );
		 if(uli<17) {
					Write( (1<<(uli-1)) & (bc.ulCode) );
		 } else {
			ULONG ulc=bc.ulCode;
			ulc>>=16;
			Write ( (1<<(uli-17)) & (ulc) );
		 }
	 }
}

IOBS::~IOBS() {
	if(m_pfile == NULL)
			return;
	
	if(m_bcResdue.ulBits!=0 && m_eMode == OUTPUT) {
		m_bcResdue.ulCode<<= (8-m_bcResdue.ulBits);
		BYTE o = m_bcResdue.ulCode;
		fwrite(&o,1,1,m_pfile);
	}

	fclose(m_pfile);
}



