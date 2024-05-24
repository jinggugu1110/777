#include <digitalv.h>

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
