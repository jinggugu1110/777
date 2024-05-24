#include "MYconio.h"
#include "FIELD.h"
#include "PLAYER.h"
#include "BULLET.h"
#include "ENEMY.h"
#include <time.h>
#include "SLOT.h"
#include <windows.h>
#include <locale.h>
#include "main.h"
#include "conioex.h"
#include "START.h"

#define	MENU_COLOR		BROWN	// フィールド枠の色
#define MENU_X          35
#define MENU_Y          18
#define	TITLE_COLOR		BROWN	// フィールド枠の色
#define TITLE_X   
#define TITLE_Y	
#define INSTRUTION_X   (FIELD_X+FIELD_WIDTH+9)
#define INSTRUTION_Y	FIELD_Y+5

bool titleflag = TRUE;
bool preinstructflag = TRUE;
bool instructflag = TRUE;
bool countdownflag = TRUE;
bool eflag = TRUE;

const char* path = { "money.mp3", };
int* l = NULL;

void StartMenu() {
	l = MciOpenSound(path);
	title();
	
	instruct();
	
	countdown();

	MciCloseSound(l);
}

void title() {
	system("cls");
	while (titleflag == TRUE) {
		backcolor(BROWN);
		gotoxy(MENU_X + 5, MENU_Y-3);
		printf("                               ");
		resetcolor();
		gotoxy(MENU_X + 5, MENU_Y - 5);
		textcolor(BROWN);
		printf("L U C K Y  F L I G H T - 7 7 7");
		textcolor(RED);
		gotoxy(MENU_X+5, MENU_Y);
		printf("Press ENTER Key To Start Game ");
		if (input(PK_ENTER)) {
			MciPlaySound(l, 0);
			titleflag = FALSE;
			instructflag = TRUE;
		}
	}
}

void instruct() {
	system("cls");
	while (instructflag == TRUE) 
	{
		textcolor(RED);
		gotoxy(MENU_X+10, MENU_Y+25);
		printf(" <Are You Ready? Press ENTER Key To Skip Instruction & Start Flight>>>>>");
		//draw player
		textcolor(DARKGRAY);
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 5);
		printf("    A    ");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 4);
		printf("   A A       ---   L U C K Y  F L I G H T - 7 7 7 ");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 3);
		printf("[7][7][7]          Move : Up/Down/Left/Right Key");

		//draw enemy
		textcolor(LIGHTMAGENTA);
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 27);
		printf("Y");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 26);
		printf(" E        ---   W O R D S (Zero coin)");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 25);
		printf("  A              Shoot Them otherwise Will be Barrier");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 24);
		printf("   H");

		textcolor(LIGHTGREEN);
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 23);
		printf("C");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 22);
		printf(" O        ---   W O R D S (1 coin)");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 21);
		printf("  O              Shoot Them and Get Coins");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 20);
		printf("   L");

		textcolor(LIGHTRED);
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 19);
		printf("N");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 18);
		printf(" I        ---   W O R D S (2 Coins)");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 17);
		printf("  C             Shoot Them and Get Coins");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 16);
		printf("   E");

		textcolor(RED);
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 15);
		printf("G");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 14);
		printf(" O        ---   W O R D S (3 Coins)");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 13);
		printf("  O              Shoot Them and Get Coins");
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 12);
		printf("   D");

		//draw bullet
		textcolor(BROWN);
		gotoxy(FIELD_WIDTH / 2 - 13, FIELD_HEIGHT - 10);
		printf("($)       ---   B U L L E T (Using SPACE Key to Shoot)");

		SlotBoxDraw();
		//InstructionDraw();
		textcolor(LIGHTGRAY);
		gotoxy(INSTRUTION_X + 5, INSTRUTION_Y + 6);
		printf("You have 60 Seconds");
		gotoxy(INSTRUTION_X + 5, INSTRUTION_Y + 8);
		printf("to get coins and play the slot");
		textcolor(LIGHTGRAY);
		gotoxy(INSTRUTION_X + 5, INSTRUTION_Y + 12);
		printf("When 15 Coins Collected");
		gotoxy(INSTRUTION_X + 5, INSTRUTION_Y + 14);
		printf("Press 1, 2, 3 TO STOP SLOT SCROLL");
		gotoxy(INSTRUTION_X + 5, INSTRUTION_Y + 16);
		printf("Press 0 TO RESTART SLOT SCROLL");
		gotoxy(INSTRUTION_X + 5, INSTRUTION_Y + 18);
		textcolor(BROWN);
		printf("IF NUMBER IS 7 7 7 YOU WIN 500 COINS!");
		gotoxy(INSTRUTION_X + 5, INSTRUTION_Y + 19);
		printf("GOOD LUCK!!!");
		textcolor(DARKGRAY);
		gotoxy(INSTRUTION_X , INSTRUTION_Y);
		printf("************* INSTRUCTION *************\n\n");
		gotoxy(INSTRUTION_X -30, INSTRUTION_Y);
		printf("************* INSTRUCTION *************\n\n");
		gotoxy(INSTRUTION_X - 60, INSTRUTION_Y);
		printf("************* INSTRUCTION *************\n\n");
		//draw box
		textcolor(RED);
		gotoxy(SLOT_X + 1, SLOT_Y);
		printf("%s", "7");
		gotoxy(SLOT_X + 9, SLOT_Y);
		printf("%s", "7");
		gotoxy(SLOT_X + 17, SLOT_Y);
		printf("%s", "7");
		//draw 777
		textcolor(RED);
		gotoxy(SLOT_X + 1, SLOT_Y);
		printf("%s", "7");
		gotoxy(SLOT_X + 9, SLOT_Y);
		printf("%s", "7");
		gotoxy(SLOT_X + 17, SLOT_Y);
		printf("%s", "7");
		if (!input(PK_ENTER)) {
			preinstructflag = FALSE;
		}
		if (preinstructflag  == FALSE && input(PK_ENTER)) {
			instructflag = FALSE;
		}
		
	}
}

