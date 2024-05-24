/**
* @file	conioex.h
* @brief	Win32 コンソール I/O 拡張 for Visual C++/Borland C++
*
* @date	2023/11 2023年度版
* @version	1.02
* @note
*  コンソールウィンドウのプロパティの設定を以下に変更すること
*  ・「従来のコンソールを使う」のチェックを外す
*  ・「簡易編集モード」のチェックを外す
* 
* コンソール関数について
* https://learn.microsoft.com/ja-jp/windows/console/console-functions
*/

#ifndef __CONIOEX_H
#define __CONIOEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <process.h>
#include <windows.h>
#include <tchar.h>
#include <mmsystem.h>
#include <digitalv.h>
#include <xinput.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "xinput.lib")

//#pragma warning(disable:4996)

#define NOCURSOR		0
#define SOLIDCURSOR		1
#define NORMALCURSOR	2

#define DEF_FONTSIZE_X		8
#define DEF_FONTSIZE_Y		16

#define CONSOLE_WINDOW_WIDTH_MIN	60	// 60未満だとスクリーンバッファの作成に失敗する
#define CONSOLE_WINDOW_HEIGHT_MIN	1

// https://en.wikipedia.org/wiki/Color_Graphics_Adapter

/* キーコード */
#define PK_ENTER				VK_RETURN
#define PK_ESC					VK_ESCAPE
#define PK_F1					VK_F1
#define PK_F2					VK_F2
#define PK_F3					VK_F3
#define PK_F4					VK_F4
#define PK_F5					VK_F5
#define PK_F6					VK_F6
#define PK_F7					VK_F7
#define PK_F8					VK_F8
#define PK_F9					VK_F9
#define PK_F10					VK_F10
#define PK_F11					VK_F11
#define PK_F12					VK_F12
#define PK_SP					VK_SPACE
#define PK_UP					VK_UP
#define PK_DOWN					VK_DOWN
#define PK_RIGHT				VK_RIGHT
#define PK_LEFT					VK_LEFT
#define PK_SHIFT				VK_SHIFT
#define PK_CTRL					VK_CONTROL
#define PK_ALT					VK_MENU
#define PK_BS					VK_BACK
#define PK_PAUSE				VK_PAUSE
#define PK_INS					VK_INSERT
#define PK_DEL					VK_DELETE
#define PK_TAB					VK_TAB
#define PK_NFER					VK_KANA		/* [無変換]	*/
#define PK_XFER					VK_CONVERT	/* [変換]	*/
#define PK_0					0x30		/* '0' */
#define PK_1					0x31		/* '1' */
#define PK_2					0x32		/* '2' */
#define PK_3					0x33		/* '3' */
#define PK_4					0x34		/* '4' */
#define PK_5					0x35		/* '5' */
#define PK_6					0x36		/* '6' */
#define PK_7					0x37		/* '7' */
#define PK_8					0x38		/* '8' */
#define PK_9					0x39		/* '9' */
#define PK_NUMPAD0				VK_NUMPAD0	/* テンキーの[0]	*/
#define PK_NUMPAD1				VK_NUMPAD1	/* テンキーの[1]	*/
#define PK_NUMPAD2				VK_NUMPAD2	/* テンキーの[2]	*/
#define PK_NUMPAD3				VK_NUMPAD3	/* テンキーの[3]	*/
#define PK_NUMPAD4				VK_NUMPAD4	/* テンキーの[4]	*/
#define PK_NUMPAD5				VK_NUMPAD5	/* テンキーの[5]	*/
#define PK_NUMPAD6				VK_NUMPAD6	/* テンキーの[6]	*/
#define PK_NUMPAD7				VK_NUMPAD7	/* テンキーの[7]	*/
#define PK_NUMPAD8				VK_NUMPAD8	/* テンキーの[8]	*/
#define PK_NUMPAD9				VK_NUMPAD9	/* テンキーの[9]	*/
#define PK_A					0x41		/* 'A' */
#define PK_B					0x42		/* 'B' */
#define PK_C					0x43		/* 'C' */
#define PK_D					0x44		/* 'D' */
#define PK_E					0x45		/* 'E' */
#define PK_F					0x46		/* 'F' */
#define PK_G					0x47		/* 'G' */
#define PK_H					0x48		/* 'H' */
#define PK_I					0x49		/* 'I' */
#define PK_J					0x4A		/* 'J' */
#define PK_K					0x4B		/* 'K' */
#define PK_L					0x4C		/* 'L' */
#define PK_M					0x4D		/* 'M' */
#define PK_N					0x4E		/* 'N' */
#define PK_O					0x4F		/* 'O' */
#define PK_P					0x50		/* 'P' */
#define PK_Q					0x51		/* 'Q' */
#define PK_R					0x52		/* 'R' */
#define PK_S					0x53		/* 'S' */
#define PK_T					0x54		/* 'T' */
#define PK_U					0x55		/* 'U' */
#define PK_V					0x56		/* 'V' */
#define PK_W					0x57		/* 'W' */
#define PK_X					0x58		/* 'X' */
#define PK_Y					0x59		/* 'Y' */
#define PK_Z					0x5A		/* 'Z' */

