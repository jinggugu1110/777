//*******************************************************************
// WindowsAPI
//*******************************************************************
#include "MYconio.h"
#include <digitalv.h>

//---------------------
// グローバル変数宣言
//---------------------
static char KeyTable[256];

//===================================================================
// 入力ポートを確認する
//===================================================================
bool input(int key)
{
    HANDLE	h = GetStdHandle(STD_INPUT_HANDLE);// 標準入力のハンドルを取
    DWORD	event_num = 0;

    if (GetNumberOfConsoleInputEvents(h, &event_num) && event_num) {
        for (unsigned int i = 0; i < event_num; i++) {
            DWORD			read_num = 0;
            INPUT_RECORD	input_record;

            if (ReadConsoleInput(h, &input_record, 1, &read_num) == 0) {
                printf("ReadConsoleInputエラー\n");
                exit(1);
            }
            if (read_num == 0)  continue;

            DWORD   key_code = input_record.Event.KeyEvent.wVirtualKeyCode;
            DWORD   state = input_record.Event.KeyEvent.dwControlKeyState;

            if (key_code <= 0x0FF) {
                KeyTable[key_code] = (input_record.Event.KeyEvent.bKeyDown != 0);
                KeyTable[VK_MENU] = ((state & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0);
                KeyTable[VK_CONTROL] = ((state & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0);
                KeyTable[VK_SHIFT] = ((state & SHIFT_PRESSED) != 0);
            }
        }
    }
    return KeyTable[key];
}
//===================================================================
// 入力ポートの記録をクリアする
//===================================================================
void reinput(void)
{
    memset(KeyTable, 0, sizeof(KeyTable));
}
//===================================================================
// MP3形式のサウンドファイルのオープン
//===================================================================
// サウンド処理で使う構造体のタグ宣言
typedef struct {
	const char* fname;
	int			deviceID;
}sound_t;
int opensound(const char* fname)
{
	DWORD				dwCmd = MCI_OPEN_TYPE | MCI_OPEN_ELEMENT;
	MCI_OPEN_PARMSA		param = { 0 };

	param.lpstrDeviceType = "MPEGVideo";
	param.lpstrElementName = fname;

	// ファイル名はシフトJISのためmciSendCommandA関数を使う
	if (mciSendCommandA(0, MCI_OPEN, dwCmd, (DWORD)&param)) {
		return 0;		// オープン出来なかった場合
	}
	sound_t* p;
	p = (sound_t*)malloc(sizeof(sound_t));
	if (p != NULL) {
		p->fname = fname;
		p->deviceID = (int)param.wDeviceID;
	}
	return (int)p;
}
//===================================================================
// MP3形式のサウンドファイルのクローズ
//===================================================================
void closesound(int soundID)
{
	sound_t* p = (sound_t*)soundID;

	if (p != 0 && p->deviceID != 0) {
		mciSendCommand(p->deviceID, MCI_CLOSE, 0, 0);
		free(p);
	}
}
//===================================================================
// MP3形式のサウンドの再生（繰り返しなし）
//===================================================================
void playsound(int soundID)
{
	MCI_PLAY_PARMS			param = { 0 };
	sound_t* p = (sound_t*)soundID;

	if (p != 0 && p->deviceID != 0) {
		mciSendCommand(p->deviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
		mciSendCommand(p->deviceID, MCI_PLAY, 0, (DWORD)&param);
	}
}
//===================================================================
// MP3形式のサウンドの再生（繰り返し再生）
//===================================================================
void repeatsound(int soundID)
{
	DWORD					dwCmd = MCI_FROM | MCI_DGV_PLAY_REPEAT;
	MCI_PLAY_PARMS			param = { 0 };
	sound_t* p = (sound_t*)soundID;

	if (p != 0 && p->deviceID != 0) {
		mciSendCommand(p->deviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
		mciSendCommand(p->deviceID, MCI_PLAY, dwCmd, (DWORD)&param);
	}
}
//===================================================================
// MP3形式のサウンド再生の停止
//===================================================================
void stopsound(int soundID)
{
	sound_t* p = (sound_t*)soundID;

	if (p != 0 && p->deviceID != 0) {
		mciSendCommand(p->deviceID, MCI_STOP, MCI_WAIT, 0);
		mciSendCommand(p->deviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
	}
}