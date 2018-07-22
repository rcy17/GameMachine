
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



CGobang* CGobang::Instance()
{
	static CGobang instance;
	return &instance;
}

void CGobang::Enter(CPlayer*player)
{	

	memset(g_ggcPrint, BLANK, sizeof(g_ggcPrint));
	memset(g_ggcPrintSave, BLANK, sizeof(g_ggcPrintSave));
	m_bOver = false;
	m_bWin = false;
	m_bWhiteGo = false;
	//m_iTurn = 0;
	m_HighLight = CPosition(iXEDGE / 2, iYEDGE / 2);
	m_iBlankRest = iXEDGE * iYEDGE;
	g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] = HIGHLIGHT;
	CLS;
	Refresh();
}

void CGobang::Execute(CPlayer*player)
{
	auto start = CP_CLOCK;
	auto finish = start + REFRESHTIME;
#ifdef DEBUG_GOBANG
	static bool sm_bFlag = 1;
	if (sm_bFlag || m_bOver)
	{
		Enter(player);
		sm_bFlag = 0;
	}
#endif
	if (!m_bOver)
	{

		keyin tem=NOOPERATION;
		while (CP_CLOCK < finish && !(tem = PressKey()))
		{

		}
		if (tem)
		{
			switch (tem)
			{
			case RIGHT:
			case LEFT:
			case UP:
			case DOWN:
			case RIGHT2:
			case LEFT2:
			case UP2:
			case DOWN2:
				m_HighLightBefore = m_HighLight;
				m_HighLight.Move(tem).Safetify();
				break;
			case OK:
				JudgeOver();
				break;
			case RESET:
			case RESTART:
				player->ChangeState(CGobang::Instance());
				return;
			case EXIT:
				player->ChangeState(CMain::Instance());
				break;

			}
			Refresh();
		}
			
	}
	else
	{
		keyin tem=NOOPERATION;
		while (CP_CLOCK < finish && !(tem = PressKey()))
		{

		}
		switch (tem)
		{
		case RESET:
		case RESTART:
			player->ChangeState(CGobang::Instance());
			break;
		case EXIT:
			player->ChangeState(CMain::Instance());
			break;
		}
	}
	int RefreshTime = finish - CP_CLOCK;
	if (RefreshTime < 1)
		RefreshTime = 1;
	CP_SLEEP(RefreshTime);
}

void CGobang::Exit(CPlayer*player)
{

}

CGobang::CGobang()
{

}

bool CGobang::GoCheck()
{
	m_HighLight.Safetify();
	return g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] == BLANK ||
		g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] == HIGHLIGHT;

}

void CGobang::JudgeOver()
{
	item Now = GetItem();
	if (g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] != HIGHLIGHT) return;
	g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] = Now;
	m_iBlankRest--;
	if (CountChess(Now, UP, DOWN) >= 5 || CountChess(Now, RIGHT, LEFT) >= 5 ||
		CountChess(Now, RIGHTUP, LEFTDOWN) >= 5 || CountChess(Now, LEFTUP, RIGHTDOWN) >= 5)
	{
		m_bWin = true;
		m_bOver = true;
		return;
	}
	if (!m_iBlankRest)
	{
		m_bOver = true;
		return;
	}

	/*if (m_bWhiteGo)
		m_iTurn++;*/
	m_bWhiteGo = !m_bWhiteGo;
	/*int random = rand() % m_iBlankRest + 1;
	for (int i = 0; i < iYEDGE; i++)
		for (int j = 0; j < iXEDGE; j++)
		{
			if (g_ggcPrint[i][j] == BLANK)
				random--;
			if (!random)
			{
				m_HighLight = CPosition(j, i);
				return;
			}
		}*/
}

item CGobang::GetItem()
{
	return m_bWhiteGo ? WHITECHESS : BLACKCHESS;
}

int CGobang::CountChess(const item &Now, const keyin &Direction1, const keyin &Direction2)
{
	int Count = 0;
	CPosition tem = m_HighLight.Move(Direction1);
	while (tem.SafeCheck() && g_ggcPrint[tem.Get_Y()][tem.Get_X()] == Now)
	{
		Count++;
		tem.Move(Direction1);
	}
	tem = m_HighLight.Move(Direction2);
	while (tem.SafeCheck() && g_ggcPrint[tem.Get_Y()][tem.Get_X()] == Now)
	{
		Count++;
		tem.Move(Direction2);
	}
	return Count;
}

void CGobang::Refresh()
{
	//memset(g_ggcPrint, BLANK, sizeof(g_ggcPrint));
	if (m_HighLight != m_HighLightBefore)
	{
		switch (g_ggcPrint[m_HighLightBefore.Get_Y()][m_HighLightBefore.Get_X()])
		{
		case BLACKHIGHLIGHT:
			g_ggcPrint[m_HighLightBefore.Get_Y()][m_HighLightBefore.Get_X()] = BLACKCHESS;
			break;
		case WHITEHIGHLIGHT:
			g_ggcPrint[m_HighLightBefore.Get_Y()][m_HighLightBefore.Get_X()] = WHITECHESS;
			break;
		case HIGHLIGHT:
			g_ggcPrint[m_HighLightBefore.Get_Y()][m_HighLightBefore.Get_X()] = BLANK;
			break;
		default:
			break;
		}
		switch (g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()])
		{
		case BLACKCHESS:
			g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] = BLACKHIGHLIGHT;
			break;
		case WHITECHESS:
			g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] = WHITEHIGHLIGHT;
			break;
		case BLANK:
			g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] = HIGHLIGHT;
			break;
		default:
			break;
		}
	}
	Print(GOBANG, g_ggcPrint, g_ggcPrintSave);
	memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
}
