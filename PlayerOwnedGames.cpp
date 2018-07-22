
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

void CMain::Enter(CPlayer*player)
{
	CLS;
#ifdef WIN32
	cout << "Back to Main Menu!\n";
#endif
	HighLightGame = RETROSNAKE;
	HighLightGameSave = NONE;
	CP_SLEEP(500);
}

void CMain::Execute(CPlayer*player)
{
	if (HighLightGame != HighLightGameSave)
	{


		CLS;
#ifdef WIN32
		cout << "Choose the game:\n1.RetroSnake\n2.Gobang\n3.Tetris\n4.Sokoban\n";
		cout << "The highlight one is:" << HighLightGame;
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
		
		if (HighLightGame <= NONE) HighLightGame = Game_Name(int(END_LABEL) - 1);
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
		break;
	}
}

void CMain::Exit(CPlayer*player)
{
	
}




