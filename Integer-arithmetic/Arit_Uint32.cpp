// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Arit_Uint32.c
// 
//  整数型変数(int)を使用してatan()を計算します。
//  CPUの算術(指数、掛け算、割り算)を使わないで実現する。
//  
// 
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

#include "stdafx.h"	// VC++のプリコンパイル ヘッダーを使用する場合


Float64 MY_atan_m(Float64 Tilt);

Uint32 toUint32(const Float64 &I1);
Float64 toFloat64(const Uint32 &I1);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// 32Bit 割り算を行います。 
// I1 < I2 の関係を満たして下さい。
// I1とI2 の最上位ビットは必ず0にして下さい。
// 割り算した答えに0x100000000 を掛けた値が戻り値として戻ります。
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
//  1付近の掛け算  Around 1
// 分解能 0x80000000 → 1.0
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

//		四捨五入
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
// 分解能 1 → 1.0
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

	// 少数以下を四捨五入したい場合
//	if((numerator<<1) >= denominator)	ans++;

	return ans;
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// int型整数を使用してatan()を計算します。
//
// @param
// Tilt ： atan(y/x) のy/xの部分
//         y/x： 0～0x3FFFFFFF
//         分解能： 0x40000000 → 1.0
// return: / 0x20000000 rad
//
//  内部の計算は分解能1bit上げてます。
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
Uint32 Arit_atan_m(Uint32 Tilt)
{
	Uint32 adti,adti2,div;
	Uint32 seki;
	Uint32 n=1;
	Uint32 thresh = 429496730;			// about 0.4
	Uint32 accum=0;


	if((Tilt > 0x40000000))	return 0;	// 算出不能

	if(Tilt < thresh){
		adti = Tilt<<1;
	}else{
		// Tiltのままだと1.0付近が収束しにくいので加法定理を使用する。
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
		// Arit_divArd1() が正しく計算できないので。
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
// 検証関数
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

		// ランダム生成
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

		// ランダム生成
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

	// **** 使用する方をコメント外して下さい。****
//	Arti_AtanTest();	// atan(y/x)	(y/x) = 0～0.999999

	Arti_Atan2Test();	// atan2(y,x)	 y = intmin～Intmax ,x = intmin～Intmax

	printf("---- END ----\n");

	getchar();
}

