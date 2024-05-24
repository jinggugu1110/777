//*******************************************************************
// 文字列処理モジュール(Ver.0.1)
// 作成日：2021/12/07
// 作成者：百春
//*******************************************************************
#include "MYconio.h"
#include "STRINGS.h"

//========================================================================
// ファイル入力して文字列情報を生成する
//========================================================================
// 内部関数：ファイルの終わりまで入力して文字数と行数を取得
static void input_all(FILE* fp, int *size, int *height)
{
    int     c;

    *height = *size = 0;
    while ((c = getc(fp)) != EOF) {
        (*size)++;                      // 文字数カウントアップ
        if (c == '\n') (*height)++;     // 行数カウントアップ
    }
}
// 内部関数：文字ポインタ配列を作りながら文字列を入力
static void make_strings(FILE *fp, strings_t* p)
{
    int     c, len;
    char* cp = (char*)(p->strings + p->height);

    p->width = 0;
    for (int i = 0; i < p->height; i++) {
        *(p->strings + i) = cp;
        while ((c = getc(fp)) != '\n' && c != EOF) {
            *cp++ = c;
        }
        *cp++ = '\0';
        len = strlen(*(p->strings + i));
        if (p->width < len) {
            p->width = len;
        }
    }
}
// ファイル入力して文字列情報を生成する関数の本体
void StringsCreate(strings_t* p, const char* fname)
{
    FILE* fp;
    int     size;               // 文字数カウント用

    fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("\n%sがオープン出来ませんでした\n", fname);
        WAIT_ENTER();
        exit(1);
    }
    
    input_all(fp, &size, &p->height);   // 空読み込みで文字数と行数を調べる

    p->strings = (char**)malloc(p->height * sizeof(char*) + size + 1);
    if (p->strings == NULL) {
        printf("\n動的メモリを獲得できませんでした\n");
        WAIT_ENTER();
        exit(1);
    }

    p->fname = fname;       // ファイル名を記録(動的メモリ獲得済状態)
    rewind(fp);             // ファイルの入力位置を先頭に戻す

    make_strings(fp, p);    // 文字ポインタ配列を作りながら文字列を読込む

    fclose(fp);
}
//========================================================================
// 文字列配列から文字列情報を作成する
//========================================================================
void StringsSet(strings_t* p, const char* str[])
{
    p->strings = (char**)str;
    p->width = 0;
    for (p->height = 0; str[p->height] != NULL; p->height++) {
        int	len = strlen(str[p->height]);

        if (len > p->width)	p->width = len;
    }
    p->fname = NULL;    // ファイルからの文字列セットではない状態にする
}
//========================================================================
// 文字列情報の動的メモリを返却する
//========================================================================
void StringsDelete(strings_t* p)
{
    if (p->fname != NULL) { // ファイルからの文字列セットの場合
        free(p->strings);       // 動的メモリを返却
        p->fname = NULL;        // 動的メモリ未獲得状態にする
    }
}
//========================================================================
// 指定座標位置に文字列配列を表示
//========================================================================
void StringsDraw(int X, int Y, const strings_t* p)
{
    for (int i = 0; i < p->height; i++) {
        gotoxy(X, Y++);
        printf("%s", p->strings[i]);
    }
}
//========================================================================
// 指定座標位置に表示した文字配列を消去する(スペースを表示)
//========================================================================
void StringsErase(int x, int y, const strings_t* p)
{
    for (int i = 0; i < p->height; i++) {
        gotoxy(x, y++);
        printf("%*s", p->width, "");
    }
}
