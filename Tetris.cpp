#ifndef ARDUINO
#include"PlayerOwnedGames.h"
#include"Player.h"
#include"Compatible.h"
#endif



#ifdef WINDOWS_SYSTEM
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

//here saved all 19 shape states
CP_GLOBALSTATE unsigned char sg_gcShapeAll[39] = { 0x10,0x54,  0x40,0x98,  0x12,0x40,  0x10,0x95,  0x62,0x45,
0x51,0x89,  0x40,0x65,  0x01,0x84,  0x10,0x62,  0x10,0x65,  0x51,0x84,  0x12,0x45,
0x40,0x95,  0x10,0x52,  0x51,0x94,  0x41,0x65,  0x40,0x85,  0x62,0xea,  0x10,0x32,  0x00 };

CP_GLOBALSTATE int sg_giFreshTime[5] = { 800,600,400,200,100 };


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
	memset(m_ggiPrint, BLANK, sizeof(m_ggiPrint));
	memset(m_ggiPrintSave, BLANK, sizeof(m_ggiPrintSave));
	m_bOver = false;
	m_bStart = false;
	m_bChangeFlag = false;
	m_cSpeed = 0;
	CLS;
#ifdef WINDOWS_SYSTEM
	cout << "    Speed\n        ";
	cout << (int)m_cSpeed;
