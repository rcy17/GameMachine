
#include"PlayerOwnedGames.h"
#include"Player.h"





#ifdef _WIN32
#pragma warning(disable:4996)
#include<conio.h>
#include<iostream>
using std::cout;
#endif

//here are some assignments for const variables

//the refresh time for deferent levels
const char CStartrek::m_cRefreshTruns[6] = { 20,18,15,12,9,6 };

//the Max y-coordinate
const char CStartrek::m_cYMax = 14;

//the Min y-coordinate
const char CStartrek::m_cYMin = 1;

//the x range
const char CStartrek::m_cXEdge = 20;

//the delay time between two frame
const char CStartrek::m_cDelay = 50;

CStartrek* CStartrek::Instance()
{
	static CStartrek instance;
	return &instance;
}

void CStartrek::Enter(CPlayer*player)
{
	CLS;
	Initialize();
}

void CStartrek::Execute(CPlayer*player)
{
#ifdef DEBUG_STARTREK
	if (!m_iCount)
		Initialize();
#endif
	//if the game start then work
	if (m_bStart)
	{
		if (m_bPause)
		{
			switch (PressKey())
			{
			case PAUSE:
			case START:
				m_bPause = false;
				break;
			case BACK:
			case RESET:
				player->ChangeState(CStartrek::Instance());
				return;
			case RESTART:
				Initialize();
				return;
			case EXIT:
				player->ChangeState(CMain::Instance());
				return;
			default:
				//avoid high CPU occupation
				Sleep(1);
				return;
			}
		}

		//calculate this frame's finish time
		auto start = CP_CLOCK;
		auto finish = CP_CLOCK + m_cDelay;
		Refresh();
		keyin tem = NOOPERATION;

		//wait until press any key or time out
		while (!(tem = PressKey()) && CP_CLOCK < finish)
		{
			//avoid high CPU occupation
			Sleep(1);
		}
		if (tem)
			switch (tem)
			{
			case LEFT:
			case LEFT2:
				if (m_cY < m_cYMax - 1)
					m_cY++;
				break;
			case RIGHT:
			case RIGHT2:
				if (m_cY > m_cYMin + 1)
					m_cY--;
				break;
			case DOWN:
			case DOWN2:
				Refresh();
				break;
			case PAUSE:
				m_bPause = true;
				return;
			case EXIT:
			case BACK:
				player->ChangeState(CMain::Instance());
				return;
			case RESTART:
			case RESET:
				Enter(player);
				return;
			}

		//if the time isn't time, then wait
		int RefreshTime = finish - CP_CLOCK;
		if (RefreshTime < 1)
			RefreshTime = 1;
		CP_SLEEP(RefreshTime);
	}
	else if (m_bOver)
	{
		switch (PressKey())
		{
		case EXIT:
		case BACK:
			player->ChangeState(CMain::Instance());
			return;
		case RESTART:
		case RESET:
			Enter(player);
			return;
		default:
			//avoid high CPU occupation
			Sleep(1);
			return;
		}
	}

	//press any key to start
	else
	{
		m_bStart = true;
		switch (PressKey())
		{
		case UP:
		case UP2:
		case DOWN:
		case DOWN2:
			Refresh();
			break;
		case LEFT:
		case LEFT2:
			m_cY++;
			break;
		case RIGHT:
		case RIGHT2:
			m_cY--;
			break;
		case PAUSE:
			m_bPause = true;
			return;
		case EXIT:
		case BACK:
			player->ChangeState(CMain::Instance());
			return;
		case RESTART:
		case RESET:
			Enter(player);
			return;
		default:
			//avoid high CPU occupation
			Sleep(1);
			m_bStart = false;
			break;
		}
	}
}

void CStartrek::Exit(CPlayer*player)
{

}

CStartrek::CStartrek()
{

}

