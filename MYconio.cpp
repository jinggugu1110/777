//*******************************************************************
// WindowsAPI
//*******************************************************************
#include "MYconio.h"
#include <digitalv.h>

//---------------------
// �O���[�o���ϐ��錾
//---------------------
static char KeyTable[256];

//===================================================================
// ���̓|�[�g���m�F����
//===================================================================
bool input(int key)
{
    HANDLE	h = GetStdHandle(STD_INPUT_HANDLE);// �W�����͂̃n���h������
    DWORD	event_num = 0;

    if (GetNumberOfConsoleInputEvents(h, &event_num) && event_num) {
        for (unsigned int i = 0; i < event_num; i++) {
            DWORD			read_num = 0;
            INPUT_RECORD	input_record;

            if (ReadConsoleInput(h, &input_record, 1, &read_num) == 0) {
                printf("ReadConsoleInput�G���[\n");
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
// ���̓|�[�g�̋L�^���N���A����
//===================================================================
void reinput(void)
{
    memset(KeyTable, 0, sizeof(KeyTable));
}
//===================================================================
// MP3�`���̃T�E���h�t�@�C���̃I�[�v��
//===================================================================
// �T�E���h�����Ŏg���\���̂̃^�O�錾
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

	// �t�@�C�����̓V�t�gJIS�̂���mciSendCommandA�֐����g��
	if (mciSendCommandA(0, MCI_OPEN, dwCmd, (DWORD)&param)) {
		return 0;		// �I�[�v���o���Ȃ������ꍇ
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
// MP3�`���̃T�E���h�t�@�C���̃N���[�Y
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
// MP3�`���̃T�E���h�̍Đ��i�J��Ԃ��Ȃ��j
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
// MP3�`���̃T�E���h�̍Đ��i�J��Ԃ��Đ��j
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
// MP3�`���̃T�E���h�Đ��̒�~
//===================================================================
void stopsound(int soundID)
{
	sound_t* p = (sound_t*)soundID;

	if (p != 0 && p->deviceID != 0) {
		mciSendCommand(p->deviceID, MCI_STOP, MCI_WAIT, 0);
		mciSendCommand(p->deviceID, MCI_SEEK, MCI_SEEK_TO_START, 0);
	}
}