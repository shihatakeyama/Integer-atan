// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// rand.cpp
//
// ランダム
//
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *


#include "stdafx.h"	// VC++のプリコンパイル ヘッダーを使用する場合



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// random 0 ~ 1.0 Less than 
// 
// 参考：http://marupeke296.com/TIPS_No16_flaotrandom.html
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Float64 drand() 
{
	Uint64 v;

	v = (Uint64)rand()<<45 | (Uint64)rand()<<30 | (Uint64)rand()<<15 | (Uint64)rand();

    Uint64 res = v | 0x3ff0000000000000;
    return (*(Float64*)&res) - 1.0;	
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// random 0 ~ 1.0 Less than 
// 
// 参考：http://marupeke296.com/TIPS_No16_flaotrandom.html
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
// 参考：////http://iquilezles.org/www/articles/sfrand/sfrand.htm
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

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// signed max
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Sint32 sirand()
{
	Uint32 ret;
	Uint32 tmp;

	tmp = rand();
	ret = (Uint32)(rand()&0x01)<<30 | (Uint32)rand()<<15 | rand();

	if(tmp&0x02){
		ret |= 0x80000000;
	}

	return (Sint32)ret;
}

