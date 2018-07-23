
#include"PlayerOwnedGames.h"
#include"Player.h"



#ifdef _WIN32
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

CMain::CMain():m_HighLightGame(RETROSNAKE),m_HighLightGameSave(MAIN)
{
	
}

void CMain::Enter(CPlayer*player)
{
	CLS;
#ifdef _WIN32
	cout << "Back to Main Menu!\n";
#endif
	m_HighLightGame = RETROSNAKE;
	m_HighLightGameSave = MAIN;
	CP_SLEEP(500);
	m_bPrintFlag = false;
}

void CMain::Execute(CPlayer*player)
{
	if (m_HighLightGame != m_HighLightGameSave)
	{
#ifdef _WIN32
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
			MoveCursor(0, m_HighLightGameSave);
			cout << "  ";
		}

		//use arrow to higlight the choioce
		MoveCursor(0, m_HighLightGame);
		cout << "¡ú";
#endif
		m_HighLightGameSave = m_HighLightGame;
	}
	switch (PressKey())
	{
	case LEFT:
	case UP:
	case LEFT2:
	case UP2:
		m_HighLightGame = Game_Name(m_HighLightGame - 1);
		
		if (m_HighLightGame <= MAIN) m_HighLightGame = Game_Name(int(END_LABEL) - 1);
		break;
	case RIGHT2:
	case DOWN2:
	case RIGHT:
	case DOWN:
		m_HighLightGame = Game_Name(m_HighLightGame + 1);
		if (m_HighLightGame >= END_LABEL) m_HighLightGame = Game_Name(1);
		break;
	case OK:
	case START:
		player->ChangeState(GetInstance(m_HighLightGame));
	default:
		//avoid high CPU occupation
		Sleep(1);
		break;
	}
	
}

void CMain::Exit(CPlayer*player)
{
	
}




