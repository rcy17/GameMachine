
#include"PlayerOwnedGames.h"
#include"State.h"
#include"Player.h"
#include"Compatible.h"




#ifdef WIN32
#pragma warning(disable:4996)
#include<conio.h>
#include<iostream>
using std::cout;
#endif

char CStartrek::m_cRefreshTruns[6] = { 20,18,15,12,9,6 };

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
				return;
			}
		}
		auto start = CP_CLOCK;
		auto finish = CP_CLOCK + m_cDelay;
		Refresh();
		keyin tem = NOOPERATION;
		while (!(tem = PressKey())&&CP_CLOCK < finish)
		{

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
			return;
		}
	}
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
	m_cYMax = 14;
	m_cYMin = 1;
	m_cXEdge = 20;
	m_cDelay = 50;
}

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
#ifdef WIN32
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 4,26 };
	SetConsoleCursorPosition(hout, coord);
	cout << m_iScore;

#endif
	Refresh();
}

void CStartrek::Refresh()
{
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

	//char m_cLabel;
	if (m_bStart)
	{
		for (char m_cLabel = m_cXEdge - 1; m_cLabel > 0; m_cLabel--)
		{
			if (g_ggcPrint[m_cY][m_cLabel] == BLOCK)
			{
				g_ggcPrint[m_cY][m_cLabel] = BULLET;
				m_iScore++;
#ifdef WIN32
				HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
				COORD coord = { 4,26 };
				SetConsoleCursorPosition(hout, coord);
				cout << m_iScore;
#endif
				break;
			}
			if (m_iCount % 2 == m_cLabel % 2)
				g_ggcPrint[m_cY][m_cLabel] = BULLET;
		}
	}
	if (!(m_iCount % m_cRefreshTruns[m_cLevel]))
	{

		for (char i = m_cYMin + 1; i < m_cYMax; i++)
			for (char j = m_cXEdge-3; j > 0; j--)
				g_ggcPrint[i][j + 1] = g_ggcPrint[i][j];
			//memmove((g_ggcPrint[i] + 2), (g_ggcPrint[i] + 1), sizeof(g_ggcPrint[0][0]) * (m_cXEdge - 3));
		for (char i = m_cYMin + 1; i < m_cYMax; i++)
			if (rand() % 2)
				g_ggcPrint[i][1] = BLOCK;
			else
				g_ggcPrint[i][1] = BLANK;
		for (char i = m_cYMin + 1; i < m_cYMax; i++)
			if (g_ggcPrint[i][m_cXEdge - 2] == BLOCK)
			{
#ifdef DEBUG_STARTREK
				g_ggcPrint[i][m_cXEdge - 2] = BLOCK;
				Print(STARTREK, g_ggcPrint, g_ggcPrintSave);
				memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
#endif
				m_bOver = true;
				m_bStart = false;
				return;
			}
	}
	g_ggcPrint[m_cY][m_cXEdge - 2] = FIGHTER;
	g_ggcPrint[m_cY][m_cXEdge - 1] = FIGHTER;
	if (g_ggcPrint[m_cY - 1][m_cXEdge - 1] != EDGE)
		g_ggcPrint[m_cY - 1][m_cXEdge - 1] = FIGHTER;
	if (g_ggcPrint[m_cY + 1][m_cXEdge - 1] != EDGE)
		g_ggcPrint[m_cY + 1][m_cXEdge - 1] = FIGHTER;

	Print(STARTREK, g_ggcPrint, g_ggcPrintSave);
	memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
	m_iCount++;
	if (m_iCount > m_cLevel * 200 && m_cLevel < 5)
		m_cLevel++;

}