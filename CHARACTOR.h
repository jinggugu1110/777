#pragma once
//*******************************************************************
//move charactor
//*******************************************************************
#include "STRINGS.h"

//---------------------
// 構造体タグ宣言
//---------------------
typedef struct {
	bool		live;		// true:生きている,false:死亡
	strings_t	obj;		// 表示する物
	int			color;		// 表示する物の色
	int			life;		// 表示する物の体力
	float		now_X;		// 現表示位置
	float		now_Y;
	float		next_X;		// 次表示位置
	float		next_Y;
	float		speed_X;	// 移動スピード
	float		speed_Y;
	int			top;		// 移動可能な上下左右位置
	int			bottom;
	int			left;
	int			right;
	bool        isPay;   //false: not pay true:pay 50
	int         coin;
}charactor_t;

//---------------------
// 関数プロトタイプ宣言
//---------------------
void CharactorSet(charactor_t* p, const char* aa[], int color, int life, float speed_X, float speed_Y);
void CharactorFirstDraw(charactor_t* p, int X, int Y);
bool isCharactorUpdate(charactor_t* p, int move_X, int move_Y);
void CharactorDraw(charactor_t* p);
bool isCharactorCollision(charactor_t* p, charactor_t* q);
int coinresult(charactor_t p);
