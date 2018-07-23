#pragma once
//------------------------------------------------------------------------------------
//
//	Name:	Compatible.h
//
//	Desc:	defining some grands and functions to be compatible with deferent systems
//
//------------------------------------------------------------------------------------

#include "Global.h"


//these including and grands only work in windows system
#ifdef _WIN32

#include<iostream>
#include<conio.h>
#include<windows.h>
#include<time.h>
//they should be implimented in deferent ways in deferent systems

//delay for a while
#define CP_SLEEP(time)	Sleep(time) 

//get the time
#define CP_CLOCK		clock()

//clear the screen
#define CLS				system("cls")
#endif

#ifdef _WIN32
//here decide how to print in Windows
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

//put the cursor to any place
inline void MoveCursor(char x, char y)
{
	static HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { x,y };
	SetConsoleCursorPosition(handle, coord);
}
#endif

//here refresh the screen by the two grids
inline void Print(int GameNow)
{
	for (int y = 0; y < iYEDGE; y++)
		for (int x = 0; x < iXEDGE; x++)
		{
			//only refresh when needed
			if (g_ggcPrint[y][x] != g_ggcPrintSave[y][x])
			{
#ifdef  _WIN32
				if (GameNow == TETRIS)
				{
					MoveCursor(y, x);
				}
				else if (GameNow == STARTREK)
				{
					MoveCursor(iYEDGE - y, x);
				}
				else
				{
					MoveCursor(x, y);
				}
				if (GameNow == DRETROSNAKE && g_ggcPrint[y][x] >= SNAKE2)
					putchar(Dot(SNAKE2));
				else if ((GameNow == RETROSNAKE || GameNow == DRETROSNAKE) && g_ggcPrint[y][x] >= SNAKE)
					putchar(Dot(SNAKE));
				else
					putchar(Dot(g_ggcPrint[y][x]));
#endif
			}
		}
}

//here give the map between input and the key
inline keyin PressKey(bool stickpermitted = true, bool stickpermitted2 = true)
{
	keyin KeyPressed = NOOPERATION;
#ifdef _WIN32
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



