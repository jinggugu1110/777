#pragma once
//*******************************************************************
// 文字列処理モジュール用ヘッダ(Ver.0.1)
//*******************************************************************
//---------------------
// 構造体タグ宣言
//---------------------
typedef struct {
	int		width;
	int		height;
	char** strings;
	const char* fname;
} strings_t;

//---------------------
// 関数プロトタイプ宣言
//---------------------
void StringsCreate(strings_t* p, const char* fname);
void StringsSet(strings_t* p, const char* str[]);
void StringsDelete(strings_t* p);
void StringsDraw(int X, int Y, const strings_t* p);
void StringsErase(int x, int y, const strings_t* p);
