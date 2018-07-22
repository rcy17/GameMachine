#pragma once

//此处是借助定义宏的办法进行DEBUG
//#define DEBUG_RETROSNAKE
//#define DEBUG_GOBANG
//#define DEBUG_TETRIS
//#define DEBUG_STARTREK

//These only work in Windows system
#ifdef WIN32

#include<iostream>
#include<conio.h>
#include<windows.h>
#include<time.h>
//They should be implimented in deferent ways in deferent systems

//Delay for a while
#define CP_SLEEP(time)	_sleep(time)

//Get the time
#define CP_CLOCK		clock()

//Clear the screen
#define CLS				system("cls")
#endif

//Two global variables grids for Screen
extern unsigned char g_ggcPrint[16][32];
extern unsigned char g_ggcPrintSave[16][32];

//Here define some const number
#define iYEDGE  (16)
#define iXEDGE  (32)
#define REFRESHTIME (150)

//Here give some keys involved
enum keyin
{
	//no operation
	NOOPERATION,
	UP,		//for game pad
	UP2,	//for joysticks
	DOWN,	//down
	DOWN2,
	LEFT,	//left
	LEFT2,
	RIGHT,	//right
	RIGHT2,
	RIGHTUP,//these four direction not neccessary, only for some judge
	RIGHTDOWN,
	LEFTUP,
	LEFTDOWN,

	//Ensure the choice
	OK,		
	//Cancel present operation or withdraw
	CANCEL,	
	//Start or go on the game
	START,	
	//Pause the game
	PAUSE,	
	//Exit the game
	EXIT,	
	//Restart this round
	RESTART,
	//Back to the last menu or level
	BACK,	
	//Get to the next one
	NEXT,
	//Reload this game
	RESET,	


};

enum Game_Name
{
	NONE,
	RETROSNAKE,
	GOBANG,
	TETRIS,
	SOKOBAN,
	STARTREK,

	//Label the end of enum
	END_LABEL,	
	//Just a flag to print, shouldn't be counted
	DRETROSNAKE 
};

enum item
{
	//Basic blank
	BLANK,

	//These two for RetroSnake
	FOOD,
	SNAKE,

	//These five for Gobang
	HIGHLIGHT,
	BLACKCHESS,
	WHITECHESS,
	BLACKHIGHLIGHT,
	WHITEHIGHLIGHT,

	//These two for Tetris
	BLOCK,
	EDGE,

	//These are for Sokoban
	PATH,
	OBSTACLE,
	PERSON,
	BOX,
	AIM,
	EMPTY_LABEL,
	PERSON_AIM,
	BOX_AIM,

	//These two for Startrek
	FIGHTER,
	BULLET,

	
	//This is the second snake
	SNAKE2 = 129
};


#ifdef WIN32
//Here decide how to print in Windows
inline char Dot(int n)
{
	char tem;
	switch (n)
	{
	case BLANK:
		tem = ' ';
		break;
	case SNAKE:
		tem = '0';
		break;
	case FOOD:
		tem = '8';
		break;
	case BLACKCHESS:
		tem = 'x';
		break;
	case WHITECHESS:
		tem = 'y';
		break;
	case HIGHLIGHT:
		tem = '?';
		break;
	case BLACKHIGHLIGHT:
		tem = 'X';
		break;
	case WHITEHIGHLIGHT:
		tem = 'Y';
		break;
	case BLOCK:
		tem = '*';
		break;
	case EDGE:
		tem = '#';
	case OBSTACLE:
		tem = 'O';
		break;
	case PERSON:
		tem = 'p';
		break;
	case BOX:
		tem = 'b';
		break;
	case AIM:
		tem = '#';
		break;
	case PERSON_AIM:
		tem = 'P';
		break;
	case BOX_AIM:
		tem = 'B';
		break;
	case SNAKE2:
		tem = '*';
		break;
	case FIGHTER:
		tem = '5';
		break;
	case BULLET:
		tem = 'x';
		break;
	default:
		tem = ' ';
		break;
	}
	return tem;
}

#endif

//Here refresh the screen by the two grids
inline void Print(int GameNow, unsigned char now[iYEDGE][iXEDGE], unsigned char past[iYEDGE][iXEDGE])
{
	for (int y = 0; y < iYEDGE; y++)
		for (int x = 0; x < iXEDGE; x++)
		{
			//Only refresh when needed
			if (now[y][x] != past[y][x])
			{
#ifdef  WIN32
				static HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
				COORD coord;
				if (GameNow == TETRIS)
				{
					coord.X = y;
					coord.Y = x;
				}
				else if (GameNow == STARTREK)
				{
					coord.X = iYEDGE - y;
					coord.Y = x;
				}
				else
				{
					coord.X = x;
					coord.Y = y;
				}
				SetConsoleCursorPosition(hout, coord);
				if (GameNow == DRETROSNAKE && now[y][x] >= SNAKE2)
					putchar(Dot(SNAKE2));
				else if ((GameNow == RETROSNAKE || GameNow == DRETROSNAKE) && now[y][x] >= SNAKE)
					putchar(Dot(SNAKE));
				else
					putchar(Dot(now[y][x]));
#endif
			}
		}
}

//Here give the map between input and the key
inline keyin PressKey(bool stickpermitted = true, bool stickpermitted2 = true)
{
	keyin KeyPressed = NOOPERATION;
#ifdef WIN32
	unsigned char tem = '\0';
	if (_kbhit())
		tem = _getch();
	/*if (flush)
	while (_kbhit())
	_getch();*/
	switch (tem)
	{
	case'a':
		if (stickpermitted)
			KeyPressed = LEFT;
		break;
	case's':
		if (stickpermitted)
			KeyPressed = DOWN;
		break;
	case'd':
		if (stickpermitted)
			KeyPressed = RIGHT;
		break;
	case'w':
		if (stickpermitted)
			KeyPressed = UP;
		break;
	case'\r':
		KeyPressed = OK;
		break;
	case'x':
		KeyPressed = CANCEL;
		break;
	case'z':
		KeyPressed = START;
		break;
	case'p':
		KeyPressed = PAUSE;
		break;
	case 27://ESC
		KeyPressed = EXIT;
		break;
	case'r':
		KeyPressed = RESTART;
		break;
	case'\b':
		KeyPressed = BACK;
		break;
	case' ':
		KeyPressed = RESET;
		break;
	case 'q':
		KeyPressed = BACK;
		break;
	case 'e':
		KeyPressed = NEXT;
		break;
	case 224:
		//for direction key
		tem = _getch();
		if (stickpermitted2)
			switch (tem)
			{
			case 72:
				KeyPressed = UP2;
				break;
			case 80:
				KeyPressed = DOWN2;
				break;
			case 75:
				KeyPressed = LEFT2;
				break;
			case 77:
				KeyPressed = RIGHT2;
				break;
			}
		break;
	default:
		KeyPressed = NOOPERATION;
		break;
	}
#endif
	return KeyPressed;
}

