/**
* @file	conioex.h
* @brief	Win32 �R���\�[�� I/O �g�� for Visual C++/Borland C++
*
* @date	2023/11 2023�N�x��
* @version	1.02
* @note
*  �R���\�[���E�B���h�E�̃v���p�e�B�̐ݒ���ȉ��ɕύX���邱��
*  �E�u�]���̃R���\�[�����g���v�̃`�F�b�N���O��
*  �E�u�ȈՕҏW���[�h�v�̃`�F�b�N���O��
* 
* �R���\�[���֐��ɂ���
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

#define CONSOLE_WINDOW_WIDTH_MIN	60	// 60�������ƃX�N���[���o�b�t�@�̍쐬�Ɏ��s����
#define CONSOLE_WINDOW_HEIGHT_MIN	1

// https://en.wikipedia.org/wiki/Color_Graphics_Adapter

/* �L�[�R�[�h */
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
#define PK_NFER					VK_KANA		/* [���ϊ�]	*/
#define PK_XFER					VK_CONVERT	/* [�ϊ�]	*/
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
#define PK_NUMPAD0				VK_NUMPAD0	/* �e���L�[��[0]	*/
#define PK_NUMPAD1				VK_NUMPAD1	/* �e���L�[��[1]	*/
#define PK_NUMPAD2				VK_NUMPAD2	/* �e���L�[��[2]	*/
#define PK_NUMPAD3				VK_NUMPAD3	/* �e���L�[��[3]	*/
#define PK_NUMPAD4				VK_NUMPAD4	/* �e���L�[��[4]	*/
#define PK_NUMPAD5				VK_NUMPAD5	/* �e���L�[��[5]	*/
#define PK_NUMPAD6				VK_NUMPAD6	/* �e���L�[��[6]	*/
#define PK_NUMPAD7				VK_NUMPAD7	/* �e���L�[��[7]	*/
#define PK_NUMPAD8				VK_NUMPAD8	/* �e���L�[��[8]	*/
#define PK_NUMPAD9				VK_NUMPAD9	/* �e���L�[��[9]	*/
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
#define PK_DOT					VK_DECIMAL	/* �e���L�[��[.]	*/
#define PK_DIV					VK_DIVIDE	/* �e���L�[��[/]	*/
#define PK_BSLUSH				0x0E2		/* [_]	*/

#define PK_SEMICOLON			0x0BB		/* [;]	*/
#define PK_ADD					VK_ADD		/* �e���L�[��[+]	*/
#define PK_COLON				0x0BA		/* [:]	*/
#define PK_MUL					VK_MULTIPLY	/* �e���L�[��[*]	*/
#define PK_RBRACE				0x0DD		/* []]	*/

#define PK_ATMARK				0x0C0		/* [@]	*/
#define PK_LBRACE				0x0DB		/* [[]	*/

#define PK_MINUS				0x0BD		/* [-]	*/
#define PK_SUB					VK_SUBTRACT	/* �e���L�[��[-]	*/
#define PK_XOR					0x0DE		/* [^]	*/
#define PK_YEN					0x0DC		/* [\]	*/

#define PK_KANJI				0x0F3		/* [���p/�S�p]	*/
#define PK_CAPS					0x0F0		/* [�p��][�Ђ炪��]	*/

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

// ������
COORD InitConio(int width, int height);			// ������
void UninitConio();								// �I������
int InitDoubleBuffer(void);						// �_�u���o�b�t�@������
void SetScreenFontSize(int width, int height);	// �t�H���g�T�C�Y�ݒ�

// �`��
int GetCursorX(void);							// ���������̃J�[�\���ʒu���擾
int GetCursorY(void);							// ���������̃J�[�\���ʒu���擾
void SetCursorPosition(int x, int y);			// �J�[�\���ʒu�̈ړ�
int GetScreenSizeX(void);						// ���������̃X�N���[���T�C�Y���擾
int GetScreenSizeY(void);						// ���������̃X�N���[���T�C�Y���擾
void SetCursorOriginZeroFlag(bool flag);		// �J�[�\���ʒu�̌��_��ݒ�
#ifdef UNICODE
///#define	PrintString	PrintStringW
void PrintString(const char* _srcbuf, int _size);	// ������̏o��(�}���`�o�C�g��Unicode�ϊ���)
#else
#define	PrintString	PrintStringA
#endif // UNICODE
void PrintStringW(const wchar_t* _srcbuf, int _size);	// ������̏o�́iUnicode�����p�j
void PrintStringA(const char* _srcbuf, int _size);	// ������̏o�́i�}���`�o�C�g�����p�j
void ClearLine(void);							// �s���܂ŏ���
void ClearScreen(void);							// ��ʏ���
void FlipScreen(void);							// �_�u���o�b�t�@���̕`��ʐؑ�
void SetHighVideoColor(void);					// �����F���P�x��
void SetLowVideoColor(void);					// �����F��P�x��
void SetNormalVideoColor(void);					// ���蕶���F�ݒ�
void SetTextBackColor(int color);				// �����w�i�F�ݒ�
void SetConsoleTextColor(int color);			// �����F�ݒ�
void SetTextAttribute(int attribute);			// �����F�w�i�F�����ݒ�
void SetCursorType(int type);					// �J�[�\���^�C�v�ݒ�
void InsertLine(void);							// ���ݍs�ɑ}��
void DeleteLine(void);							// ���ݍs�̍폜
void PrintFrameBuffer(char* buf);				// ������̈ꊇ�]��
void ClearFrameBuffer(char* buf);				// �ꊇ�]���p�o�b�t�@�N���A

// �E�B���h�E
void SetCaption(const char* title);					// �R���\�[���E�B���h�E�̃^�C�g���o�[�ɕ\�������e�L�X�g��ݒ�
int GetCaption(char* title, int len);			// �R���\�[���E�B���h�E�̃^�C�g���o�[�ɕ\�������e�L�X�g���擾

// ���͌n
void ResetKeyMap(void);							// �L�[��񃊃Z�b�g
int InputKeyMouse(int port);					// �L�[�{�[�h�E�}�E�X����
int InputJoystick(int port);					// �Q�[���p�b�h����(DirectInput�Ή�)
int InputJoystickX(int port);					// �Q�[���p�b�h����(XInput�Ή�)

// �����o��
int* MciOpenSound(const char* path);			// �T�E���h�t�@�C�����J��
void MciCloseSound(int* sound_id);				// �T�E���h�t�@�C�������
void MciPlaySound(int* sound_id, int repeat);	// �T�E���h���Đ�����
void MciStopSound(int* sound_id);				// �T�E���h�Đ����~����
int MciCheckSound(int* sound_id);				// �T�E���h�Đ���Ԃ̎擾
void MciUpdateSound(int* sound_id);				// ���[�v�Đ��̋����X�V
void MciSetVolume(int* sound_id, int percent);	// �Đ����ʂ�ݒ肷��

// �g�������񏈗�
wchar_t* HanToZenW(const wchar_t* _src);	//���p������S�p�����ɕϊ�
char* HanToZenA(const char* _src);	//���p������S�p�����ɕϊ�
void PrintStringFA(bool _zenkaku, const char* _format, ...);	//������̏o��(�����w���)
void PrintStringFW(bool _zenkaku, const wchar_t* _format, ...);	//������̏o��(�����w���)
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
