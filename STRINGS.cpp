//*******************************************************************
// �����񏈗����W���[��(Ver.0.1)
// �쐬���F2021/12/07
// �쐬�ҁF�S�t
//*******************************************************************
#include "MYconio.h"
#include "STRINGS.h"

//========================================================================
// �t�@�C�����͂��ĕ�������𐶐�����
//========================================================================
// �����֐��F�t�@�C���̏I���܂œ��͂��ĕ������ƍs�����擾
static void input_all(FILE* fp, int *size, int *height)
{
    int     c;

    *height = *size = 0;
    while ((c = getc(fp)) != EOF) {
        (*size)++;                      // �������J�E���g�A�b�v
        if (c == '\n') (*height)++;     // �s���J�E���g�A�b�v
    }
}
// �����֐��F�����|�C���^�z������Ȃ��當��������
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
// �t�@�C�����͂��ĕ�������𐶐�����֐��̖{��
void StringsCreate(strings_t* p, const char* fname)
{
    FILE* fp;
    int     size;               // �������J�E���g�p

    fp = fopen(fname, "r");
    if (fp == NULL) {
        printf("\n%s���I�[�v���o���܂���ł���\n", fname);
        WAIT_ENTER();
        exit(1);
    }
    
    input_all(fp, &size, &p->height);   // ��ǂݍ��݂ŕ������ƍs���𒲂ׂ�

    p->strings = (char**)malloc(p->height * sizeof(char*) + size + 1);
    if (p->strings == NULL) {
        printf("\n���I���������l���ł��܂���ł���\n");
        WAIT_ENTER();
        exit(1);
    }

    p->fname = fname;       // �t�@�C�������L�^(���I�������l���Ϗ��)
    rewind(fp);             // �t�@�C���̓��͈ʒu��擪�ɖ߂�

    make_strings(fp, p);    // �����|�C���^�z������Ȃ��當�����Ǎ���

    fclose(fp);
}
//========================================================================
// ������z�񂩂當��������쐬����
//========================================================================
void StringsSet(strings_t* p, const char* str[])
{
    p->strings = (char**)str;
    p->width = 0;
    for (p->height = 0; str[p->height] != NULL; p->height++) {
        int	len = strlen(str[p->height]);

        if (len > p->width)	p->width = len;
    }
    p->fname = NULL;    // �t�@�C������̕�����Z�b�g�ł͂Ȃ���Ԃɂ���
}
//========================================================================
// ��������̓��I��������ԋp����
//========================================================================
void StringsDelete(strings_t* p)
{
    if (p->fname != NULL) { // �t�@�C������̕�����Z�b�g�̏ꍇ
        free(p->strings);       // ���I��������ԋp
        p->fname = NULL;        // ���I���������l����Ԃɂ���
    }
}
//========================================================================
// �w����W�ʒu�ɕ�����z���\��
//========================================================================
void StringsDraw(int X, int Y, const strings_t* p)
{
    for (int i = 0; i < p->height; i++) {
        gotoxy(X, Y++);
        printf("%s", p->strings[i]);
    }
}
//========================================================================
// �w����W�ʒu�ɕ\�����������z�����������(�X�y�[�X��\��)
//========================================================================
void StringsErase(int x, int y, const strings_t* p)
{
    for (int i = 0; i < p->height; i++) {
        gotoxy(x, y++);
        printf("%*s", p->width, "");
    }
}
