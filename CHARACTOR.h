#pragma once
//*******************************************************************
//move charactor
//*******************************************************************
#include "STRINGS.h"

//---------------------
// �\���̃^�O�錾
//---------------------
typedef struct {
	bool		live;		// true:�����Ă���,false:���S
	strings_t	obj;		// �\�����镨
	int			color;		// �\�����镨�̐F
	int			life;		// �\�����镨�̗̑�
	float		now_X;		// ���\���ʒu
	float		now_Y;
	float		next_X;		// ���\���ʒu
	float		next_Y;
	float		speed_X;	// �ړ��X�s�[�h
	float		speed_Y;
	int			top;		// �ړ��\�ȏ㉺���E�ʒu
	int			bottom;
	int			left;
	int			right;
	bool        isPay;   //false: not pay true:pay 50
	int         coin;
}charactor_t;

//---------------------
// �֐��v���g�^�C�v�錾
//---------------------
void CharactorSet(charactor_t* p, const char* aa[], int color, int life, float speed_X, float speed_Y);
void CharactorFirstDraw(charactor_t* p, int X, int Y);
bool isCharactorUpdate(charactor_t* p, int move_X, int move_Y);
void CharactorDraw(charactor_t* p);
bool isCharactorCollision(charactor_t* p, charactor_t* q);
int coinresult(charactor_t p);
