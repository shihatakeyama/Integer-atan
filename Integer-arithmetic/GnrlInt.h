// *********************************************************************************
//   格納容器のの定義を行う
//  GnrlInt.h
//
//
// *********************************************************************************


#ifndef GNRL_INT_H
#define GNRL_INT_H

#include <assert.h>


// *********************************************************************************
// 型をプロトタイプ
// *********************************************************************************
typedef	char			Sint8;
typedef	unsigned char	Uint8;
typedef	bool			Bool8;		// 時機に廃止予定
typedef	bool			Bool;
typedef	int				Bool32;
typedef	void*			pVoid;		// 時機に廃止予定
typedef	void*			Pvoid;		// 32bit-64bit ポインタ互換の為
typedef short			Sint16;
typedef unsigned short	Uint16;
typedef	int				Sint32;
typedef unsigned int	Uint32;
typedef __int64			Sint64;
typedef unsigned __int64 Uint64;
typedef float			Float32;
typedef double			Float64;

typedef Uint8			Exp8;	// 8bit指数  0x7F  オフセット
typedef Uint16			Exp16;	// 16bit指数 0x3FF オフセット

#ifndef TRUE
#define TRUE	0x01
#define FALSE	0x00
#endif

#ifndef NULL
#define NULL	nullptr
#endif


// *********************************************************************************
// 各種マクロ
// *********************************************************************************
#if 1
// 大小判定マクロ ※VSで<stdio.h>をインクルードした後に定義するとエラーになる。
template <typename X> X MAX(X arg1, X arg2)	{ return arg1>arg2 ? arg1 : arg2; }
template <typename X> X MIN(X arg1, X arg2)	{ return arg1<arg2 ? arg1 : arg2; }
#endif



#endif // #ifndef GNRL_INT_H