#define PK_LT					0x0BC		/* [,]	*/
#define PK_GT					0x0BE		/* [.]	*/
#define PK_SLUSH				0x0BF		/* [?]	*/
#define PK_DOT					VK_DECIMAL	/* テンキーの[.]	*/
#define PK_DIV					VK_DIVIDE	/* テンキーの[/]	*/
#define PK_BSLUSH				0x0E2		/* [_]	*/

#define PK_SEMICOLON			0x0BB		/* [;]	*/
#define PK_ADD					VK_ADD		/* テンキーの[+]	*/
#define PK_COLON				0x0BA		/* [:]	*/
#define PK_MUL					VK_MULTIPLY	/* テンキーの[*]	*/
#define PK_RBRACE				0x0DD		/* []]	*/

#define PK_ATMARK				0x0C0		/* [@]	*/
#define PK_LBRACE				0x0DB		/* [[]	*/

#define PK_MINUS				0x0BD		/* [-]	*/
#define PK_SUB					VK_SUBTRACT	/* テンキーの[-]	*/
#define PK_XOR					0x0DE		/* [^]	*/
#define PK_YEN					0x0DC		/* [\]	*/

#define PK_KANJI				0x0F3		/* [半角/全角]	*/
#define PK_CAPS					0x0F0		/* [英数][ひらがな]	*/

#define PM_LEFT					VK_LBUTTON
#define PM_MID					VK_MBUTTON
#define PM_RIGHT				VK_RBUTTON
#define PM_CURX					0x0101
#define PM_CURY					0x0102

#define PJ1_XPOS				0x0200
#define PJ1_YPOS				0x0201
#define PJ1_ZPOS				0x0202
#define PJ1_BTNS				0x0203
#define PJ2_XPOS				0x0210
#define PJ2_YPOS				0x0211
#define PJ2_ZPOS				0x0212
#define PJ2_BTNS				0x0213
#define PJ3_XPOS				0x0220
#define PJ3_YPOS				0x0221
#define PJ3_ZPOS				0x0222
#define PJ3_BTNS				0x0223
#define PJ4_XPOS				0x0230
#define PJ4_YPOS				0x0231
#define PJ4_ZPOS				0x0232
#define PJ4_BTNS				0x0233
#define PJ_XPOS					PJ1_XPOS
#define PJ_YPOS					PJ1_YPOS
#define PJ_ZPOS					PJ1_ZPOS
#define PJ_BTNS					PJ1_BTNS

#define PJX1_LXPOS				0x0200
#define PJX1_LYPOS				0x0201
#define PJX1_LTRG				0x0202
#define PJX1_RXPOS				0x0203
#define PJX1_RYPOS				0x0204
#define PJX1_RTRG				0x0205
#define PJX1_BTNS				0x0206
#define PJX2_LXPOS				0x0210
#define PJX2_LYPOS				0x0211
#define PJX2_LTRG				0x0212
#define PJX2_RXPOS				0x0213
#define PJX2_RYPOS				0x0214
#define PJX2_RTRG				0x0215
#define PJX2_BTNS				0x0216
#define PJX3_LXPOS				0x0220
#define PJX3_LYPOS				0x0221
#define PJX3_LTRG				0x0222
#define PJX3_RXPOS				0x0223
#define PJX3_RYPOS				0x0224
#define PJX3_RTRG				0x0225
#define PJX3_BTNS				0x0226
#define PJX4_LXPOS				0x0230
#define PJX4_LYPOS				0x0231
#define PJX4_LTRG				0x0232
#define PJX4_RXPOS				0x0233
#define PJX4_RYPOS				0x0234
#define PJX4_RTRG				0x0235
#define PJX4_BTNS				0x0236
#define PJX_LXPOS				PJX1_LXPOS
#define PJX_LYPOS				PJX1_LYPOS
#define PJX_LTRG				PJX1_LTRG
#define PJX_RXPOS				PJX1_RXPOS
#define PJX_RYPOS				PJX1_RYPOS
#define PJX_RTRG				PJX1_RTRG
#define PJX_BTNS				PJX1_BTNS