//initialize the game
void CStartrek::Initialize()
{
	memset(g_ggcPrint, BLANK, sizeof(g_ggcPrint));
	memset(g_ggcPrintSave, BLANK, sizeof(g_ggcPrintSave));
	m_iScore = 0;
	m_cY = iYEDGE / 2;
	m_bOver = false;
	m_iCount = 0;
	m_bStart = 0;
	m_cLevel = 0;
	for (int i = 0; i <= m_cXEdge; i++)
		g_ggcPrint[m_cYMin][i] = g_ggcPrint[m_cYMax][i] = EDGE;
	for (int i = m_cYMin; i < m_cYMax; i++)
		g_ggcPrint[i][0] = g_ggcPrint[i][m_cXEdge] = EDGE;
#ifdef _WIN32
	MoveCursor((m_cYMax + m_cYMin) / 2 + 1, m_cXEdge + 3);
	cout << m_iScore;
#endif
	Refresh();
}

//refresh the screen every frame
void CStartrek::Refresh()
{
	//clear the bullets in last frame
	for (char i = m_cYMin + 1; i < m_cYMax; i++)
	{
		if (g_ggcPrint[i][m_cXEdge - 2] == FIGHTER)
		{
			for (char j = m_cXEdge - 1; g_ggcPrint[i][j] != BLOCK && j > 0; j--)
				g_ggcPrint[i][j] = BLANK;
			if (g_ggcPrint[i - 1][m_cXEdge - 1] != EDGE)
				g_ggcPrint[i - 1][m_cXEdge - 1] = BLANK;
			if (g_ggcPrint[i + 1][m_cXEdge - 1] != EDGE)
				g_ggcPrint[i + 1][m_cXEdge - 1] = BLANK;
			break;
		}
	}

	//refresh the bullet as if it could move forward
	if (m_bStart)
	{
		for (char m_cLabel = m_cXEdge - 1; m_cLabel > 0; m_cLabel--)
		{
			//refresh the bullet until there is a block
			if (g_ggcPrint[m_cY][m_cLabel] == BLOCK)
			{
				g_ggcPrint[m_cY][m_cLabel] = BULLET;

				//shot one block per frame
				m_iScore++;
#ifdef _WIN32
				MoveCursor((m_cYMax + m_cYMin) / 2 + 1, m_cXEdge + 3);
				cout << m_iScore;
#endif
				break;
			}
			if (m_iCount % 2 == m_cLabel % 2)
				g_ggcPrint[m_cY][m_cLabel] = BULLET;
		}
	}

	//refresh the blocks at duing time
	if (!(m_iCount % m_cRefreshTruns[m_cLevel]))
	{
		//fall the block
		for (char i = m_cYMin + 1; i < m_cYMax; i++)
			memmove((g_ggcPrint[i] + 2), (g_ggcPrint[i] + 1), sizeof(g_ggcPrint[0][0]) * (m_cXEdge - 3));

		//create new block half of posibility
		for (char i = m_cYMin + 1; i < m_cYMax; i++)
			if (rand() % 2)
				g_ggcPrint[i][1] = BLOCK;
			else
				g_ggcPrint[i][1] = BLANK;

		//if the block go into the bottom tow rows, game over
		for (char i = m_cYMin + 1; i < m_cYMax; i++)
			if (g_ggcPrint[i][m_cXEdge - 2] == BLOCK)
			{
#ifdef DEBUG_STARTREK
				g_ggcPrint[i][m_cXEdge - 2] = BLOCK;
				Print(STARTREK);
				memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
#endif
				m_bOver = true;
				m_bStart = false;
				return;
			}
	}

	//refresh the fighter
	g_ggcPrint[m_cY][m_cXEdge - 2] = FIGHTER;
	g_ggcPrint[m_cY][m_cXEdge - 1] = FIGHTER;
	if (g_ggcPrint[m_cY - 1][m_cXEdge - 1] != EDGE)
		g_ggcPrint[m_cY - 1][m_cXEdge - 1] = FIGHTER;
	if (g_ggcPrint[m_cY + 1][m_cXEdge - 1] != EDGE)
		g_ggcPrint[m_cY + 1][m_cXEdge - 1] = FIGHTER;

	Print(STARTREK);
	memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
	m_iCount++;
	
	//speed up after the duing time
	if (m_iCount > m_cLevel * 200 && m_cLevel < 5)
		m_cLevel++;

}