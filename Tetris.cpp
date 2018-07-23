
#include"PlayerOwnedGames.h"
#include"Player.h"




#ifdef WIN32
#pragma warning(disable:4996)
#include<conio.h>
#include<iostream>
using std::cout;
#endif

enum blocklabel
{
	OSHAPE = 0,
	LSHAPE = 4,
	JSHAPE = 8,
	ZSHAPE = 10,
	SSHAPE = 12,
	TSHAPE = 16,
	ISHAPE = 18

};

//here are some assignments for const variables

//here saved all 19 shape states
const unsigned char sg_gcShapeAll[39] = { 0x10,0x54,  0x40,0x98,  0x12,0x40,  0x10,0x95,  0x62,0x45,
0x51,0x89,  0x40,0x65,  0x01,0x84,  0x10,0x62,  0x10,0x65,  0x51,0x84,  0x12,0x45,
0x40,0x95,  0x10,0x52,  0x51,0x94,  0x41,0x65,  0x40,0x85,  0x62,0xea,  0x10,0x32,  0x00 };

//here saved freshtimes for defferent speed
const short int sg_giFreshTime[5] = { 800,600,400,200,100 };


CTetris* CTetris::Instance()
{
	static CTetris Instance;
	return &Instance;
}

CTetris::CTetris()
{
	m_iXEdge = 21;
	m_iYEdge = 11;
}

void CTetris::Enter(CPlayer*player)
{
	memset(g_ggcPrint, BLANK, sizeof(g_ggcPrint));
	memset(g_ggcPrintSave, BLANK, sizeof(g_ggcPrintSave));
	m_bOver = false;
	m_bStart = false;
	m_bChangeFlag = false;
	m_bPrintFlag = true;
	m_cSpeed = 0;
	CLS;
#ifdef WIN32
	cout << "Speed: 0";
#endif
}

void CTetris::Execute(CPlayer*player)
{
#ifdef DEBUG_TETRIS
	static unsigned long long debugtest = 0;
	if (!debugtest)
	{
		m_cSpeed = 0;
		Initialize();
	}
	debugtest++;
#endif
	//after the game start, it works
	if (m_bStart)
	{
		//if press Pause, the game pauses
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
				player->ChangeState(CTetris::Instance());
				return;
			case RESTART:
				Initialize();
				return;
			case EXIT:
				player->ChangeState(CMain::Instance());
				break;
			default:
				//avoid high CPU occupation
				Sleep(1);
				return;
			}
		}

		//when game over, can't go on
		if (m_bOver)
		{
			switch (PressKey())
			{
			case EXIT:
				player->ChangeState(CMain::Instance());
				return;
			case RESET:
			case BACK:
				player->ChangeState(CTetris::Instance());
				return;
			case RESTART:
				Initialize();
				return;
			default:
				//avoid high CPU occupation
				Sleep(1);
				return;
			}
		}
		auto start = CP_CLOCK;
		auto finish = start + sg_giFreshTime[m_cSpeed];

		//refresh the new block
		if (m_bNewBlock)
		{
			for (int i = 0; i < 4; i++)
			{
				//location the new shape
				unsigned char s = GetLocation(sg_gcShapeAll + 2 * m_cShapeNow, i);
				CPosition p = CPosition(s / 4, s % 4) + m_Shape;
				if (g_ggcPrint[p.Get_Y()][p.Get_X()] == BLOCK)
				{
					m_bOver = true;
					return;
				}
				g_ggcPrint[p.Get_Y()][p.Get_X()] = BLOCK;
			}
			Print(TETRIS);
			memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
			m_bNewBlock = false;
		}
		while (CP_CLOCK < finish)
		{
			keyin tem = PressKey();
			switch (tem)
			{
			case RIGHT:
			case RIGHT2:
			case LEFT2:
			case LEFT:
			case UP2:
			case UP:
			case DOWN:
			case DOWN2:
				//move after player press the key
				if (SafeMove(tem))
				{
					Print(TETRIS);
					memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
					CP_SLEEP(20);
				}
				break;
			case PAUSE:
				m_bPause = true;
				return;
			case OK:
				//fall the shape immediately
				while (SafeMove(DOWN));
				break;
			case EXIT:
				player->ChangeState(CMain::Instance());
				return;
			case RESET:
			case BACK:
				player->ChangeState(CTetris::Instance());
				return;
			case RESTART:
				Initialize();
				return;

			}
			//limit the operation
			Sleep(50);
		}

		//if the shape move to the bottom, then the new shape comes
		if (!SafeMove(DOWN))
		{
			Clear();
			m_bNewBlock = true;
		}
		Refresh();

		int SleepTime = finish - CP_CLOCK;
		if (SleepTime < 1) SleepTime = 1;
		CP_SLEEP(SleepTime);

	}
	//option for speed
	else
	{
		auto start = CP_CLOCK;
		auto finish = start + REFRESHTIME;
		keyin tem;
		while (CP_CLOCK < finish && !(tem = PressKey()))
		{
			//avoid high CPU occupation
			Sleep(1);
		}
		if (tem)
		{
			m_bChangeFlag = true;
			switch (tem)
			{
			case DOWN:
			case DOWN2:
			case LEFT2:
			case LEFT:
				m_cSpeed--;
				if (m_cSpeed < 0)
					m_cSpeed = 4;
				break;
			case UP:
			case UP2:
			case RIGHT2:
			case RIGHT:
				m_cSpeed++;
				if (m_cSpeed > 4)
					m_cSpeed = 0;
				break;
			case CANCEL:
			case BACK:
			case EXIT:
				player->ChangeState(CMain::Instance());
				return;
			case OK:
			case START:
				Initialize();
				break;
			default:
				m_bChangeFlag = false;
				break;
			}
		}

		//refresh the print
		if (m_bChangeFlag)
		{
			if (m_bPrintFlag)
			{
				CLS;
#ifdef WIN32
				cout << "Speed:";
#endif
				m_bPrintFlag = false;
			}
#ifdef WIN32
			MoveCursor(7, 0);
			cout << (int)m_cSpeed;
#endif
			m_bChangeFlag = false;
		}
		int RefreshTime = finish - CP_CLOCK;
		if (RefreshTime < 1)
			RefreshTime = 1;
		CP_SLEEP(RefreshTime);
	}
}

