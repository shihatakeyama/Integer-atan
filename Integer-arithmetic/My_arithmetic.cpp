// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// My_arithmetic.cpp
// 
//  C����̊i�[�^ doub���g�p���Čv�Z����B
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"	// VC++�̃v���R���p�C�� �w�b�_�[���g�p����ꍇ


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �������l��0�ł���r�b�g�̌���Ԃ��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Exp8 leftZero(const Uint32 &Val)
{
	Exp8 cnt = 0;
	Uint32 msk = 0x80000000;

	while(1){
		if((Val & msk) || (cnt == (sizeof(Uint32)*8) )){
			return cnt;
		}

		msk >>= 1;
		cnt++;
	}
}
Exp8 leftZero(const Uint64 &Val)
{
	Exp8 cnt = 0;
	Uint64 msk = 0x8000000000000000l;

	while(1){
		if((Val & msk) || (cnt == (sizeof(Uint64)*8) )){
			return cnt;
		}

		msk >>= 1;
		cnt++;
	}
}

#if 1
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// �ϊ�
//             ����\ 0x80000000 �� 1.0
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 toUint32(const Float64 &I1)
{
	const Sint32 reso = 31;
	Sint32 keta;
	Uint64 exp;
	Uint64 data = (*(Uint64*)&I1);	
	Uint32 res;

	if(I1 == 0.0)	return 0;

	exp	= (data & 0x3ff0000000000000)>>52;
	res = ((data & 0xFFFFFFFFFFFFF)|0x10000000000000)>>(0x3ff-exp + reso - 10);

	return res;
}

Float64 toFloat64(const Uint32 &I1)
{
	const Sint32 reso = 31;
	Sint32 keta;
	Uint64 exp;

	if(I1 == 0)		return 0.0;

	keta = leftZero(I1);

	exp = -keta - reso + 0x3ff +31;

	Uint64 res = ((exp<<52) & 0x3ff0000000000000)	| ((Uint64)I1 << (keta+21) & 0xFFFFFFFFFFFFF);

    return (*(Float64*)&res);	
}
#endif


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Float64 MY_atan_m(Float64 Tilt)
{
	Float64 adti,adti2,div;
	Float64 seki;
	Uint32 n=1;
	Float64 thresh = 0.4;			// about 0.4pi
	Float64 accum=0;


	if((Tilt > 1.0f))	return 0;	// �Z�o�s�\

	if(Tilt < thresh){
		adti = Tilt;
	}else{
		// Tilt�̂܂܂���1.0�t�߂��������ɂ����̂ŉ��@�藝���g�p����B
		adti = (1.0-Tilt) / (1.0+Tilt);
	}

	seki = adti;
	accum= 0;
	adti2 = adti * adti;

	while(1){

		div = seki / n;

		if(n & 0x02){
			accum -= div;
		}else{
			accum += div;		
		}

		if(div == 0.0){
			break;		
		}
		if(n >= 40){
//			break;
		}

		seki = seki * adti2;
		n	+= 2;

	}

	if(Tilt < thresh){
		return accum;
	}else{
		return M_PI_4 - accum;
	}
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���؊֐��̌Ăь�
// ���؂��������Z�̃R�����g���O���ĉ������B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Sint32 Float32_xxTest()
{
	Uint32 ans= toUint32(0.5);
	Float64 ans2 = toFloat64(ans);

	return 0;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���؊֐�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void My_AtanTest()
{
	Uint32 tilt,atn;

	Sint32 i;

	Float64 a;
	Uint32 b;
	Float64 c;

	Uint32	da;
	Float64	db;
	Float64	dc;
	Float64	rate;
	Float64 errmax=0;

	srand(2020);

	for(i=0;i<1000000;i++){

		// �����_������
		a = drand();

		// CPU calculation
		c = atan(a);

		// Discrete calculation
		dc = MY_atan_m(a);

		// Error check
		rate	= fabs(dc - c);
		if(rate >= 6e-10){
			// Place a breakpoint here and check for errors.
			Uint32	panic = 1;
		}
		if(rate > errmax){
			errmax = rate;
		}
//--		printf("atan(%fl) = %fl rad error = %fl e-6\n", a ,dc,rate*10e+6);

	}

	printf("MaxError = %f e-6\n",errmax*1e+6);
}

void MyArti_Test()
{
	Float32_xxTest();

	printf("---- START ----\n");

	My_AtanTest();

	printf("---- END ----\n");

	getchar();
}

