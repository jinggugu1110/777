//*******************************************************************
// move bullet
//*******************************************************************
#include "MYconio.h"
#include "COMMON.h"
#include "FIELD.h"
#include "CHARACTOR.h"
#include "BULLET.h"
#include "conioex.h"

//---------------------
// マクロ定義
//---------------------
#define BULLET_COLOR	BROWN

#define BULLET_SPEED_X	0
#define BULLET_SPEED_Y	2.0f

#define LIFE_INIT		1	 //bullet hp
#define BULLET_LIMIT_Y  10    //max number

//---------------------
// グローバル変数宣言
//---------------------
static charactor_t	Bullet;
static bool			PlayerEscape;		// ESCキーが押された

static const char* aa[] = {
	"($)",
	NULL,
};

const char* pathshoot = { "shoot.mp3", };
int* shoot = NULL;
//static int sound_id;

//===================================================================
// init処理
//===================================================================
void BulletInit(void)
{
	//const char* fname = "money.mp3";
	//sound_id = opensound(fname);					//SE file add
	//if (sound_id == 0)
	//{
	//	MessageDraw("%snot found", fname);
	//	WAIT_ENTER();
	//	MessageDraw("");
	//}
	//shoot = MciOpenSound(pathshoot);
	memset(&Bullet, 0, sizeof(Bullet)); // bullet not live
}
//===================================================================
// 生成処理
//===================================================================
void BulletCreate(int x, int y)
{
	if (!Bullet.live && y >= BULLET_LIMIT_Y) {

		//playsound(sound_id);
		//MciPlaySound(shoot, 0);
		CharactorSet(&Bullet, aa, BULLET_COLOR, LIFE_INIT,
			BULLET_SPEED_X, BULLET_SPEED_Y);
		CharactorFirstDraw(&Bullet, x-1, y);	// position表示させる
	}
}
//===================================================================
// 終了処理
//===================================================================
void BulletEnd(void)
{
	//closesound(sound_id);
	//MciCloseSound(shoot);
}
//===================================================================
// 更新処理
//===================================================================
void BulletUpdate(void)
{
	charactor_t* p = &Bullet;
	//int		key = input_key(); replaced by winapi

	if(p->live) {
		int		move_x = 0, move_y = -1;
		if (p->next_X < 0) {
			p->live = false; //bullet set to not live
		}
		else if (!isCharactorUpdate(p, move_x, move_y)) {
			p->next_X = -1;   //delete 
		}
		//
	}
}
//===================================================================
// 表示処理
//===================================================================
void BulletDraw(void)
{
	CharactorDraw(&Bullet);
}
//===================================================================
// collision
//===================================================================
void BulletCollision(charactor_t* target) {

	charactor_t* p = &Bullet;

	if (p->live && p->next_X >= 0) {
		(void)isCharactorCollision(target, p);
		
	}
}