void CTetris::Exit(CPlayer*player)
{

}

//initialize the game
void CTetris::Initialize()
{
	CLS;
	m_iScore = 0;
#ifdef WIN32
	MoveCursor(12, 18);
	cout << "Score";
	MoveCursor(13, 19);
	cout << 0;
#endif
	memset(g_ggcPrint, BLANK, sizeof(g_ggcPrint));
	memset(g_ggcPrintSave, BLANK, sizeof(g_ggcPrintSave));
	for (char i = 0; i <= m_iXEdge; i++)
	{
		g_ggcPrint[m_iYEdge][i] = EDGE;
	}
	for (char i = 0; i <= m_iXEdge; i++)
	{
		g_ggcPrint[0][i] = EDGE;
	}
	for (char i = 0; i < m_iYEdge; i++)
	{
		g_ggcPrint[i][m_iXEdge] = EDGE;
	}
	for (char i = 0; i < m_iYEdge; i++)
	{
		g_ggcPrint[i][0] = EDGE;
	}

	m_bChangeFlag = false;
	m_bStart = true;
	m_bOver = false;
	m_bNewBlock = true;
	m_bPause = false;
	m_cShapeNext = rand() % 19;
	m_Shape = CPosition(1, m_iYEdge / 2 - 1);
	Refresh();
}

//clear the row when it's full
void CTetris::Clear()
{
	char x = m_Shape.Get_X();
	char count = 0;
	char Label;

	//count how man row can be clear
	for (char i = x; i < x + 4 && i < m_iXEdge; i++)
	{
		bool flag = true;
		for (char j = 1; j < m_iYEdge; j++)
			if (g_ggcPrint[j][i] != BLOCK)
			{
				flag = false;
				break;
			}
		if (flag)
		{
			if (!count)
				Label = i;
			count++;
		}
		else if (count)
			break;
	}

	//if there is any row to clear
	if (count)
	{
		for (char i = Label; i < Label + count; i++)
		{
			for (char j = 1; j < m_iYEdge; j++)
				g_ggcPrint[j][i] = BLANK;
			m_iScore += i - Label + 1;
		}
#ifdef WIN32
		MoveCursor(13, 19);
		cout << m_iScore;
#endif
		Print(TETRIS);
		memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));

		//let the player see the blank rows
		CP_SLEEP(20);

		//then fall the block above the blanks
		for (char i = Label + count - 1; i >= count + 1; i--)
			for (char j = 1; j < m_iYEdge; j++)
				g_ggcPrint[j][i] = g_ggcPrint[j][i - count];
		for (char i = count; i >= 1; i--)
			for (char j = 1; j < m_iYEdge; j++)
				g_ggcPrint[j][i] = BLANK;
		if (m_cSpeed < 4 && m_iScore >= 100 * (m_cSpeed + 1))
			m_cSpeed++;
		CP_SLEEP(10);
	}

}

//refresh the screen every frame
void CTetris::Refresh()
{
	if (m_bNewBlock)
	{
		//m_bNewBlock = false;
		m_cShapeNow = m_cShapeNext;
		m_cShapeNext = rand() % 19;
		m_Shape = CPosition(1, m_iYEdge / 2 - 1);
		for (char i = 12; i < 16; i++)
			for (char j = 3; j < 7; j++)
				g_ggcPrint[i][j] = BLANK;
		for (int i = 0; i < 4; i++)
		{
			unsigned char tem = GetLocation(sg_gcShapeAll + 2 * m_cShapeNext, i);
			g_ggcPrint[12 + tem % 4][3 + tem / 4] = BLOCK;
		}
	}
	Print(TETRIS);
	memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
}

