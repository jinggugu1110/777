//*******************************************************************
// �@�E�Փ˂������A�����y�эĕ\���ݒ肷��
//*******************************************************************
#include "MYconio.h"
#include "FIELD.h"
#include "CHARACTOR.h"

// �}�N���֐�
#define IS_SAME_POSITION(p)	((int)(p)->now_X == (int)(p)->next_X && \
							 (int)(p)->now_Y == (int)(p)->next_Y)

//===================================================================
// CHARACTOR�\���̂�����������
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
// �����\������
//===================================================================
void CharactorFirstDraw(charactor_t* p, int X, int Y)
{
	p->now_X = (float)X;
	p->now_Y = (float)Y;

	// ���݈ʒu���g�O�̏ꍇ�A���ʒu�ɕ␳�����ʒu���Z�b�g�����
	(void)isCharactorUpdate(p, 0, 0);
	p->now_X = p->next_X;
	p->now_Y = p->next_Y;

	textcolor(p->color);
	StringsDraw((int)p->now_X + FIELD_X, (int)p->now_Y + FIELD_Y, &p->obj);
}
//===================================================================
// �X�V����(�t�B�[���h�g���Ɏ��߂�l�Ɉړ������Ɏ��ʒu���Z�b�g����)
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
// �\������(���\�������������Ď��ʒu�ɕ\��������)
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
// �Փ˃`�F�b�N
//===================================================================
// �����֐��F�Փ˂����ꍇ�̏����ƍĕ\��(�����Ă�������ꕔ���������Ă����)
static void charactor_erase_draw(charactor_t* p, charactor_t* q)
{
	int		p_life = p->life;	// p���̗̑�
	int		q_life = q->life;	// q���̗̑�

	p->life -= 1;	// �Փ˂�����̗͂�����p->life -= q_life;
	q->life -= 1;	// �Փ˂�����̗͂�q->life -= p_life;

	if (p->life <= 0 && q->life > 0) {
		 //p�������ŁAq���͐����Ă���
		p->next_X = -1, p->life = 0;				// p������
		if (IS_SAME_POSITION(q))	q->now_X = -1;	 	// q���ĕ\��
	}
	else if (p->life > 0 && q->life <= 0) {
		// q�������ŁAp���͐����Ă���
		q->next_X = -1, q->life = 0;				// q������
		if (IS_SAME_POSITION(p))	p->now_X = -1; // p���ĕ\��
	}
	else if (p->life <= 0 && q->life <= 0) {
		// p����q�����ɏ���
	p->next_X = q->next_X = -1;	// Draw�����ŗ�������������
	p->life = q->life = 0;
	}
	else {
		 //p����q�����ɐ����Ă���
		// ���u����
	}
}
// �Փ˃`�F�b�N�֐��{��
bool isCharactorCollision(charactor_t* p, charactor_t* q)
{
	int		px0, px1, py0, py1;		// p���̃L�����̎l���ʒu
	int		qx0, qx1, qy0, qy1;		// q���̃L�����̎l���ʒu

	px0 = (int)p->next_X;
	px1 = (int)p->next_X + p->obj.width - 1;
	py0 = (int)p->next_Y;
	py1 = (int)p->next_Y + p->obj.height - 1;

	qx0 = (int)q->next_X;
	qx1 = (int)q->next_X + q->obj.width - 1;
	qy0 = (int)q->next_Y;
	qy1 = (int)q->next_Y + q->obj.height - 1;

	if (px1 < qx0)	return false;	// �d�Ȃ��Ă��Ȃ�
	if (qx1 < px0)	return false;	// �d�Ȃ��Ă��Ȃ�
	if (py1 < qy0)	return false;	// �d�Ȃ��Ă��Ȃ�
	if (qy1 < py0)	return false;	// �d�Ȃ��Ă��Ȃ�

	charactor_erase_draw(p, q);		// �̗͏���A�̗͂������Ȃ���������ݒ�

	return true;	// �d�Ȃ��Ă���(�Փ˂���)
}

