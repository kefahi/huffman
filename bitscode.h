#ifndef _BITSCODE_H_
#define _BITSCODE_H_

class BitsCode {
public:
	BitsCode() : ulCode(0), ulBits(0) {}
	BitsCode(const BitsCode& bc) {ulCode = bc.ulCode; ulBits = bc.ulBits;}
	BitsCode(ULONG ulc, ULONG uln) : ulCode(ulc), ulBits(uln) {}

	ULONG 	ulCode;
	ULONG	ulBits;  // number of bits in a Code

	void AddBit(BOOL b) {
		//ulCode <<=1;
		//if (b)
		//	ulCode |=1;
		ulCode = (ulCode << 1) | (b);
		ulBits++;
	}


	BitsCode& operator=(BitsCode bc) {
		ulCode=bc.ulCode;
		ulBits=bc.ulBits;
		return *this;
	}

	BOOL operator==(BitsCode& bc) {return ( ulCode==bc.ulCode && ulBits==bc.ulBits);}
	BOOL operator!=(BitsCode& bc) {return ulCode!=bc.ulCode || ulBits!=bc.ulBits;}
};

#endif // _BITSCODE_H_