//try to move the block, return true if move successfully 
bool CTetris::SafeMove(const keyin&key)
{
	//get the relative position of the shape
	unsigned char s[4] = { GetLocation(sg_gcShapeAll + 2 * m_cShapeNow, 0) ,
						   GetLocation(sg_gcShapeAll + 2 * m_cShapeNow,1) ,
						   GetLocation(sg_gcShapeAll + 2 * m_cShapeNow,2) ,
						   GetLocation(sg_gcShapeAll + 2 * m_cShapeNow,3) };

	//get the position of the leftup of the shpae
	unsigned char tem_x = m_Shape.Get_X(), tem_y = m_Shape.Get_Y(), tem_rotate = 0;

	//get the absolute position of the shape
	CPosition Judge[4] = { CPosition(s[0] / 4 + tem_x,s[0] % 4 + tem_y),
		CPosition(s[1] / 4 + tem_x,s[1] % 4 + tem_y) ,
		CPosition(s[2] / 4 + tem_x,s[2] % 4 + tem_y),
		CPosition(s[3] / 4 + tem_x,s[3] % 4 + tem_y) };

	CPosition p[4], Judgetem;
	memcpy(p, Judge, sizeof(Judge));
	
	//try to move alone the direction, judeg if shape will reach the edge
	switch (key)
	{
	case DOWN:
	case DOWN2:
		m_Shape.Move(RIGHT, Judgetem);
		for (char i = 0; i < 4; i++)
		{
			if (!p[i].Move(RIGHT).SafeCheck(m_iXEdge, m_iYEdge, 1, 1))
				return false;
		}
		break;
	case LEFT:
	case LEFT2:
		m_Shape.Move(UP, Judgetem);
		for (char i = 0; i < 4; i++)
		{
			if (!p[i].Move(UP).SafeCheck(m_iXEdge, m_iYEdge, 1, 1))
				return false;
		}
		break;
	case RIGHT:
	case RIGHT2:
		m_Shape.Move(DOWN, Judgetem);
		for (char i = 0; i < 4; i++)
		{
			if (!p[i].Move(DOWN).SafeCheck(m_iXEdge, m_iYEdge, 1, 1))
				return false;
		}
		break;
	case UP:
	case UP2:
		//rotate the shape
		Judgetem = m_Shape;
		switch (m_cShapeNow)
		{
		case OSHAPE:
			tem_rotate = OSHAPE;
			break;
		case LSHAPE:
			tem_rotate = 1 + OSHAPE;
			break;
		case JSHAPE:
			tem_rotate = 1 + LSHAPE;
			break;
		case ZSHAPE:
			tem_rotate = 1 + JSHAPE;
			break;
		case SSHAPE:
			tem_rotate = 1 + ZSHAPE;
			break;
		case TSHAPE:
			tem_rotate = 1 + SSHAPE;
			break;
		case ISHAPE:
			tem_rotate = 1 + TSHAPE;
			break;
		default:
			tem_rotate = m_cShapeNow + 1;
			break;
		}
		for (int i = 0; i < 4; i++)
		{
			s[i] = GetLocation(sg_gcShapeAll + 2 * tem_rotate, i);
			p[i] = CPosition(s[i] / 4 + tem_x, s[i] % 4 + tem_y);
			if (!p[i].SafeCheck(m_iXEdge, m_iYEdge, 1, 1))
				return false;
		}
		break;
	default:
		break;
	}

	//change the position of the shape
	for (char i = 0; i < 4; i++)
	{
		g_ggcPrint[Judge[i].Get_Y()][Judge[i].Get_X()] = BLANK;

	}
	for (char i = 0; i < 4; i++)
	{
		g_ggcPrint[p[i].Get_Y()][p[i].Get_X()] += BLOCK;

	}
	bool flag = true;

	//judge if there is overlapping block
	for (char i = 0; i < 4; i++)
	{
		if (g_ggcPrint[p[i].Get_Y()][p[i].Get_X()] != BLOCK)
		{
			flag = false;
			break;
		}
	}

	//if overlapping, turn back
	if (!flag)
	{
		for (char i = 0; i < 4; i++)
		{
			g_ggcPrint[p[i].Get_Y()][p[i].Get_X()] -= BLOCK;
			g_ggcPrint[Judge[i].Get_Y()][Judge[i].Get_X()] = BLOCK;
		}
		return false;
	}

	//after move, change the position
	m_Shape = Judgetem;

	//after rotating, the shape will change
	if (tem_rotate)
		m_cShapeNow = tem_rotate;
	return true;
}

//get the position number of a block by its storage value
unsigned char CTetris::GetLocation(const unsigned char*s, const char&Count)
{
	//save the shape's position with least memory
	unsigned char tem;
	switch (Count)
	{
	case 0:
		tem = (*s & 0x0fu);
		break;
	case 1:
		tem = int(*s);
		tem = *s >> 4;
		break;
	case 2:
		tem = (*(s + 1) & 0x0fu);
		break;
	case 3:
		tem = *(s + 1) >> 4;
		break;
	}
	return tem;
}

