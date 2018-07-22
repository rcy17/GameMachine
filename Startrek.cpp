#ifndef ARDUINO
#include"PlayerOwnedGames.h"
#include"State.h"
#include"Player.h"
#include"Compatible.h"
#endif



#ifdef WINDOWS_SYSTEM
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
				player->ChangeState(TETRIS);
				return;
			case RESTART:
				Initialize();
				return;
			case EXIT:
				player->ChangeState(NONE);
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
				player->ChangeState(NONE);
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
			player->ChangeState(NONE);
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
			player->ChangeState(NONE);
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
	memset(m_ggiPrint, BLANK, sizeof(m_ggiPrint));
	memset(m_ggiPrintSave, BLANK, sizeof(m_ggiPrintSave));
	m_iScore = 0;
	m_cY = iYEDGE / 2;
	m_bOver = false;
	m_iCount = 0;
	m_bStart = 0;
	m_cLevel = 0;
	for (int i = 0; i <= m_cXEdge; i++)
		m_ggiPrint[m_cYMin][i] = m_ggiPrint[m_cYMax][i] = EDGE;
	for (int i = m_cYMin; i < m_cYMax; i++)
		m_ggiPrint[i][0] = m_ggiPrint[i][m_cXEdge] = EDGE;
#ifdef WINDOWS_SYSTEM
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 4,26 };
	SetConsoleCursorPosition(hout, coord);
	cout << m_iScore;
#else

	print_score(m_iScore);

#endif
	Refresh();
}

void CStartrek::Refresh()
{
	for (char i = m_cYMin + 1; i < m_cYMax; i++)
	{
		if (m_ggiPrint[i][m_cXEdge - 2] == FIGHTER)
		{
			for (char j = m_cXEdge - 1; m_ggiPrint[i][j] != BLOCK && j > 0; j--)
				m_ggiPrint[i][j] = BLANK;
			if (m_ggiPrint[i - 1][m_cXEdge - 1] != EDGE)
				m_ggiPrint[i - 1][m_cXEdge - 1] = BLANK;
			if (m_ggiPrint[i + 1][m_cXEdge - 1] != EDGE)
				m_ggiPrint[i + 1][m_cXEdge - 1] = BLANK;
			break;
		}
	}

	//char m_cLabel;
	if (m_bStart)
	{
		for (char m_cLabel = m_cXEdge - 1; m_cLabel > 0; m_cLabel--)
		{
			if (m_ggiPrint[m_cY][m_cLabel] == BLOCK)
			{
				m_ggiPrint[m_cY][m_cLabel] = BULLET;
				m_iScore++;
#ifdef WINDOWS_SYSTEM
				HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
				COORD coord = { 4,26 };
				SetConsoleCursorPosition(hout, coord);
				cout << m_iScore;
#else
				print_score(m_iScore);
#endif
				break;
			}
			if (m_iCount % 2 == m_cLabel % 2)
				m_ggiPrint[m_cY][m_cLabel] = BULLET;
		}
	}
	if (!(m_iCount % m_cRefreshTruns[m_cLevel]))
	{

		for (char i = m_cYMin + 1; i < m_cYMax; i++)
			for (char j = m_cXEdge-3; j > 0; j--)
				m_ggiPrint[i][j + 1] = m_ggiPrint[i][j];
			//memmove((m_ggiPrint[i] + 2), (m_ggiPrint[i] + 1), sizeof(m_ggiPrint[0][0]) * (m_cXEdge - 3));
		for (char i = m_cYMin + 1; i < m_cYMax; i++)
			if (rand() % 2)
				m_ggiPrint[i][1] = BLOCK;
			else
				m_ggiPrint[i][1] = BLANK;
		for (char i = m_cYMin + 1; i < m_cYMax; i++)
			if (m_ggiPrint[i][m_cXEdge - 2] == BLOCK)
			{
#ifdef DEBUG_STARTREK
				m_ggiPrint[i][m_cXEdge - 2] = BLOCK;
				Print(STARTREK, m_ggiPrint, m_ggiPrintSave);
				memcpy(m_ggiPrintSave, m_ggiPrint, sizeof(m_ggiPrint));
#endif
				m_bOver = true;
				m_bStart = false;
				return;
			}
	}
	m_ggiPrint[m_cY][m_cXEdge - 2] = FIGHTER;
	m_ggiPrint[m_cY][m_cXEdge - 1] = FIGHTER;
	if (m_ggiPrint[m_cY - 1][m_cXEdge - 1] != EDGE)
		m_ggiPrint[m_cY - 1][m_cXEdge - 1] = FIGHTER;
	if (m_ggiPrint[m_cY + 1][m_cXEdge - 1] != EDGE)
		m_ggiPrint[m_cY + 1][m_cXEdge - 1] = FIGHTER;

	Print(STARTREK, m_ggiPrint, m_ggiPrintSave);
	memcpy(m_ggiPrintSave, m_ggiPrint, sizeof(m_ggiPrint));
	m_iCount++;
	if (m_iCount > m_cLevel * 200 && m_cLevel < 5)
		m_cLevel++;

}