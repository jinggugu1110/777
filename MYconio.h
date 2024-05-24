#pragma once
//*******************************************************************
//WindowsAPI利用のキー入力
//*******************************************************************
#include <stdio.h>
#include <Windows.h>

#pragma warning(disable:4996)		// _CRT_SECURE_NO_WARNINGSの代替
#pragma comment(lib, "winmm.lib")	// 高精度時間測定用関数の取込み用

//---------------------
// マクロ定義
//---------------------
// 画面制御系のマクロ定義
#define gotoxy(x,y)			printf("\033[%d;%dH", (y), (x))
#define textcolor(color)	printf("\033[%dm", ((color)<8?(color)+30:(color)+82))
#define backcolor(color)	printf("\033[%dm", ((color)<8?(color)+40:(color)+92))
#define resetcolor()		printf("\033[%dm", 0)
#define clrscr()			printf("\033[%dm",0),printf("\033[%dJ", 2),gotoxy(1,1)
#define clreol()			printf("\033[%dK", 0)

#define BLACK			0	// 黒
#define RED				1	// 暗い赤
#define GREEN			2	// 緑
#define BROWN			3	// 茶色
#define BLUE			4	// 暗い青
#define MAGENTA			5	// 紫
#define CYAN			6	// 青緑
#define LIGHTGRAY		7	// 明るい灰色
#define DARKGRAY		8	// 暗い灰色
#define LIGHTRED		9	// 赤
#define LIGHTGREEN		10	// 明るい緑
#define YELLOW			11	// 黄
#define LIGHTBLUE		12	// 青
#define LIGHTMAGENTA	13	// ピンク
#define LIGHTCYAN		14	// 水色
#define WHITE			15	// 白

#define NOCURSOR		'l'	// カーソルの非表示指定のタイプ
#define NORMALCURSOR	'h'	// カーソルの表示指定のタイプ
#define setcursortype(type)	printf("\033[?25%c", (type))	// カーソルの表示/非表示設定

// 汎用マクロ定義
#define WAIT_ENTER()	while(!input(PK_ENTER))

// キー入力のためのマクロ定義(input対応)
#define PK_SP		VK_SPACE
#define PK_ESC		VK_ESCAPE
#define PK_ENTER	VK_RETURN
#define PK_UP		VK_UP
#define PK_DOWN		VK_DOWN
#define PK_LEFT		VK_LEFT
#define PK_RIGHT	VK_RIGHT
#define PK_NON		0

//---------------------
// 関数プロトタイプ宣言
//---------------------
bool input(int key);
void reinput(void);

int opensound(const char* fname);
void closesound(int soundID);
void playsound(int soundID);
void repeatsound(int soundID);
void stopsound(int soundID);
