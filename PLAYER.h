#pragma once
//*******************************************************************
// move player h
//*******************************************************************
#include "CHARACTOR.h"


//---------------------
// �֐��v���g�^�C�v�錾
//---------------------
void PlayerInit(void);
void PlayerEnd(void);
void PlayerUpdate(void);
void PlayerDraw(void);
bool isPlayerEscape(void);
int GetPlayerCoin(void);
void PlayerCollision(charactor_t* target);
void PlayerCoinUp(int enemy_life);
void PlayerCoinDown(int enemy_life);
void PlayerCoinPay();
void SlotInit();
void SlotEnd();