void countdown() {
	system("cls");
	while (countdownflag == TRUE) {
		//START count down
		gotoxy(MENU_X + 15, MENU_Y + 5);
		textcolor(WHITE);
		printf(" Credit: 0 Coin ");
		textcolor(RED);
		gotoxy(MENU_X + 13, MENU_Y);
		printf("   3 s  T O  G O!!");
		gotoxy(MENU_X + 13, MENU_Y + 3);
		textcolor(YELLOW);
		printf("     *  *  *");
		Sleep(500);
		gotoxy(MENU_X + 13, MENU_Y);
		textcolor(RED);
		printf("   2 s  T O  G O!!");
		gotoxy(MENU_X + 13, MENU_Y + 3);
		textcolor(YELLOW);
		printf("     *  *   ");
		Sleep(500);
		gotoxy(MENU_X + 13, MENU_Y);
		textcolor(RED);
		printf("   1 s  T O  G O!!");
		gotoxy(MENU_X + 13, MENU_Y + 3);
		textcolor(YELLOW);
		printf("     *    ");
		Sleep(500);
		gotoxy(MENU_X + 13, MENU_Y);
		textcolor(RED);
		printf("   S T A R T  !  !  !  ");
		gotoxy(MENU_X + 13, MENU_Y + 3);
		printf("          ");
		Sleep(500);
		resetcolor();
		
			countdownflag = FALSE;
	}
}

void ResultMenu()
{   
	int coin = GetPlayerCoin();
	system("cls");
	while (eflag == TRUE) 
	{
		textcolor(RED);
		gotoxy(MENU_X + 8, MENU_Y);
		printf(" G A M E  O V E R ");
		textcolor(WHITE);
		gotoxy(MENU_X + 8, MENU_Y + 8);
		printf("**  Kindly Remind  ** ");
		gotoxy(MENU_X + 8, MENU_Y + 10);
		printf("The winning rate of this game is way high than a real slot! ");
		gotoxy(MENU_X + 8, MENU_Y + 12);
		textcolor(RED);
		printf("PRESS ESC & ENTER TO CLOSE"); 
		textcolor(BROWN);
		gotoxy(MENU_X + 8, MENU_Y + 5);
		printf("You Have Earned %d Coins", coin);
		if (input(PK_ESC))
			eflag = FALSE;
	}

}