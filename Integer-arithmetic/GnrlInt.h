// *********************************************************************************
//   �i�[�e��̂̒�`���s��
//  GnrlInt.h
//
//
// *********************************************************************************


#ifndef GNRL_INT_H
#define GNRL_INT_H

#include <assert.h>


// *********************************************************************************
// �^���v���g�^�C�v
// *********************************************************************************
typedef	char			Sint8;
typedef	unsigned char	Uint8;
typedef	bool			Bool8;		// ���@�ɔp�~�\��
typedef	bool			Bool;
typedef	int				Bool32;
typedef	void*			pVoid;		// ���@�ɔp�~�\��
typedef	void*			Pvoid;		// 32bit-64bit �|�C���^�݊��̈�
typedef short			Sint16;
typedef unsigned short	Uint16;
typedef	int				Sint32;
typedef unsigned int	Uint32;
typedef __int64			Sint64;
typedef unsigned __int64 Uint64;
typedef float			Float32;
typedef double			Float64;

typedef Uint8			Exp8;	// 8bit�w��  0x7F  �I�t�Z�b�g
typedef Uint16			Exp16;	// 16bit�w�� 0x3FF �I�t�Z�b�g

#ifndef TRUE
#define TRUE	0x01
#define FALSE	0x00
#endif

#ifndef NULL
#define NULL	nullptr
#endif


// *********************************************************************************
// �e��}�N��
// *********************************************************************************
#if 1
// �召����}�N�� ��VS��<stdio.h>���C���N���[�h������ɒ�`����ƃG���[�ɂȂ�B
template <typename X> X MAX(X arg1, X arg2)	{ return arg1>arg2 ? arg1 : arg2; }
template <typename X> X MIN(X arg1, X arg2)	{ return arg1<arg2 ? arg1 : arg2; }
#endif



#endif // #ifndef GNRL_INT_H
