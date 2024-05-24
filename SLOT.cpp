//*******************************************************************
// SLOT 
//*******************************************************************
#include "MYconio.h"
#include "COMMON.h"
#include "FIELD.h"
#include "CHARACTOR.h"
#include "BULLET.h"
#include "SLOT.h"
#include "conioex.h"

//---------------------
// マクロ定義
//---------------------

#define backcolor WHITE

//---------------------
// グローバル変数宣言
//---------------------
static charactor_t	Slot;
int numbers[3] = { 1, 2, 3 };
int stop_flag[3] = { 0, 0, 0 }; /* 0:回転中 1:停止中 */
int i;
bool c = false;

const char* pathwinning = { "winning.mp3", };
int* winning = NULL;
const char* pathhint = { "hint.mp3", };
int* hint = NULL;
const char* pathhint2 = { "hint2.mp3", };
int* hint2 = NULL;

//static int sound_id;

//===================================================================
// init処理
//===================================================================
void SlotInit(void)
{
	//const char* fname = "money.mp3";
	//sound_id = opensound(fname);					//SE file add
	//if (sound_id == 0)
	//{
	//	MessageDraw("%snot found", fname);
	//	WAIT_ENTER();
	//	MessageDraw("");
	//}
	winning = MciOpenSound(pathwinning);
	//hint = MciOpenSound(pathhint);
	//hint2 = MciOpenSound(pathhint2);
}

//===================================================================
// 生成処理
//===================================================================
void SlotCreate()
{
	
}
//===================================================================
// 終了処理
//===================================================================
void SlotEnd(void)
{
	MciCloseSound(winning);
	//MciCloseSound(hint);
	//MciCloseSound(hint2);
}
//===================================================================
// 更新処理
//===================================================================
void SlotUpdate(charactor_t* p)
{

	//cs = &CoinS;
	/*/数字処理 */
	for (i = 0; i < 3; i++) {
		if (stop_flag[i] == 0) {
			//数字更新 */
			numbers[i]++;
			if (numbers[i] > 9) {
				numbers[i] = 1;
			}
		}

	}
	if (input(VK_NUMPAD0) || input(VK_F5)) {
		MciPlaySound(hint2, 0);
		stop_flag[0] = 0;
		stop_flag[1] = 0;
		stop_flag[2] = 0;
		c = false;
		gotoxy(SLOT_X - 3, SLOT_Y + 8);
		printf("                                         ");

	}
	if (p->coin >= 15) {
		if (input(VK_NUMPAD1)||input(VK_F2)) {
			stop_flag[0] = 1;
			MciPlaySound(hint, 0);
		}
		else if (input(VK_NUMPAD2) || input(VK_F3)) {
			stop_flag[1] = 1;
			MciPlaySound(hint, 0);
			if (numbers[0] == 7) { numbers[1] = 7; }//SLOT の確率を1/9に高くーー楽しく
		}
		else if (input(VK_NUMPAD3) || input(VK_F4)) {
			stop_flag[2] = 1;
			MciPlaySound(hint, 0);
			if (numbers[1] == 7) { numbers[2] = 7; }//SLOT の確率を1/9に高くーー楽しく
			
		}
		
		//else if
		
	}
	if (stop_flag[0] == 1 && stop_flag[1] == 1 && stop_flag[2] == 1 && c==false) {	/* 全部停止 */
		p->coin-= 15;
		c = true;
		if (numbers[0] == numbers[1] && numbers[1] == numbers[2]) {		/* 全部の数字が同じ */
			MciPlaySound(winning, 0);
			gotoxy(SLOT_X - 3, SLOT_Y + 8);
			
			printf("CONGRATULATIONS!                         ");
			p->coin += 500;
			
		}
		else {
			gotoxy(SLOT_X - 3, SLOT_Y + 8);
			
			printf("PITY! PLEASE PRESS NUM 0 TO RESET SLOT");
			
		}
	}
}
//===================================================================
// 表示処理
//===================================================================
void SlotNoDraw()
{
	/* 数字表示 */
	
	textcolor(RED);
	gotoxy(SLOT_X + 1, SLOT_Y);
	printf("%d", numbers[0]);
	gotoxy(SLOT_X + 9, SLOT_Y);
	printf("%d", numbers[1]);
	gotoxy(SLOT_X + 17, SLOT_Y);
	printf("%d", numbers[2]);
	
}

