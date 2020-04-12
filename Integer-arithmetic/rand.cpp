// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// rand.cpp
//
// �����_��
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include "stdafx.h"	// VC++�̃v���R���p�C�� �w�b�_�[���g�p����ꍇ


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// random 0 ~ 1.0 Less than 
// 
// �Q�l�Fhttp://marupeke296.com/TIPS_No16_flaotrandom.html
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Float32 frand() 
{
	Uint32 v;

	v = rand();
	v *= rand();

    unsigned res = v | 0x3f800000;
    return (*(float*)&res) - 1.0f;	
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// random() -1.0 More ~ +1.0 Less than 
//  
// �Q�l�F////http://iquilezles.org/www/articles/sfrand/sfrand.htm
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Float32 sfrand()
{
	union {
		Uint32	u;
		Float32 f;
	};
	Uint32 v;

	v = rand();
	v *= rand();

    u = v | 0x3f800000;
	f = f -1.0f;

	if(v & 0x00800000){
		f = -f;	// minus
	}

	return f;
}

Float32 Float32MaxRand()
{
	Uint32 v = rand();

	v = rand();
	v *= rand();

	if((v & 0x3f800000) == 0)	v = 0;

    return (*(float*)&v);
}
