//*******************************************************************
// field 
//*******************************************************************
#include <stdarg.h>
#include "MYconio.h"
#include "FIELD.h"
#include "conioex.h"

//---------------------
// マクロ定義
//---------------------
#define	FRAME_COLOR		DARKGRAY	// フィールド枠の色
#define COIN_IN_COLOR   RED
#define SLOT_COLOR      RED
#define LIFE_X     (FIELD_X+FIELD_WIDTH+5)
#define LIFE_Y      FIELD_Y
#define INSTRUTION_X   (FIELD_X+FIELD_WIDTH+9)
#define INSTRUTION_Y	FIELD_Y+5


//===================================================================
// フィールドを表示する
//===================================================================
void FieldDraw(int Coin)
{
	backcolor(FRAME_COLOR);
	textcolor(BLACK);

	gotoxy(FIELD_X - 2, FIELD_Y - 1);
	printf("%-*s", FIELD_WIDTH + 2, "  ");
	printf("%s", "  ");

	for (int y = FIELD_Y; y < FIELD_Y + FIELD_HEIGHT; y++) {
		gotoxy(FIELD_X - 2, y);
		printf("%s", "  ");
		gotoxy(FIELD_X + FIELD_WIDTH, y);
		printf("%s", "  ");
	}
	gotoxy(FIELD_X - 2, FIELD_Y + FIELD_HEIGHT);
	printf("%-*s", FIELD_WIDTH + 2, "  ");
	printf("%s", "  ");

	//COIN_IN
	backcolor(COIN_IN_COLOR);
	gotoxy(FIELD_X, FIELD_Y );
	if (Coin < 15) { printf("PRESS SPACE TO SHOOT & COLLECT 15 COINS TO START SLOT     "); }
	else {
		backcolor(GREEN);
		             printf("     PRESS NUM 1,2,3 TO SLOT    PRESS NUM 0 TO RESET SLOT ");
	}
	gotoxy(SLOT_X - 10, SLOT_Y - 5);
	if (Coin < 15) { printf("           INSUFFICIENT COIN                 "); }
	else {
		
		printf("PRESS NUM 1, 2, 3 TO SLOT  (COST 15 COINS)   ");
	}

	resetcolor();
	
}
//===================================================================
// メッセージ行にメッセージを表示する(printfと同じ書式指定)
//===================================================================
void MessageDraw(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);
	backcolor(RED);
	textcolor(WHITE);
	//resetcolor();
	gotoxy(3, FIELD_Y + FIELD_HEIGHT+1);
	clreol();
	vprintf(format, ap);

	va_end(ap);
}
//===================================================================
// HP 
//===================================================================
void LifeDraw(int life) {

	textcolor(LIGHTGREEN);
	gotoxy(LIFE_X, LIFE_Y);
	printf("TIME LEFT: %d S",life/60);
}
//===================================================================
// INSTRUCTION
//===================================================================
void InstructionDraw() {
	
	textcolor(DARKGRAY);
	gotoxy(INSTRUTION_X+4, INSTRUTION_Y);
	printf("************* INSTRUCTION *************\n\n");
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 2);
	printf("   <SHOOT DOWN LUCKY WORDS TO COLLECT COINS>\n");
	textcolor(LIGHTGRAY);
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 4);
	printf("           MOVE:  LEFT\n");
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 5);
	printf("                  RIGHT\n");
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 6);
	printf("                  UP\n");
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 7);
	printf("                  DOWN\n");
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 9);
	printf("           SHOOT: SPACE KEY\n");
	textcolor(DARKGRAY);
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 12);
	printf("            <START THE SLOT>\n");
	textcolor(LIGHTGRAY);
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 14);
	printf("           WHEN 15 COINS COLLECTED");
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 15);
	printf("        PRESS 1,2,3 TO STOP SLOT SCROLL");
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 17);
	printf("        PRESS 0 TO RESTART SLOT SCROLL");
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 19);
	textcolor(BROWN);
	printf("     IF NUMBER IS 7 7 7 YOU WIN 500 COINS");
	gotoxy(INSTRUTION_X + 1, INSTRUTION_Y + 21);
	printf("                GOOD LUCK!!!");
	gotoxy(INSTRUTION_X + 6, INSTRUTION_Y + 37);
	printf("(Press Esc to END Game)");
	
}

