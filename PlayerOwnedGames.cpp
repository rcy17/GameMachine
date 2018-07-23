
#include"PlayerOwnedGames.h"
#include"Player.h"



#ifdef WIN32
#include<conio.h>
#include<iostream>
//#pragma warning(disable:4996)
using std::cout;
#endif

unsigned char g_ggcPrint[16][32];
unsigned char g_ggcPrintSave[16][32];

/************** methods for CMain ***************************/
CMain* CMain::Instance()
{
	static CMain instance;
	return &instance;
}

CMain::CMain():HighLightGame(RETROSNAKE),HighLightGameSave(MAIN)
{
	
}

void CMain::Enter(CPlayer*player)
{
	CLS;
#ifdef WIN32
	cout << "Back to Main Menu!\n";
#endif
	HighLightGame = RETROSNAKE;
	HighLightGameSave = MAIN;
	CP_SLEEP(500);
	m_bPrintFlag = false;
}

void CMain::Execute(CPlayer*player)
{
	if (HighLightGame != HighLightGameSave)
	{
#ifdef WIN32
		//ensure the text will print only once
		if (!m_bPrintFlag)
		{
			CLS;
			cout << "Choose the game:\n  RetroSnake\n  Gobang\n  Tetris\n  Sokoban\n  StarTrek";
			m_bPrintFlag = true;
		}

		//should't add blank when first executing
		else
		{
			CursorMoveTo(0, HighLightGameSave);
			cout << "  ";
		}

		//use arrow to higlight the choioce
		CursorMoveTo(0, HighLightGame);
		cout << "¡ú";
#endif
		HighLightGameSave = HighLightGame;
	}
	switch (PressKey())
	{
	case LEFT:
	case UP:
	case LEFT2:
	case UP2:
		HighLightGame = Game_Name(HighLightGame - 1);
		
		if (HighLightGame <= MAIN) HighLightGame = Game_Name(int(END_LABEL) - 1);
		break;
	case RIGHT2:
	case DOWN2:
	case RIGHT:
	case DOWN:
		HighLightGame = Game_Name(HighLightGame + 1);
		if (HighLightGame >= END_LABEL) HighLightGame = Game_Name(1);
		break;
	case OK:
	case START:
		player->ChangeState(GetInstance(HighLightGame));
	default:
		//avoid high CPU occupation
		Sleep(1);
		break;
	}
	
}

void CMain::Exit(CPlayer*player)
{
	
}