// 初期化
COORD InitConio(int width, int height);			// 初期化
void UninitConio();								// 終了処理
int InitDoubleBuffer(void);						// ダブルバッファ初期化
void SetScreenFontSize(int width, int height);	// フォントサイズ設定

// 描画
int GetCursorX(void);							// 水平方向のカーソル位置を取得
int GetCursorY(void);							// 垂直方向のカーソル位置を取得
void SetCursorPosition(int x, int y);			// カーソル位置の移動
int GetScreenSizeX(void);						// 水平方向のスクリーンサイズを取得
int GetScreenSizeY(void);						// 垂直方向のスクリーンサイズを取得
void SetCursorOriginZeroFlag(bool flag);		// カーソル位置の原点を設定
#ifdef UNICODE
///#define	PrintString	PrintStringW
void PrintString(const char* _srcbuf, int _size);	// 文字列の出力(マルチバイト⇒Unicode変換版)
#else
#define	PrintString	PrintStringA
#endif // UNICODE
void PrintStringW(const wchar_t* _srcbuf, int _size);	// 文字列の出力（Unicode文字用）
void PrintStringA(const char* _srcbuf, int _size);	// 文字列の出力（マルチバイト文字用）
void ClearLine(void);							// 行末まで消去
void ClearScreen(void);							// 画面消去
void FlipScreen(void);							// ダブルバッファ時の描画面切替
void SetHighVideoColor(void);					// 文字色高輝度化
void SetLowVideoColor(void);					// 文字色低輝度化
void SetNormalVideoColor(void);					// 既定文字色設定
void SetTextBackColor(int color);				// 文字背景色設定
void SetConsoleTextColor(int color);			// 文字色設定
void SetTextAttribute(int attribute);			// 文字色背景色同時設定
void SetCursorType(int type);					// カーソルタイプ設定
void InsertLine(void);							// 現在行に挿入
void DeleteLine(void);							// 現在行の削除
void PrintFrameBuffer(char* buf);				// 文字列の一括転送
void ClearFrameBuffer(char* buf);				// 一括転送用バッファクリア

// ウィンドウ
void SetCaption(const char* title);					// コンソールウィンドウのタイトルバーに表示されるテキストを設定
int GetCaption(char* title, int len);			// コンソールウィンドウのタイトルバーに表示されるテキストを取得

// 入力系
void ResetKeyMap(void);							// キー情報リセット
int InputKeyMouse(int port);					// キーボード・マウス入力
int InputJoystick(int port);					// ゲームパッド入力(DirectInput対応)
int InputJoystickX(int port);					// ゲームパッド入力(XInput対応)

// 音声出力
int* MciOpenSound(const char* path);			// サウンドファイルを開く
void MciCloseSound(int* sound_id);				// サウンドファイルを閉じる
void MciPlaySound(int* sound_id, int repeat);	// サウンドを再生する
void MciStopSound(int* sound_id);				// サウンド再生を停止する
int MciCheckSound(int* sound_id);				// サウンド再生状態の取得
void MciUpdateSound(int* sound_id);				// ループ再生の強制更新
void MciSetVolume(int* sound_id, int percent);	// 再生音量を設定する

// 拡張文字列処理
wchar_t* HanToZenW(const wchar_t* _src);	//半角文字を全角文字に変換
char* HanToZenA(const char* _src);	//半角文字を全角文字に変換
void PrintStringFA(bool _zenkaku, const char* _format, ...);	//文字列の出力(書式指定版)
void PrintStringFW(bool _zenkaku, const wchar_t* _format, ...);	//文字列の出力(書式指定版)
#ifdef UNICODE
#define	HanToZen	HanToZenA
#define	PrintStringF	PrintStringFA
#else	// UNICODE
#define	HanToZen	HanToZenA
#define	PrintStringF	PrintStringFA
#endif // UNICODE


#ifdef __cplusplus
}
#endif

#endif /* __CONIOEX_H */

/**
* @copyright (c) 2018-2019 HAL Osaka College of Technology & Design (Ihara, H.)
* (2023/11 Adjusted by Tatsumi, K.)
*/