#else
	print_speed(m_cSpeed);
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
				break;
			default:
				return;
			}
		}
		auto start = CP_CLOCK;
		auto finish = start + CP_GLOBAL(sg_giFreshTime[m_cSpeed]);

		if (m_bOver)
		{
			switch (PressKey())
			{
			case EXIT:
				player->ChangeState(NONE);
				return;
			case RESET:
			case BACK:
				player->ChangeState(TETRIS);
				return;
			case RESTART:
				Initialize();
				return;
			default:
				return;
			}
		}
		if (m_bNewBlock)
		{
			for (int i = 0; i < 4; i++)
			{
				char s = GetLocation(CP_GLOBAL(sg_gcShapeAll) + 2 * m_cShapeNow, i);
				CPosition p = CPosition(s / 4, s % 4) + m_Shape;
				if (m_ggiPrint[p.Get_Y()][p.Get_X()] == BLOCK)
				{
					m_bOver = true;
					return;
				}
				m_ggiPrint[p.Get_Y()][p.Get_X()] = BLOCK;
			}
			Print(TETRIS, m_ggiPrint, m_ggiPrintSave);
			memcpy(m_ggiPrintSave, m_ggiPrint, sizeof(m_ggiPrint));
			m_bNewBlock = false;
			//m_bStay = true;
		}
		keyin tem = NOOPERATION;
		while (CP_CLOCK < finish && !(tem = PressKey()))
		{

		}
		if (tem)
			switch (tem)
			{
			case RIGHT:
			case RIGHT2:
			case LEFT2:
			case LEFT:
			case UP2:
			case UP:
				if (SafeMove(tem))
				{
					Print(TETRIS, m_ggiPrint, m_ggiPrintSave);
					memcpy(m_ggiPrintSave, m_ggiPrint, sizeof(m_ggiPrint));
					CP_SLEEP(20);
				}
				break;
			case DOWN:
			case DOWN2:
				if (SafeMove(tem))
				{
					Print(TETRIS, m_ggiPrint, m_ggiPrintSave);
					memcpy(m_ggiPrintSave, m_ggiPrint, sizeof(m_ggiPrint));
					CP_SLEEP(CP_GLOBAL(sg_giFreshTime[4]));
				}
				return;
			case PAUSE:
				m_bPause = true;
				return;
			case OK:
				while (SafeMove(DOWN));
				break;
			case EXIT:
				player->ChangeState(NONE);
				return;
			case RESET:
			case BACK:
				player->ChangeState(TETRIS);
				return;
			case RESTART:
				Initialize();
				return;

			}
		if (!m_bStay && !SafeMove(DOWN))
		{
			Clear();
			m_bNewBlock = true;
		}
		//m_bStay = false;
		Refresh();

		int SleepTime = finish - CP_CLOCK;
		if (SleepTime < 1) SleepTime = 1;
		CP_SLEEP(SleepTime);

	}
	else
	{
		auto start = CP_CLOCK;
		auto finish = start + REFRESHTIME;
		keyin tem;
		while (CP_CLOCK < finish && !(tem = PressKey()))
		{

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
				player->ChangeState(NONE);
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
		if (m_bChangeFlag)
		{
			CLS;
#ifdef WINDOWS_SYSTEM
			cout << "    Speed\n        ";
			cout << (int)m_cSpeed;
#else
			print_speed(m_cSpeed);
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

void CTetris::Initialize()
{
	CLS;
	m_iScore = 0;
#ifdef WINDOWS_SYSTEM
	HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 12,18 };
	SetConsoleCursorPosition(hout, coord);
	cout << "Score";
	coord.X = 13;
	coord.Y = 19;
	SetConsoleCursorPosition(hout, coord);
	cout << 0;
#else
	print_score(m_iScore);
#endif
	memset(m_ggiPrint, BLANK, sizeof(m_ggiPrint));
	memset(m_ggiPrintSave, BLANK, sizeof(m_ggiPrintSave));
	for (char i = 0; i <= m_iXEdge; i++)
	{
		m_ggiPrint[m_iYEdge][i] = EDGE;
	}
	for (char i = 0; i <= m_iXEdge; i++)
	{
		m_ggiPrint[0][i] = EDGE;
	}
	for (char i = 0; i < m_iYEdge; i++)
	{
		m_ggiPrint[i][m_iXEdge] = EDGE;
	}
	for (char i = 0; i < m_iYEdge; i++)
	{
		m_ggiPrint[i][0] = EDGE;
	}
	//m_bStay = false;
	m_bChangeFlag = false;
	m_bStart = true;
	m_bOver = false;
	m_bNewBlock = true;
	m_bPause = false;
	m_cShapeNext = rand() % 19;
	m_Shape = CPosition(1, m_iYEdge / 2 - 1);
	Refresh();

}

void CTetris::Clear()
{
	char x = m_Shape.Get_X();
	char count = 0;
	char Label;
	for (char i = x; i < x + 4 && i < m_iXEdge; i++)
	{
		bool flag = true;
		for (char j = 1; j < m_iYEdge; j++)
			if (m_ggiPrint[j][i] != BLOCK)
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
	if (count)
	{
		for (char i = Label; i < Label + count; i++)
		{
			for (char j = 1; j < m_iYEdge; j++)
				m_ggiPrint[j][i] = BLANK;
			m_iScore += i - Label + 1;
		}
#ifdef WINDOWS_SYSTEM
		HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord = { 13,19 };
		SetConsoleCursorPosition(hout, coord);
		cout << m_iScore;
#else
		print_score(m_iScore);
#endif
		Print(TETRIS, m_ggiPrint, m_ggiPrintSave);
		memcpy(m_ggiPrintSave, m_ggiPrint, sizeof(m_ggiPrint));

		CP_SLEEP(20);
		for (char i = Label + count - 1; i >= count + 1; i--)
			for (char j = 1; j < m_iYEdge; j++)
				m_ggiPrint[j][i] = m_ggiPrint[j][i - count];
		for (char i = count; i >= 1; i--)
			for (char j = 1; j < m_iYEdge; j++)
				m_ggiPrint[j][i] = BLANK;
		if (m_cSpeed < 4 && m_iScore >= 100 * (m_cSpeed + 1))
			m_cSpeed++;
		CP_SLEEP(20);
		}

	}

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
				m_ggiPrint[i][j] = BLANK;
		for (int i = 0; i < 4; i++)
		{
			char tem = GetLocation(CP_GLOBAL(sg_gcShapeAll) + 2 * m_cShapeNext, i);
			m_ggiPrint[12 + tem % 4][3 + tem / 4] = BLOCK;
		}
	}
	Print(TETRIS, m_ggiPrint, m_ggiPrintSave);
	memcpy(m_ggiPrintSave, m_ggiPrint, sizeof(m_ggiPrint));
}

bool CTetris::SafeMove(const keyin&key)
{

	unsigned char s[4] = { GetLocation(CP_GLOBAL(sg_gcShapeAll) + 2 * m_cShapeNow, 0) ,
		GetLocation(CP_GLOBAL(sg_gcShapeAll) + 2 * m_cShapeNow,1) ,
		GetLocation(CP_GLOBAL(sg_gcShapeAll) + 2 * m_cShapeNow,2) ,
		GetLocation(CP_GLOBAL(sg_gcShapeAll) + 2 * m_cShapeNow,3) };
	unsigned char tem_x = m_Shape.Get_X(), tem_y = m_Shape.Get_Y(), tem_rotate = 0;
	CPosition Judge[4] = { CPosition(s[0] / 4 + tem_x,s[0] % 4 + tem_y),CPosition(s[1] / 4 + tem_x,s[1] % 4 + tem_y) ,
		CPosition(s[2] / 4 + tem_x,s[2] % 4 + tem_y),CPosition(s[3] / 4 + tem_x,s[3] % 4 + tem_y) };
	CPosition p[4], Judgetem;
	memcpy(p, Judge, sizeof(Judge));
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
			s[i] = GetLocation(CP_GLOBAL(sg_gcShapeAll) + 2 * tem_rotate, i);
			p[i] = CPosition(s[i] / 4 + tem_x, s[i] % 4 + tem_y);
			if (!p[i].SafeCheck(m_iXEdge, m_iYEdge, 1, 1))
				return false;
		}
		break;
	default:
		break;
	}
	for (char i = 0; i < 4; i++)
	{
		m_ggiPrint[Judge[i].Get_Y()][Judge[i].Get_X()] = BLANK;

	}
	for (char i = 0; i < 4; i++)
	{
		m_ggiPrint[p[i].Get_Y()][p[i].Get_X()] += BLOCK;

	}
	bool flag = true;
	for (char i = 0; i < 4; i++)
	{
		if (m_ggiPrint[p[i].Get_Y()][p[i].Get_X()] != BLOCK)
		{
			flag = false;
			break;
		}
	}
	if (!flag)
	{
		for (char i = 0; i < 4; i++)
		{
			m_ggiPrint[p[i].Get_Y()][p[i].Get_X()] -= BLOCK;
			m_ggiPrint[Judge[i].Get_Y()][Judge[i].Get_X()] = BLOCK;
		}
		return false;
	}
	m_Shape = Judgetem;
	if (tem_rotate)
		m_cShapeNow = tem_rotate;
	return true;
}

char CTetris::GetLocation(const unsigned char*s, const char&Count)
{
	char tem;
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

