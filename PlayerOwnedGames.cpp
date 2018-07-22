#ifndef ARDUINO
#include"PlayerOwnedGames.h"
#include"State.h"
#include"Player.h"
#include"Compatible.h"
#endif



#ifdef WINDOWS_SYSTEM
#include<conio.h>
#include<iostream>
#pragma warning(disable:4996)
using std::cout;
#endif

/************** methods for CMain ***************************/
CMain* CMain::Instance()
{
	static CMain instance;
	return &instance;
}

void CMain::Enter(CPlayer*player)
{
	CLS;
	//MusicManage(NOMUSIC);
#ifdef WINDOWS_SYSTEM
	cout << "Back to Main Menu!\n";
#else
	print_backtomenu();
#endif
	HighLightGame = RETROSNAKE;
	HighLightGameSave = NONE;
	CP_SLEEP(500);
}

void CMain::Execute(CPlayer*player)
{
	//MusicManage(BGM);
	if (HighLightGame != HighLightGameSave)
	{


		CLS;
#ifdef WINDOWS_SYSTEM
		cout << "Choose the game:\n1.RetroSnake\n2.Gobang\n3.Tetris\n4.Sokoban\n";
		cout << "The highlight one is:" << HighLightGame;
#else 
		switch (HighLightGame)
		{
		case RETROSNAKE:
			print_retrosnake();
			break;
		case GOBANG:
			print_gobang();
			
			break;
		case TETRIS:
			print_tetris();
			break;
		case SOKOBAN:
			print_sokoban();
			break;
		case STARTREK:
			print_startrek();
			break;
	}
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
		player->ChangeState(HighLightGame);
	default:
		break;
	}
}

void CMain::Exit(CPlayer*player)
{
	//MusicManage(NOMUSIC);
}




