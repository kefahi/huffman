#include "iobs.h"
#include "huffman.h"
#include <assert.h>

// Takes the last two elements out of the linklist and inserts itself in the proper place instead of th:em.
// Its frequency is the sum of the two childern.
Element::Element(Element * & pelHead, Element * & pelTail) {
	// pelHead and pelTail mustnot be NULL. Also, the list mustnot have one element only.
	assert(pelHead != NULL && pelTail != NULL && pelHead != pelTail);

	// my children are the last two
	pelChild[0]=pelTail;
	pelChild[1]=(pelTail)->elPrevious;

	// this is a combination constructor, it does the combination and resort
	ulFrequency = pelChild[0]->ulFrequency + pelChild[1]->ulFrequency;
	bLeaf=FALSE;
	oByte=0;

	// after becoming my childs, i shall remove them from the link list
	if(pelChild[1]->elPrevious != NULL) {
		(pelChild[1]->elPrevious)->elNext=NULL;
		pelTail=(pelChild[1]->elPrevious);
	} else {	// No more nodes left.
		pelHead = pelTail = NULL;
	}

	PlaceInList(pelHead, pelTail);

	// If this is the root parent, do recursive encoding.
	if(pelHead == pelTail)
		BackPropagate();
}

// A leaf-element constructor. 
// Place the element in the linklist (passed in Head and Tail) sorted in descending order. 
Element::Element(Element* &pelHead, Element* & pelTail, BYTE oByteIn, ULONG _ulFrequency) {
	ulFrequency = _ulFrequency;
	oByte = oByteIn;
	bLeaf = TRUE;
	pelChild[0]=NULL;
	pelChild[1]=NULL;

	assert(ulFrequency!=0);

	PlaceInList(pelHead, pelTail);
}

void Element::BackPropagate() {
	if(bLeaf)
		return;

	assert(pelChild[0] != NULL && pelChild[1] != NULL);

	// give my childern my heritage
	pelChild[1]->Code=Code;
	pelChild[1]->Code.AddBit(TRUE);
	pelChild[1]->BackPropagate();

	pelChild[0]->Code=Code;
	pelChild[0]->Code.AddBit(FALSE);
	pelChild[0]->BackPropagate();
}


void Element::PlaceInList(Element * & pelHead, Element * & pelTail) {
	//	Either both NULL or both not NULL.
	assert( (pelHead == NULL && pelTail == NULL) || (pelHead != NULL && pelTail != NULL));
		
	// The first element in the list.
	if(pelTail == NULL) {
		pelTail = pelHead = this;
		elNext = elPrevious = NULL;
		return;
	}
	
	Element *pel = NULL;
	for(pel=pelTail; pel!=NULL; pel=pel->elPrevious) {
		if( (ulFrequency >= pel->ulFrequency) && 
		( (pel->elPrevious == NULL) || (ulFrequency <= (pel->elPrevious)->ulFrequency)) )
			break;
	}

	elNext=pel;

	if(pel!=NULL) {
		elPrevious=pel->elPrevious;
		pel->elPrevious=this;
		if(elPrevious!=NULL)
			elPrevious->elNext=this;
		else
			pelHead=this;
	} else {
		elPrevious=pelTail;
		pelTail=this;
		elPrevious->elNext=this;
	}
}


Element::~Element() {
	// first i shall delete my childern
	if((pelChild[0] != NULL) && (pelChild[1]!=NULL)) {
		delete pelChild[0];
		delete pelChild[1];
	}
}


/////////////////////////

HuffmanList::HuffmanList(ULONG * pulFrequency) {
	// creat elements --> linklist
	Element *pelHead=NULL, *pelTail=NULL;

	assert (pulFrequency != NULL);

	for(ULONG uli=0; uli<256; uli++) {
		aulFrequency[uli]=pulFrequency[uli];
		if(aulFrequency[uli] != 0)
			ascii2code[uli]=new Element(pelHead,pelTail,uli,aulFrequency[uli]);
		else
			ascii2code[uli] = NULL; 
	}

	// generate Huffman Code
	while(pelTail!=pelHead)
		new Element(pelHead,pelTail);

	m_pelCurrent = m_pelTop = pelHead; 
}

BOOL HuffmanList::Code2Ascii(BOOL b, BYTE& o) {
	m_pelCurrent = m_pelCurrent->pelChild[b?1:0];

	if(m_pelCurrent->bLeaf) {
		o = m_pelCurrent->oByte;
		m_pelCurrent = m_pelTop;
		return TRUE;
	}
	else
		return FALSE;
}

BitsCode HuffmanList::Ascii2Code(BYTE  oIndex) { return ascii2code[oIndex]->Code; }

HuffmanList::~HuffmanList() { delete m_pelTop; }

