#include "MYconio.h"
#include "FIELD.h"
#include "PLAYER.h"
#include "BULLET.h"
#include "ENEMY.h"
#include <time.h>
#include "SLOT.h"
#include <windows.h>
#include <locale.h>
#include "main.h"
#include "conioex.h"
#include "START.h"

#define CRT_SECURE_NO_WARNINGS

//FPS 制御用、表示用変数
DWORD RecordTime;
DWORD DispTime, DispCnt;

static void Init(void);
static void Update(void);
static void End(void);
static void Draw(void);
const char* titlename = { "LUCKY FLIGHT 777" };


int main()
{
	
	SetCaption(titlename);
	setlocale(LC_ALL, "jpn");//for Japanese words display
	SystemInit();
	//HWND consoleWindow = GetConsoleWindow();
	//HANDLE stdOutputWindow = GetStdHandle(STD_OUTPUT_HANDLE);
	//RECT rect;
	
	// 设置控制台窗口的尺寸 WINAPI
	
	//GetWindowRect(consoleWindow, &rect);
	//MoveWindow(consoleWindow, rect.left, rect.top, 1024, 1200, TRUE);
	//SMALL_RECT smallRect;
	//Windows API but not useful for the project

	//smallRect.Top = rect.top;
	//smallRect.Bottom = rect.bottom;
	//smallRect.Right = rect.right;
	//smallRect.Left = rect.left;

	//COORD coord;
	//coord.X = smallRect.Right - smallRect.Left + 1;
	//coord.Y = smallRect.Bottom - smallRect.Top + 1;

	//smallRect.Top = 100;
	//smallRect.Bottom = 200;
	//smallRect.Right = 200;
	//smallRect.Left = 100;

	//auto b = SetConsoleScreenBufferSize(stdOutputWindow, coord);
	//auto a = SetConsoleWindowInfo(stdOutputWindow, TRUE, &smallRect);
	//auto e = GetLastError();

	// fullscreen but not effective
	//auto style = GetWindowLong(consoleWindow, GWL_STYLE) | WS_MAXIMIZE;
	////auto style = 0x14ff0000;
	//SetWindowLong(consoleWindow, GWL_STYLE, style);
	//ShowWindow(consoleWindow, SW_SHOW);;
	StartMenu();
	
	Init();
		
	while(!isPlayerEscape())
	{
		DWORD now_time = timeGetTime();

		if (now_time - RecordTime >= 1000 / 60)
		{
			RecordTime = now_time;

			if (now_time - DispTime >= 500) {
				textcolor(WHITE), gotoxy(70, 1);
				printf("FPS:%5.1lf",								//double cast
					((double)DispCnt * 1000) / (now_time - DispTime));
				DispTime = now_time;
				DispCnt = 0;
			}
			DispCnt++;
			
			Update();
			Draw();
		}
	}
	
	ResultMenu();
	End();
	SystemUninit();
	return 0;
}

//INIT
static void Init(void) 
{
	system("cls");
	setcursortype(NOCURSOR); //カーサルを非表示
	srand((unsigned int)time(NULL));

	timeBeginPeriod(1);		//1ms

	RecordTime = 0;
	DispTime = timeGetTime();
	DispCnt = 0;

	
	SlotBoxDraw();
	PlayerInit();
	BulletInit();
	EnemyInit();
	//SlotInit();
}

//END
static void End(void) 
{
	PlayerEnd();
	BulletEnd();
	EnemyEnd();
	MessageDraw("GAME OVER");
	WAIT_ENTER();
	setcursortype(NORMALCURSOR); //カーサルを表示にする
}

//UPDATE
static void Update(void)
{
	//FieldDraw(Coin);
	PlayerUpdate();
	BulletUpdate();
	EnemyUpdate();
	EnemyCollisionUpdate();
	
	//SlotUpdate(Coin);
	
}

static void Draw(void) 
{
	BulletDraw();
	PlayerDraw();
	EnemyDraw();
	SlotNoDraw();
	
}

void SystemInit() {
	// conioexの初期化
	InitConio(SCREEN_SIZE_X, SCREEN_SIZE_Y);
	// フォントサイズを設定
	SetScreenFontSize(8, 16);
	// カーソルの原点を(0, 0)として設定
	SetCursorOriginZeroFlag(true);
	// カーソルを非表示
	SetCursorType(NOCURSOR);
	// フレームレートの設定
	//SetFrameRate(60);
	// ウィンドウの表示名を設定
	//SetCaption("サンプル: Image");
	// ダブルバッファを初期化
	InitDoubleBuffer();
	// 入力保持処理の初期化
	//InitInput();
}
void SystemUninit() {
	// conioexの終了処理
	UninitConio();
	// 入力保持処理の終了処理
	//UninitInput();
}
