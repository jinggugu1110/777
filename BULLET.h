#pragma once
//*******************************************************************
// Bullet h
//*******************************************************************

#include "CHARACTOR.h"

//---------------------
// �֐��v���g�^�C�v�錾
//---------------------
void BulletInit(void);
void BulletCreate(int x, int y);
void BulletEnd(void);
void BulletUpdate(void);
void BulletDraw(void);
void BulletCollision(charactor_t* target);

