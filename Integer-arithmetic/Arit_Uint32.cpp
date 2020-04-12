// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Arit_Uint32.c
// 
//  �����^�ϐ�(int)���g�p����atan()���v�Z���܂��B
//  CPU�̎Z�p(�w���A�|���Z�A����Z)���g��Ȃ��Ŏ�������B
//  
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"	// VC++�̃v���R���p�C�� �w�b�_�[���g�p����ꍇ


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 32Bit ����Z���s���܂��B 
// Numerator < Denominator (�^����)�̊֌W�𖞂����ĉ������B
// Numeratort��Denominator�̍ŏ�ʃr�b�g�͕K��0�ɂ��ĉ������B
// ����Z����������0x100000000 ���|�����l���߂�l�Ƃ��Ė߂�܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
template<typename X>
inline X Arit_divArd1(X I1 ,X I2)
{
	X ret = 0;
	Sint32 i;

	if((I1 > I2) || (I1 & ((X)1 << (sizeof(I1)*8)-1))){
		return 0;
	}

	for(i=32;i>=0;i--){

		if(I1 >= I2){
			ret |= 1 << i;
			I1 -= I2;
		}

		I1 <<= 1;
	}

	return ret;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  1�t�߂̊|���Z  Around 1
// ����\ 0x80000000 �� 1.0
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 Arit_multArd1(const Uint32& I1, const Uint32 &I2)
{
	Sint32 i;
	Uint32 acum = 0;
	Uint32 msk = 1; 

	i=0;
	while(1){
		
		if(I2 & msk){
			acum += I1;
		}

		if(i >= (sizeof(Uint32)*8)-1){
			break;
		}

//		�l�̌ܓ�
//		if(acum & 0x00000001)	acum += 2;

		i++;
		acum >>= 1;
		msk  <<= 1;
	}

	return acum;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Division
// ����\ 1 �� 1.0
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 Arit_Div(const Uint32& I1, const Uint32 &I2)
{
	Sint32 i;
	Uint32 numerator_msb = I1;	// use 64-bit variables, but keep them as 32-bit variables
	Uint32 numerator=0;
	Uint32 denominator = I2;
	Uint32 msk = 0x80000000;
	Uint32 ans=0;

	i = 0;
	while(1){

		numerator <<= 1;
		numerator |= numerator_msb >> ((sizeof(Uint32)*8)-1);
		numerator_msb <<= 1;

		if((numerator >= denominator)){
			numerator -= denominator;
			ans |= msk;
		}

		if(i >= (sizeof(Uint32)*8)-1){
			break;
		}
		i++;
		msk  >>= 1;
	}

	// �����ȉ����l�̌ܓ��������ꍇ
//	if((numerator<<1) >= denominator)	ans++;

	return ans;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// atan()
// Tilt �F y/x 0�`0x7FFFFFFF  (0.99999999976) 
//             ����\ 0x80000000 �� 1.0
// return: / 0x80000000 rad
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 Arit_atan_m(Uint32 Tilt)
{
	Uint32 adti,adti2,div;
	Uint32 seki;
	Uint32 n=1;
	Uint32 thresh = 858993459;			// about 0.4pi
	Uint32 accum=0;


	if((Tilt > 0x7FFFFFFF))	return 0;	// �Z�o�s�\

	if(Tilt < thresh){
		adti = Tilt;
	}else{
		// Tilt�̂܂܂���1.0�t�߂��������ɂ����̂ŉ��@�藝���g�p����B
		adti = Arit_divArd1<Uint32>((0x80000000-Tilt)>>1 ,(0x80000000+Tilt)>>1) >> 1;
//		adti = (Uint32)(((1.0-(Float64)Tilt/(Float64)0x80000000)/(1.0+(Float64)Tilt/(Float64)0x80000000)) * (Float64)0x80000000);
	}

	seki = adti;
	accum= 0;
	adti2 = Arit_multArd1(adti ,adti);

	while(1){

//		arctan = seki / n;
		div = Arit_Div(seki, n);

		if(n & 0x02){
			accum -= div;
		}else{
			accum += div;		
		}

		if(div == 0){
			break;		
		}
		if(n >= 40){
			break;
		}

		seki = Arit_multArd1(seki , adti2);
		n	+= 2;
	}

	if(Tilt < thresh){
		return accum;
	}else{
		return (Uint32)(M_PI_4 * 0x80000000) - accum;
	}
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// atan2()
// ���얢�m�F 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 Arit_atan2_m(Uint32 y ,Uint32 x)
{
	Uint32 tilt;

	tilt = Arit_divArd1<Uint32>(y ,x);

	return Arit_atan_m(tilt);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���؊֐�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void IntArti_DivTest()
{
	Uint32 ans;
		
	ans = Arit_Div(7, 1);

}

void Arti_AtanTest()
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

		a = frand();

		// CPU calculation
		c = atan(a);

		// Discrete calculation
		da = (Uint32)(a*0x80000000);
		atn = Arit_atan_m(da);
		dc = (Float64)atn / (Float64)0x80000000;

		// Error check
		rate	= fabs(dc - c);
		if(rate >= 0.0000001f){
			// Place a breakpoint here and check for errors.
			Uint32	panic = 1;
		}
		if(rate > errmax){
			errmax = rate;
		}
//		printf("atan(%fl) = %fl rad error = %fl e-6\n", a ,dc,rate*10e+6);

	}

	printf("MaxError = %fl e-6\n",errmax*10e+6);
}
void IntArti_Test()
{
	printf("---- START ----\n");

//	IntArti_DivTest();

	// �|���Z
	Arti_AtanTest();

	printf("---- END ----\n");

	getchar();
}
