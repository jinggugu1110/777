#pragma once
//*******************************************************************
// field h
//*******************************************************************

//---------------------
// マクロ定義
//---------------------
#define FIELD_X			5			// フィールドのX座標
#define FIELD_Y			3			// フィールドのY座標
#define FIELD_WIDTH		60			// フィールドの幅
#define FIELD_HEIGHT	40			// フィールドの高さ
#define SLOT_X          85
#define SLOT_Y          36
#define SLOT_WIDTH      5
#define SLOT_HEIGHT     2

//---------------------
// 関数プロトタイプ宣言
//---------------------
void FieldDraw(int Coin);
void MessageDraw(const char* format, ...);
void LifeDraw(int life);
void InstructionDraw();
void SlotBoxDraw();
void CoinDraw(int Coin);
int coinresultf(int Coin);