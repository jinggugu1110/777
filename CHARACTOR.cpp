//*******************************************************************
// 　・衝突した時、消去及び再表示設定する
//*******************************************************************
#include "MYconio.h"
#include "FIELD.h"
#include "CHARACTOR.h"

// マクロ関数
#define IS_SAME_POSITION(p)	((int)(p)->now_X == (int)(p)->next_X && \
							 (int)(p)->now_Y == (int)(p)->next_Y)

//===================================================================
// CHARACTOR構造体を初期化する
//===================================================================
void CharactorSet(charactor_t* p, const char *aa[], int color, int life, float speed_X, float speed_Y)
{
	StringsSet(&p->obj, aa);

	p->live = true;
	p->color = color;
	p->life = life;

	p->speed_X = speed_X;
	p->speed_Y = speed_Y;

	p->top = 0;
	p->bottom = FIELD_HEIGHT - p->obj.height;
	p->left = 0;
	p->right = FIELD_WIDTH - p->obj.width;
	p->isPay = false;
	p->coin = 1;
}
//===================================================================
// 初期表示する
//===================================================================
void CharactorFirstDraw(charactor_t* p, int X, int Y)
{
	p->now_X = (float)X;
	p->now_Y = (float)Y;

	// 現在位置が枠外の場合、次位置に補正した位置がセットされる
	(void)isCharactorUpdate(p, 0, 0);
	p->now_X = p->next_X;
	p->now_Y = p->next_Y;

	textcolor(p->color);
	StringsDraw((int)p->now_X + FIELD_X, (int)p->now_Y + FIELD_Y, &p->obj);
}
//===================================================================
// 更新処理(フィールド枠内に収める様に移動方向に次位置をセットする)
//===================================================================
bool isCharactorUpdate(charactor_t* p, int move_X, int move_Y)
{
	bool	result = true;
	float	next_X = p->now_X + p->speed_X * move_X;
	float	next_Y = p->now_Y + p->speed_Y * move_Y;

	if (next_X >= p->right + 1) {
		next_X = (float)p->right;
		result = false;
	}
	else if (next_X < p->left) {
		next_X = (float)p->left;
		result = false;
	}
	if (next_Y >= p->bottom + 1) { 
		next_Y = (float)p->bottom;
		result = false;
	}
	else if (next_Y < p->top) { 
		next_Y = (float)p->top;
		result = false;
	}
	p->next_X = next_X;
	p->next_Y = next_Y;
	return result;
}
//===================================================================
// 表示処理(現表示物を消去して次位置に表示し直す)
//===================================================================
void CharactorDraw(charactor_t *p)
{
	if (p->live) {
		if (!IS_SAME_POSITION(p)) {

			if (p->now_X >= 0) {
				StringsErase((int)p->now_X + FIELD_X, (int)p->now_Y + FIELD_Y, &p->obj);
			}
			if (p->next_X >= 0) {
				textcolor(p->color);
				StringsDraw((int)p->next_X + FIELD_X, (int)p->next_Y + FIELD_Y, &p->obj);
			}
		}
		p->now_X = p->next_X;
		p->now_Y = p->next_Y;
	}
}
//===================================================================
// 衝突チェック
//===================================================================
// 内部関数：衝突した場合の消去と再表示(生きている方を一部を消去している為)
static void charactor_erase_draw(charactor_t* p, charactor_t* q)
{
	int		p_life = p->life;	// p側の体力
	int		q_life = q->life;	// q側の体力

	p->life -= 1;	// 衝突したら体力が減少p->life -= q_life;
	q->life -= 1;	// 衝突したら体力がq->life -= p_life;

	if (p->life <= 0 && q->life > 0) {
		 //p側が消滅、q側は生きている
		p->next_X = -1, p->life = 0;				// p側消去
		if (IS_SAME_POSITION(q))	q->now_X = -1;	 	// q側再表示
	}
	else if (p->life > 0 && q->life <= 0) {
		// q側が消滅、p側は生きている
		q->next_X = -1, q->life = 0;				// q側消去
		if (IS_SAME_POSITION(p))	p->now_X = -1; // p側再表示
	}
	else if (p->life <= 0 && q->life <= 0) {
		// p側とq側共に消滅
	p->next_X = q->next_X = -1;	// Draw処理で両方を消去する
	p->life = q->life = 0;
	}
	else {
		 //p側とq側共に生きている
		// 処置無し
	}
}
// 衝突チェック関数本体
bool isCharactorCollision(charactor_t* p, charactor_t* q)
{
	int		px0, px1, py0, py1;		// p側のキャラの四隅位置
	int		qx0, qx1, qy0, qy1;		// q側のキャラの四隅位置

	px0 = (int)p->next_X;
	px1 = (int)p->next_X + p->obj.width - 1;
	py0 = (int)p->next_Y;
	py1 = (int)p->next_Y + p->obj.height - 1;

	qx0 = (int)q->next_X;
	qx1 = (int)q->next_X + q->obj.width - 1;
	qy0 = (int)q->next_Y;
	qy1 = (int)q->next_Y + q->obj.height - 1;

	if (px1 < qx0)	return false;	// 重なっていない
	if (qx1 < px0)	return false;	// 重なっていない
	if (py1 < qy0)	return false;	// 重なっていない
	if (qy1 < py0)	return false;	// 重なっていない

	charactor_erase_draw(p, q);		// 体力消費し、体力が無くなったら消去設定

	return true;	// 重なっている(衝突した)
}

