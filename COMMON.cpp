//*******************************************************************
// common
//*******************************************************************
#include "MYconio.h"
#include "FIELD.h"

//===================================================================
// ���y�t�@�C���I�[�v�������i�G���[�����t���j
//===================================================================
int opensoundEX(const char* fname)
{
	int		sound;

	sound = opensound(fname);
	if (sound == 0) {
		MessageDraw("%s is not found", fname);
		WAIT_ENTER();
		MessageDraw("");
	}
	return sound;
}
