#pragma once
//*******************************************************************
// field h
//*******************************************************************

//---------------------
// �}�N����`
//---------------------
#define FIELD_X			5			// �t�B�[���h��X���W
#define FIELD_Y			3			// �t�B�[���h��Y���W
#define FIELD_WIDTH		60			// �t�B�[���h�̕�
#define FIELD_HEIGHT	40			// �t�B�[���h�̍���
#define SLOT_X          85
#define SLOT_Y          36
#define SLOT_WIDTH      5
#define SLOT_HEIGHT     2

//---------------------
// �֐��v���g�^�C�v�錾
//---------------------
void FieldDraw(int Coin);
void MessageDraw(const char* format, ...);
void LifeDraw(int life);
void InstructionDraw();
void SlotBoxDraw();
void CoinDraw(int Coin);
int coinresultf(int Coin);