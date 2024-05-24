//*******************************************************************
// common
//*******************************************************************
#include "MYconio.h"
#include "FIELD.h"

//===================================================================
// 音楽ファイルオープン処理（エラー処理付き）
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
