#include"PlayerOwnedGames.h"
#include"Player.h"


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
#ifdef WIN32
	//clear the stack
	stack<CPosition>().swap(m_sChessSave);
#endif
	memset(g_ggcPrint, BLANK, sizeof(g_ggcPrint));
	memset(g_ggcPrintSave, BLANK, sizeof(g_ggcPrintSave));
	m_bOver = false;
	m_bWhiteGo = false;
	m_HighLight = CPosition(iXEDGE / 2, iYEDGE / 2);
	m_iBlankRest = iXEDGE * iYEDGE;
	g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] = HIGHLIGHT;
	CLS;
	Refresh();
}

void CGobang::Execute(CPlayer*player)
{
	//calculate this frame's end time
	auto start = CP_CLOCK;
	auto finish = start + REFRESHTIME;
#ifdef DEBUG_GOBANG
	static bool sm_bFlag = 1;
	if (sm_bFlag /*|| m_bOver*/)
	{
		Enter(player);
		sm_bFlag = 0;
	}
#endif

	//before game over, player can go chess
	if (!m_bOver)
	{
		keyin tem = NOOPERATION;
		while (CP_CLOCK < finish && !(tem = PressKey()))
		{
			//avoid high CPU occupation
			Sleep(1);
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
				//move the focus
				m_HighLightBefore = m_HighLight;
				m_HighLight.Move(tem).Safetify();
				break;
			case OK:
				//go chess
				JudgeOver();
				break;
			case RESET:
			case RESTART:
				//restart the game
				player->ChangeState(CGobang::Instance());
				return;
			case BACK:
			case EXIT:
				//back to the main menu
				player->ChangeState(CMain::Instance());
				break;
#ifdef  WIN32
			case CANCEL:
				//withdraw chess
				Withdraw();
				break;
#endif
			}
			//refresh the screen
			Refresh();
		}

	}

	//if the game is over, can't change the focus
	else
	{
		keyin tem = NOOPERATION;
		while (CP_CLOCK < finish && !(tem = PressKey()))
		{
			//avoid high CPU occupation
			Sleep(1);
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
#ifdef WIN32
		case CANCEL:
			//withdraw chess and go on playing
			Withdraw();
			m_bOver = false;
			Refresh();
			break;
#endif
		}
	}
	//if this frame's time isn't out, then wait
	int RefreshTime = finish - CP_CLOCK;
	if (RefreshTime < 1)
		RefreshTime = 1;
	CP_SLEEP(RefreshTime);
}

void CGobang::Exit(CPlayer*player)
{
#ifdef WIN32
	//free the memory
	stack<CPosition>().swap(m_sChessSave);
#endif
}

CGobang::CGobang()
{

}

//check if this position can put a chess
bool CGobang::GoCheck()
{
	//if the aim isn't chess, then player can go chess
	m_HighLight.Safetify();
	return g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] == BLANK ||
		g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] == HIGHLIGHT;

}

//judge if the game is over
void CGobang::JudgeOver()
{
	//know who is going
	item Now = GetItem();

	//only when the highlight isn't chess can player go chess
	if (g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] != HIGHLIGHT) return;
	g_ggcPrint[m_HighLight.Get_Y()][m_HighLight.Get_X()] = Now;
	m_iBlankRest--;
#ifdef WIN32
	m_sChessSave.push(m_HighLight);
#endif
	//Judge if player wins
	if (CountChess(Now, UP, DOWN) >= 5 || CountChess(Now, RIGHT, LEFT) >= 5 ||
		CountChess(Now, RIGHTUP, LEFTDOWN) >= 5 || CountChess(Now, LEFTUP, RIGHTDOWN) >= 5)
	{
		m_bOver = true;
		return;
	}

	//if there is no blank,it's a draw
	if (!m_iBlankRest)
	{
		m_bOver = true;
		return;
	}
	if (m_bWhiteGo)
		m_iTurn++;

	//exchange moving
	m_bWhiteGo = !m_bWhiteGo;
}

//get the chess which is going now
item CGobang::GetItem()
{
	return m_bWhiteGo ? WHITECHESS : BLACKCHESS;
}

//count the continuous chess in a line
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

//refresh the screen every turn
void CGobang::Refresh()
{
	//after focus's change, refresh the print
	if (m_HighLight != m_HighLightBefore)
	{
		//change back the last highlight
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

		//change the new highlight
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
	Print(GOBANG);
	memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
}

#ifdef WIN32
void CGobang::Withdraw()
{
	if (m_sChessSave.empty())
		return;
	CPosition tem = m_sChessSave.top();
	if (tem.Get_Y() == m_HighLight.Get_Y() && tem.Get_X() == m_HighLight.Get_X())
	{
		g_ggcPrint[tem.Get_Y()][tem.Get_X()] = HIGHLIGHT;
	}
	else
	{
		g_ggcPrint[tem.Get_Y()][tem.Get_X()] = BLANK;
	}
	m_bWhiteGo = !m_bWhiteGo;
	m_sChessSave.pop();
}
#endif