// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Arit_sub.h
// 
//
//  �Z�p
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"	// VC++�̃v���R���p�C�� �w�b�_�[���g�p����ꍇ


#define	PLUS	0
#define MINUS	1

template<typename X>	
Exp8 leftZero8(const X &Val)
{
	Exp8 cnt = 0;
	X msk = X(1) << ((sizeof(X)*8) -1);

	while(1){
		if((Val & msk) || (cnt == (sizeof(X)*8) )){
			return cnt;
		}

		msk >>= 1;
		cnt++;
	}
}

template<typename X>	
Exp16 leftZero16(const X &Val)
{
	Exp16 cnt = 0;
	X msk = X(1) << ((sizeof(X)*8) -1);

	while(1){
		if((Val & msk) || (cnt == (sizeof(X)*8) )){
			return cnt;
		}

		msk >>= 1;
		cnt++;
	}
}


