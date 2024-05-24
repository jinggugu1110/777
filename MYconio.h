#pragma once
//*******************************************************************
//WindowsAPI���p�̃L�[����
//*******************************************************************
#include <stdio.h>
#include <Windows.h>

#pragma warning(disable:4996)		// _CRT_SECURE_NO_WARNINGS�̑��
#pragma comment(lib, "winmm.lib")	// �����x���ԑ���p�֐��̎捞�ݗp

//---------------------
// �}�N����`
//---------------------
// ��ʐ���n�̃}�N����`
#define gotoxy(x,y)			printf("\033[%d;%dH", (y), (x))
#define textcolor(color)	printf("\033[%dm", ((color)<8?(color)+30:(color)+82))
#define backcolor(color)	printf("\033[%dm", ((color)<8?(color)+40:(color)+92))
#define resetcolor()		printf("\033[%dm", 0)
#define clrscr()			printf("\033[%dm",0),printf("\033[%dJ", 2),gotoxy(1,1)
#define clreol()			printf("\033[%dK", 0)

#define BLACK			0	// ��
#define RED				1	// �Â���
#define GREEN			2	// ��
#define BROWN			3	// ���F
#define BLUE			4	// �Â���
#define MAGENTA			5	// ��
#define CYAN			6	// ��
#define LIGHTGRAY		7	// ���邢�D�F
#define DARKGRAY		8	// �Â��D�F
#define LIGHTRED		9	// ��
#define LIGHTGREEN		10	// ���邢��
#define YELLOW			11	// ��
#define LIGHTBLUE		12	// ��
#define LIGHTMAGENTA	13	// �s���N
#define LIGHTCYAN		14	// ���F
#define WHITE			15	// ��

#define NOCURSOR		'l'	// �J�[�\���̔�\���w��̃^�C�v
#define NORMALCURSOR	'h'	// �J�[�\���̕\���w��̃^�C�v
#define setcursortype(type)	printf("\033[?25%c", (type))	// �J�[�\���̕\��/��\���ݒ�

// �ėp�}�N����`
#define WAIT_ENTER()	while(!input(PK_ENTER))

// �L�[���͂̂��߂̃}�N����`(input�Ή�)
#define PK_SP		VK_SPACE
#define PK_ESC		VK_ESCAPE
#define PK_ENTER	VK_RETURN
#define PK_UP		VK_UP
#define PK_DOWN		VK_DOWN
#define PK_LEFT		VK_LEFT
#define PK_RIGHT	VK_RIGHT
#define PK_NON		0

//---------------------
// �֐��v���g�^�C�v�錾
//---------------------
bool input(int key);
void reinput(void);

int opensound(const char* fname);
void closesound(int soundID);
void playsound(int soundID);
void repeatsound(int soundID);
void stopsound(int soundID);
