//*******************************************************************
// enemy
//*******************************************************************
#include "MYconio.h"
#include "FIELD.h"
#include "CHARACTOR.h"
#include "BULLET.h"
#include "PLAYER.h"
#include "ENEMY.h"

//---------------------
// �}�N����`
//---------------------
//#define ENEMY_COLOR	RED

#define ENEMY_SPEED_X		0.3f
#define ENEMY_SPEED_Y		0.3f

#define LIFE_INIT			4  //ENEMY
#define ENEMY_NUM			4
#define INTERVAL_TIME_MIN   1000
#define INTERVAL_TIME_MAX   5500
#define ACCEL				0.0f    

//---------------------
// structure enemy
//---------------------
typedef struct list_s{
	struct list_s* next;
	charactor_t  chara;
}list_t;


//---------------------
// �O���[�o���ϐ��錾
//---------------------
static list_t 		*EnemyTop;          //first of the list of enemy pointer


static const char* aa1[] = {
	"Y",
	"E",
	"A",
	"H",
	NULL,
};

static const char* aa2[] = {
	"C",
	"O",
	"O",
	"L",
	NULL,
};

static const char* aa3[] = {
	"N",
	"I",
	"C",
	"E",
	NULL,
};

static const char* aa4[] = {
	"G",
	"O",
	"O",
	"D",
	NULL,
};

static const char** aa[LIFE_INIT] = { aa1,aa2,aa3,aa4,};
static int color[LIFE_INIT] = { LIGHTMAGENTA,LIGHTGREEN,LIGHTRED, RED };

static DWORD  RecordTime;
static DWORD  IntervalTime;
//===================================================================
// plus enemy+
//===================================================================
charactor_t* EnemyAdd(void)
{
	list_t* work;

	work = (list_t*)malloc(sizeof(list_t));
	if (work == NULL) {
		printf("no memory\n");
		exit(1);
	}
	work->next = EnemyTop;
	EnemyTop = work;

	return &work->chara;
	
}


//===================================================================
// delete enemy
//===================================================================
void EnemyDelete(list_t*pre_p) {

	list_t* work = pre_p->next;
	pre_p->next = work ->next;
	free(work);

}

//===================================================================
// init����
//===================================================================
void EnemyInit(void)
{	
	EnemyTop = NULL;
	
	//memset(Enemy, 0, sizeof(Enemy)); // enemy not live

	RecordTime = 0;
}
//===================================================================
// ��������
//===================================================================
static void EnemyCreate()
{
	DWORD now_time = timeGetTime();

	if (now_time - RecordTime >= IntervalTime) {
		RecordTime = now_time;
		IntervalTime = (rand() % (INTERVAL_TIME_MAX - INTERVAL_TIME_MIN + 1)) + INTERVAL_TIME_MIN;
		
		charactor_t *p = EnemyAdd();
		//p->isUpLayer = rand() %2 -1;
		int life = rand()%LIFE_INIT + 1;
		CharactorSet(p, aa[life-1], color[life-1], life,
			ENEMY_SPEED_X, ENEMY_SPEED_Y);
		int y = (rand() % (30))+2;
		CharactorFirstDraw(p,0,y);	// position�\��������
	}
}
//===================================================================
// �I������
//===================================================================
void EnemyEnd(void)
{
	while (EnemyTop != NULL) {
		list_t* work = EnemyTop;
		EnemyTop = EnemyTop->next;
		free(work);
	}
}
//===================================================================
// �X�V����
//===================================================================
void EnemyUpdate(void)
{
	EnemyCreate();

	list_t* q = (list_t*)&EnemyTop;

	while (q->next != NULL)
	{
		charactor_t* p = &q->next->chara;

		int		move_x = 1, move_y = 0;
		//if (p->next_X > 52) {
			
			//p->next_Y = 0;
			//if (p->next_X = p->obj.width) {
				//p->speed_X *= -1;//EnemyDelete(q); //enemy set to not live
			//}
			//continue;
		//}
		

		if (!isCharactorUpdate(p, move_x, move_y)) {
			p->next_X += p->obj.width;
			if (p->next_X >= 52) {
				p->next_X = -20;
				

			}
			//else {
			//	q->next= NULL;
			//}
		//q->next_X= -1;   //delete 
		}
		q = q->next;
	}
	
}
//===================================================================
// �\������
//===================================================================
void EnemyDraw(void)
{
	for (list_t* q = EnemyTop; q!= NULL; q =q->next) {
		CharactorDraw(&q->chara);
	}
}
//===================================================================
// collision update
//===================================================================
void EnemyCollisionUpdate(void) {

	for (list_t* q = EnemyTop; q != NULL; q = q->next) {
		charactor_t* p = &q->chara;

		if (p->next_X >= 0 ) {
			int life = p->life;

			PlayerCollision(p);
			BulletCollision(p);

			if(p->life !=life && p->life>0){
				p->color = color[p->life - 1];
				p->obj.strings = (char**)aa[p->life - 1];
				PlayerCoinUp(p->life+1);
				//p->next_Y -= 2;        //collision effect
			}
		}
	}
}