//===================================================================
// COIN
//===================================================================
void CoinDraw(int Coin) {
	backcolor(BLUE);
	textcolor(BROWN);
	gotoxy(LIFE_X+15, LIFE_Y+30);
	printf("COIN: %5d/15     ", Coin);
	gotoxy(LIFE_X + 10, LIFE_Y + 31); 
	
	resetcolor();
}


//===================================================================
// SLOT
//===================================================================
void SlotBoxDraw() {

	backcolor(BROWN);
	textcolor(BLACK);
	//1
	gotoxy(SLOT_X-1 , SLOT_Y-1);
	printf("%-*s", SLOT_WIDTH, " ");
	printf("%s", " ");

	gotoxy(SLOT_X-1 , SLOT_Y+1);
	printf("%-*s", SLOT_WIDTH, " ");
	printf("%s", "");

	for (int y = SLOT_Y; y < SLOT_Y + SLOT_HEIGHT; y++) {
		gotoxy(SLOT_X-1, y);
		printf("%s", " ");
		gotoxy(SLOT_X-1 + SLOT_WIDTH, y);
		printf("%s", " ");
	}
	gotoxy(SLOT_X-1, SLOT_Y + SLOT_HEIGHT);
	printf("%-*s", SLOT_WIDTH, " ");
	printf("%s", " ");

	gotoxy(SLOT_X+1, SLOT_Y + SLOT_HEIGHT-1);
	printf("%s", "1");

	//2
	gotoxy(SLOT_X+8 - 1, SLOT_Y - 1);
	printf("%-*s", SLOT_WIDTH, " ");
	printf("%s", " ");

	gotoxy(SLOT_X + 8 - 1, SLOT_Y+1);
	printf("%-*s", SLOT_WIDTH, " ");
	printf("%s", " ");

	for (int y = SLOT_Y; y < SLOT_Y + SLOT_HEIGHT; y++) {
		gotoxy(SLOT_X+8 - 1, y);
		printf("%s", " ");
		gotoxy(SLOT_X+8 - 1 + SLOT_WIDTH, y);
		printf("%s", " ");
	}
	gotoxy(SLOT_X+8 - 1, SLOT_Y + SLOT_HEIGHT);
	printf("%-*s", SLOT_WIDTH, " ");
	printf("%s", " ");

	gotoxy(SLOT_X + 9 , SLOT_Y + SLOT_HEIGHT-1);
	printf("%s", "2");

	//3
	gotoxy(SLOT_X+16 - 1, SLOT_Y - 1);
	printf("%-*s", SLOT_WIDTH, " ");
	printf("%s", " ");

	gotoxy(SLOT_X + 16-1, SLOT_Y+1);
	printf("%-*s", SLOT_WIDTH, " ");
	printf("%s", " ");

	for (int y = SLOT_Y; y < SLOT_Y + SLOT_HEIGHT; y++) {
		gotoxy(SLOT_X+16 - 1, y);
		printf("%s", " ");
		gotoxy(SLOT_X+16- 1 + SLOT_WIDTH, y);
		printf("%s", " ");
	}
	gotoxy(SLOT_X+16 - 1, SLOT_Y + SLOT_HEIGHT);
	printf("%-*s", SLOT_WIDTH, " ");
	printf("%s", " ");

	gotoxy(SLOT_X + 16+1, SLOT_Y + SLOT_HEIGHT-1);
	printf("%s", "3");
	

	//frame
	gotoxy(SLOT_X -3, SLOT_Y - 3);
	printf("%-*s", SLOT_WIDTH*4+5, " ");
	printf("%s", " ");

	for (int y = SLOT_Y-2; y < SLOT_Y-5 + SLOT_HEIGHT*5; y++) {
		gotoxy(SLOT_X -2 - 1, y);
		printf("%s", " ");
		gotoxy(SLOT_X -2 - 1 + SLOT_WIDTH*4+5, y);
		printf("%s", " ");
	}
	gotoxy(SLOT_X -3, SLOT_Y-5+ SLOT_HEIGHT*5);
	printf("%-*s", SLOT_WIDTH*4+5, " ");
	printf("%s", " ");
	resetcolor();
};

