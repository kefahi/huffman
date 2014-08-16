#ifndef _HUFFMAN_H_
#define _HUFFMAN_H_

/////////////////////////
//
/////////////////////////

// An element in the Huffman tree.
struct Element
{
	Element* 	elNext;
	Element* 	elPrevious;
	ULONG 		ulFrequency;
	BYTE 		oByte;
	BOOL 		bLeaf; // flages if this element is leaf; or parent to other elements.
	Element* 	pelChild[2];
	BitsCode 	Code;
	Element( Element *&, Element *&); // A non-leaf constructor 
	Element( Element *&, Element *&, BYTE,  ULONG); // A leaf-constructor
	void PlaceInList(Element * & pelHead, Element * & pelTail); // Place the element in the passed linklist.
	void BackPropagate();
	~Element();
};

class HuffmanList
{
	Element* m_pelCurrent;
	Element* ascii2code[256];
public:
	Element* m_pelTop;

	HuffmanList(ULONG *);
	~HuffmanList();

	BitsCode Ascii2Code(BYTE);
	BOOL Code2Ascii(BOOL, BYTE&);
	ULONG aulFrequency[256];
};

#endif // _HUFFMAN_H_
