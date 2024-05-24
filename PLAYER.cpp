//*******************************************************************
// move player
//*******************************************************************
#include "MYconio.h"
#include "FIELD.h"
#include "CHARACTOR.h"
#include "BULLET.h"
#include "PLAYER.h"
#include "SLOT.h"
#include "START.h"
#include "conioex.h"

//---------------------
// �}�N����`
//---------------------
#define PLAYER_COLOR	YELLOW

#define PLAYER_SPEED_X	0.6f
#define PLAYER_SPEED_Y	0.5f

#define LIFE_INIT	3600 //1 min	

//---------------------
// �O���[�o���ϐ��錾
//---------------------
static charactor_t	Player;
static bool			PlayerEscape;		// ESC�L�[�������ꂽ

static const char* aa[] = {
	"    A    ",
	"   A A   ",
	"[7][7][7]",
	NULL,
};

int Coin;


//===================================================================
// ��������
//===================================================================
void PlayerInit(void)
{
	PlayerEscape = false;
	SlotInit();
	

	CharactorSet(&Player, aa, LIGHTCYAN, LIFE_INIT,
		PLAYER_SPEED_X, PLAYER_SPEED_Y);
	CharactorFirstDraw(&Player, FIELD_WIDTH/2-4, FIELD_HEIGHT-10);	// �ŉ��s�̍��[�ɕ\��������
	LifeDraw(Player.life);// show HP
	//Player.coin = 10;
	CoinDraw(Player.coin);
	InstructionDraw();
	Player.isPay = 0;

	//l = MciOpenSound(path);

}
//===================================================================
// �I������
//===================================================================
void PlayerEnd()
{
	//*coin = p->coin;
	//MciCloseSound(l);
	SlotEnd();
}
//===================================================================
// �X�V����
//===================================================================
void PlayerUpdate(void)
{
	charactor_t* p = &Player;
	//int		key = input_key(); replaced by winapi
	

	if (Player.life==0||input(PK_ESC)) {

		p->live = false;
		PlayerEscape = true;
		//ResultMenu();

	}else if(p->live) {
		if (p->next_X < 0) {
			p->live = false;
			
			MessageDraw("TIMES UP-- GAME OVER -- PLEASE CLICK ESC\n");
		}
		else {
			float lifedecrease = 0.0032;
			Player.life -= lifedecrease;
			int		move_x = 0, move_y = 0;
			int coin_time = 0;

			if (input(PK_UP))		move_y = -1;
			if (input(PK_DOWN))		move_y = 1;
			if (input(PK_LEFT))		move_x = -1;
			if (input(PK_RIGHT))	move_x = 1;
			(void)isCharactorUpdate(&Player, move_x, move_y);//�΂߂ł���
			if (input(PK_SP)) {
				BulletCreate(p->next_X + p->obj.width / 2, p->next_Y - 1);
			
			}
		}
	}
	//FieldDraw(Coin);
	SlotUpdate(&Player);
	
	
	
}
//===================================================================
// �\������
//===================================================================
void PlayerDraw(void)
{
	CharactorDraw(&Player);
	FieldDraw(Player.coin);
	LifeDraw(Player.life);// show HP
	CoinDraw(Player.coin);
	
}
//===================================================================
// escape
//===================================================================
bool isPlayerEscape(void)
{
	return PlayerEscape;
}

int GetPlayerCoin(void) {
	charactor_t* p = &Player;
	return p->coin;
}


//===================================================================
// collision
//===================================================================
void PlayerCollision(charactor_t* target) {

	charactor_t* p = &Player;

	if (p->live && p->next_X >= 0) {
		if (isCharactorCollision(p,target)) {
			LifeDraw(Player.life);
			
		}
	}
}
//===================================================================
// coin get
//===================================================================
void PlayerCoinUp(int enemy_life) 
{
	if (Player.live) {
		Player.coin += enemy_life;
		CoinDraw(Player.coin);
	}
}
//===================================================================
// coin lose
//===================================================================
void PlayerCoinDown(int enemy_life)
{
	if (Player.live) {
		Player.coin -= enemy_life;
	
		CoinDraw(Player.coin);
	}
	
}
//===================================================================
// coin spend
//===================================================================
void PlayerCoinPay()
{
	while (Player.live&& Player.isPay==1) {
		Player.isPay == 0;
		Player.coin -= 50;
		CoinDraw(Player.coin);
		break;
	}

}
//===================================================================
// play sound
//===================================================================
