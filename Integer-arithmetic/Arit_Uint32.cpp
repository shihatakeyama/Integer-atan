// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Arit_Uint32.c
// 
//  �����^�ϐ�(int)���g�p����atan()���v�Z���܂��B
//  CPU�̎Z�p(�w���A�|���Z�A����Z)���g��Ȃ��Ŏ�������B
//  
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"	// VC++�̃v���R���p�C�� �w�b�_�[���g�p����ꍇ


Float64 MY_atan_m(Float64 Tilt);

Uint32 toUint32(const Float64 &I1);
Float64 toFloat64(const Uint32 &I1);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 32Bit ����Z���s���܂��B 
// I1 < I2 �̊֌W�𖞂����ĉ������B
// I1��I2 �̍ŏ�ʃr�b�g�͕K��0�ɂ��ĉ������B
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
#if 1
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
#else
Uint32 Arit_multArd1(const Uint32& I1, const Uint32 &I2)
{
	Sint32 i;
	Uint64 acum = 0;
	Uint32 msk = 1; 

	i=0;
	while(1){
		
		if(I2 & msk){
			acum += (Uint64)I1 << i;
		}

		if(i >= (sizeof(Uint32)*8)-1){
			break;
		}

		i++;
		msk  <<= 1;
	}

	return (Uint32)(acum >> 31);
}
#endif

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
// int�^�������g�p����atan()���v�Z���܂��B
//
// @param
// Tilt �F atan(y/x) ��y/x�̕���
//         y/x�F 0�`0x3FFFFFFF
//         ����\�F 0x40000000 �� 1.0
// return: / 0x20000000 rad
//
//  �����̌v�Z�͕���\1bit�グ�Ă܂��B
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 Arit_atan_m(Uint32 Tilt)
{
	Uint32 adti,adti2,div;
	Uint32 seki;
	Uint32 n=1;
	Uint32 thresh = 429496730;			// about 0.4
	Uint32 accum=0;


	if((Tilt > 0x40000000))	return 0;	// �Z�o�s�\

	if(Tilt < thresh){
		adti = Tilt<<1;
	}else{
		// Tilt�̂܂܂���1.0�t�߂��������ɂ����̂ŉ��@�藝���g�p����B
		adti = Arit_divArd1<Uint32>((0x40000000-Tilt) ,(0x40000000+Tilt)) >> 1;
	}

	seki = adti;
	accum= 0;
	adti2 = Arit_multArd1(adti ,adti);

	while(1){

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
//			break;
		}

		seki = Arit_multArd1(seki , adti2);
		n	+= 2;

	}

	if(Tilt < thresh){
		return accum>>2;
	}else{
		return (Uint32)(M_PI_4 * 0x20000000) - (accum>>2);
	}
}



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// atan2()
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 Arit_atan2(Sint32 y ,Sint32 x)
{
	Uint32 tilt;
	Bool xm,ym;

	Uint32 absx = abs(x);
	Uint32 absy = abs(y);
	Sint32 atana,atana_bk;

	if(absx > absy){
		tilt = Arit_divArd1<Uint32>(absy ,absx) >> 2;
		atana = Arit_atan_m(tilt);
	}else if(absx == absy){
		// Arit_divArd1() ���������v�Z�ł��Ȃ��̂ŁB
		atana = Arit_atan_m(0x40000000);
	}else{
		tilt = Arit_divArd1<Uint32>(absx ,absy) >> 2;
		atana = (Uint32)(M_PI_2 * 0x20000000) - (Arit_atan_m(tilt));
	}
	atana_bk = atana;

	if(x<0){
		atana = (Uint32)(M_PI * 0x20000000) - atana;
	}

	if(y<0){
		atana = -atana;
	}

	return atana;
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// ���؊֐�
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
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

		// �����_������
		a = drand();

		// CPU calculation
		c = atan(a);

		// Discrete calculation
		da = (Uint32)(a*0x40000000);// & 0xFFFFFFFC;
		atn = Arit_atan_m(da);
		dc = (Float64)atn / (Float64)0x20000000;

		// Error check
		rate	= fabs(dc - c);
		if(rate >= 1e-8f){
			// Place a breakpoint here and check for errors.
			Uint32	panic = 1;
		}
		if(rate > errmax){
			errmax = rate;
		}
//--		printf("atan(%f) = %f rad error = %fl e-6\n", a ,dc,rate*10e+6);

	}

	printf("MaxError = %f e-6\n",errmax*1e+6);
}

void Arti_Atan2Test()
{
	Uint32 tilt;
	Sint32 atn2;

	Sint32 i;

	Sint32 a;
	Sint32 b;
	Float64 c;

	Uint32	da;
	Uint32	db;
	Float64	dc;
	Float64	rate;
	Float64 errmax=0;

	srand(2020);

	for(i=0;i<1000000;i++){

		// �����_������
		a = sirand();
		b = sirand();

		// CPU calculation
		c = atan2((Float64)b,(Float64)a);

		// Discrete calculation
		atn2 = Arit_atan2(b,a);
		dc = (Float64)atn2 / (Float64)0x20000000;

		// Error check
		rate	= fabs(dc - c);
		if(rate >= 1e-6f){
			// Place a breakpoint here and check for errors.
			Uint32	check = 1;
		}
		if(rate > errmax){
			errmax = rate;
		}
//--		printf("atan(%fl) = %fl rad error = %fl e-6\n", a ,dc,rate*10e+6);

	}

	printf("MaxError = %f e-6\n",errmax*1e+6);

}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
//  
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Sint32 Float32_xxTest();
void IntArti_Test()
{
	Float32_xxTest();

	printf("---- START ----\n");

	// **** �g�p��������R�����g�O���ĉ������B****
//	Arti_AtanTest();	// atan(y/x)	(y/x) = 0�`0.999999

	Arti_Atan2Test();	// atan2(y,x)	 y = intmin�`Intmax ,x = intmin�`Intmax

	printf("---- END ----\n");

	getchar();
}

