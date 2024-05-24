#pragma once
//*******************************************************************
// �����񏈗����W���[���p�w�b�_(Ver.0.1)
//*******************************************************************
//---------------------
// �\���̃^�O�錾
//---------------------
typedef struct {
	int		width;
	int		height;
	char** strings;
	const char* fname;
} strings_t;

//---------------------
// �֐��v���g�^�C�v�錾
//---------------------
void StringsCreate(strings_t* p, const char* fname);
void StringsSet(strings_t* p, const char* str[]);
void StringsDelete(strings_t* p);
void StringsDraw(int X, int Y, const strings_t* p);
void StringsErase(int x, int y, const strings_t* p);
