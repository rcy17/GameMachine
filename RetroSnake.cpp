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

CRetroSnake* CRetroSnake::Instance()
{
	static CRetroSnake instance;
	return &instance;
}

CRetroSnake::CRetroSnake()
{
	m_pcRetroSnakeOptions[SPEED] = &m_cSpeed;
	m_pcRetroSnakeOptions[WALL] = &m_cWall;
	m_pcRetroSnakeOptions[LEVELUP] = &m_cLevelup;
	m_pcRetroSnakeOptions[DOUBLEPLAYER] = &m_cDouble;
	m_gRefreshTime[0] = 1000;
	m_gRefreshTime[1] = 500;
	m_gRefreshTime[2] = 250;
	m_gRefreshTime[3] = 125;
	m_gRefreshTime[4] = 63;
}

void CRetroSnake::Enter(CPlayer*player)
{
	HighLightOption = SPEED;
	m_cSpeed = 0;
	m_cWall = 0;
	m_cLevelup = 0;
	m_bStart = false;
	m_bReset = false;
	m_bExit = false;
	m_bGetFood = false;
	m_cDouble = false;


	CLS;
#ifdef WINDOWS_SYSTEM
	cout << "Welcome to RestroSnake!\n";
	system("cls");
	cout << "      options:\n";
	cout << "speed        " << (int)m_cSpeed << "\n";
	cout << "wall         " << (m_cWall ? "on" : "off") << "\n";
	cout << "levelup      " << (m_cLevelup ? "on" : "off") << "\n";
	cout << "PC2          " << (m_cDouble ? "on" : "off") << "\n";
#else
	CLS;
	switch (HighLightOption)
	{
	case SPEED:
		print_speed(m_cSpeed);
		break;
	case WALL:
		print_wall(m_cWall);
		break;
	case LEVELUP:
		print_levelup(m_cLevelup);
		break;
	case DOUBLEPLAYER:
		print_double(m_cDouble);
		break;
	}
#endif
	//CP_SLEEP(1000);
}

void CRetroSnake::Execute(CPlayer*player)
{
#ifdef DEBUG_RETROSNAKE
	if (!m_bStart)
	{
		m_cSpeed = 2;
		m_bStart = true;
		m_bReset = false;
		m_bExit = false;
		m_bGetFood = false;
		m_cDouble = 1;
		Initialize();
	}
#endif 
	if (m_bExit)
		player->ChangeState(NONE);
	if (m_bStart)
	{
		if (m_bChangeFlag)
		{
			Initialize();
			m_bChangeFlag = false;
		}
		Run();
		if (m_bReset)
			player->ChangeState(RETROSNAKE);

	}
	else
	{
		auto start = CP_CLOCK;
		auto finish = start + REFRESHTIME;
		if (m_bChangeFlag)
		{
			RetroSnakeOptionsCheck();
			m_bChangeFlag = false;
			CLS;
#ifdef WINDOWS_SYSTEM
			cout << "      options:\n";
			cout << "speed        " << (int)m_cSpeed << "\n";
			cout << "wall         " << (m_cWall ? "on" : "off") << "\n";
			cout << "levelup      " << (m_cLevelup ? "on" : "off") << "\n";
			cout << "PC2          " << (m_cDouble ? "on" : "off") << "\n";
#else
			CLS;
			switch (HighLightOption)
			{
			case SPEED:
				print_speed(m_cSpeed);
				break;
			case WALL:
				print_wall(m_cWall);
				break;
			case LEVELUP:
				print_levelup(m_cLevelup);
				break;
			case DOUBLEPLAYER:
				print_double(m_cDouble);
				break;
			}
#endif
		}

		keyin tem = NOOPERATION;
		while (CP_CLOCK < finish && !(tem = PressKey()))
		{

		}
		if (tem)
		{
			m_bChangeFlag = true;
			switch (tem)
			{
			case LEFT:
			case LEFT2:
				(*m_pcRetroSnakeOptions[HighLightOption])--;
				break;
			case UP:
			case UP2:
				HighLightOption = RetroSnakeOptions(HighLightOption - 1);
				break;
			case RIGHT:
			case RIGHT2:
				(*m_pcRetroSnakeOptions[HighLightOption])++;
				break;
			case DOWN:
			case DOWN2:
				HighLightOption = RetroSnakeOptions(HighLightOption + 1);
				break;
			case OK:
			case START:
				m_bStart = true;
				break;
			case EXIT:
				player->ChangeState(NONE);
			default:
				break;
			}
		}
		int RefreshTime = finish - CP_CLOCK;
		if (RefreshTime < 1)
			RefreshTime = 1;
		CP_SLEEP(RefreshTime);

	}

}

void CRetroSnake::Exit(CPlayer*player)
{

}

void CRetroSnake::RetroSnakeOptionsCheck()
{
	if (m_cLevelup > 1) m_cLevelup = 0;
	if (m_cSpeed > 5) m_cSpeed = 0;
	if (m_cWall > 1) m_cWall = 0;
	if (m_cDouble > 1) m_cDouble = 0;
	if (m_cLevelup < 0) m_cLevelup = 1;
	if (m_cSpeed < 0) m_cSpeed = 5;
	if (m_cWall < 0) m_cWall = 1;
	if (m_cDouble < 0) m_cDouble = 1;
	if (m_cDouble) m_cLevelup = 0;
	if (HighLightOption >= END_LABEL_OPTIONS)  HighLightOption = RetroSnakeOptions(0);
	if (HighLightOption < 0)  HighLightOption = RetroSnakeOptions(END_LABEL_OPTIONS - 1);
}

void CRetroSnake::Run()
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
			m_bReset = true;
			return;
		case RESTART:
			Initialize();
			return;
		case EXIT:
			m_bExit = true;
			break;
		default:
			return;
		}
	}
	else
	{
		auto start = CP_CLOCK;
		auto finish = start + m_gRefreshTime[m_cSpeed];
		if (!m_cDouble)
		{
			if (!m_bOver&&m_Direction)
			{
				Move();
				Refresh();
			}
			keyin tem = NOOPERATION;
			while (CP_CLOCK < finish && !(tem = PressKey()))
			{

			}
			if (tem)
			{
				switch (tem)
				{
				case UP2:
				case UP:
					if (m_Direction != DOWN && m_Direction != NOOPERATION)
						m_Direction = UP;
					break;
				case DOWN2:
				case DOWN:
					if (m_Direction != UP)
						m_Direction = DOWN;
					break;
				case LEFT2:
				case LEFT:
					if (m_Direction != RIGHT)
						m_Direction = LEFT;
					break;
				case RIGHT2:
				case RIGHT:
					if (m_Direction != LEFT)
						m_Direction = RIGHT;
					break;
				case RESTART:
					Initialize();
					return;
				case BACK:
				case RESET:
					m_bReset = true;
					return;
				case EXIT:
					m_bExit = true;
					return;
				case PAUSE:
					m_bPause = true;
					return;
				default:
					break;
				}
			}
		}
		else
		{
			//keyin tem = m_Direction, tem2 = m_Direction2;
			bool flag = true, flag2 = true;
			keyin press = NOOPERATION;
			if (m_bOver)
			{
				while (CP_CLOCK < finish && !(press = PressKey()))
				{

				}
				switch (press)
				{
				case RESTART:
					Initialize();
					return;
				case BACK:
				case RESET:
					m_bReset = true;
					return;
				case EXIT:
					m_bExit = true;
					return;
				case PAUSE:
					m_bPause = true;
					return;
				default:
					return;
				}
			}
			while (CP_CLOCK < finish && (flag || flag2))
			{
				press = PressKey(flag, flag2);
				switch (press)
				{
				case UP:
					if (m_Direction != DOWN)
						m_Direction = UP;
					flag = false;
					Move();
					break;
				case DOWN:
					if (m_Direction != UP)
						m_Direction = DOWN;
					flag = false;
					Move();
					break;
				case LEFT:
					if (m_Direction != RIGHT)
						m_Direction = LEFT;
					flag = false;
					Move();
					break;
				case RIGHT:
					if (m_Direction != LEFT)
						m_Direction = RIGHT;
					flag = false;
					Move();
					break;
				case UP2:
					if (m_Direction2 != DOWN2)
						m_Direction2 = UP2;
					flag2 = false;
					Move(true);
					break;
				case DOWN2:
					if (m_Direction2 != UP2)
						m_Direction2 = DOWN2;
					flag2 = false;
					Move(true);
					break;
				case LEFT2:
					if (m_Direction2 != RIGHT2)
						m_Direction2 = LEFT2;
					flag2 = false;
					Move(true);
					break;
				case RIGHT2:
					if (m_Direction2 != LEFT2)
						m_Direction2 = RIGHT2;
					flag2 = false;
					Move(true);
					break;
				case RESTART:
					Initialize();
					return;
				case BACK:
				case RESET:
					m_bReset = true;
					return;
				case EXIT:
					m_bExit = true;
					return;
				case PAUSE:
					m_bPause = true;
					return;
				default:
					break;
				}
			}
			if (m_Direction && !m_Direction2)
			{
				m_Direction2 = UP2;
				flag2 = false;
				Move(true);
			}
			if (!m_Direction && m_Direction2)
			{
				m_Direction = DOWN;
				flag = false;
				Move();
			}
			if (m_Direction && !m_bOver)
			{
				if (flag)
					Move();
				if (flag2)
					Move(true);
			}
		}
		int RefreshTime = finish - CP_CLOCK;
		if (RefreshTime < 1)
			RefreshTime = 1;
		CP_SLEEP(RefreshTime);
	}
}

void CRetroSnake::Move(bool snake)
{

	CPosition NewPosition, NewPosition2;
	m_Snake2.Move(m_Direction2, NewPosition2);
	m_Snake.Move(m_Direction, NewPosition);
	if (snake)
	{
		if (SafeCheck(NewPosition2))
		{
			m_Snake2 = NewPosition2;
			if (NewPosition2 == m_Food)
			{
				m_bGetFood = true;
				m_iLength2++;
				if (m_iLength2 >= 125)
				{
					m_bWin = true;
					m_bOver = true;
					return;
				}
				AddFood();
			}
			else
			{
				for (int i = 0; i < iYEDGE; i++)
					for (int j = 0; j < iXEDGE; j++)
					{
						if (m_ggiPrint[i][j] >= SNAKE2)
							m_ggiPrint[i][j]--;
						if (m_ggiPrint[i][j] == SNAKE2)
							m_ggiPrint[i][j] = BLANK;
					}
			}
			m_ggiPrint[m_Snake2.Get_Y()][m_Snake2.Get_X()] = m_iLength2 + SNAKE2;
		}
		else
		{
			m_bOver = true;
			return;
		}
	}
	else if (SafeCheck(NewPosition))
	{
		m_Snake = NewPosition;
		if (NewPosition == m_Food)
		{
			m_bGetFood = true;
			m_iLength++;
			if (m_iLength >= 250 || (m_cDouble&&m_iLength >= 125))
			{
				m_bWin = true;
				m_bOver = true;
				return;
			}
			if (m_cLevelup && m_cSpeed < 4 && m_iLength >= (m_cSpeed + 1) * 40)
			{
				m_cSpeed++;
			}
			AddFood();
		}
		else
		{
			for (int i = 0; i < iYEDGE; i++)
				for (int j = 0; j < iXEDGE; j++)
				{
					if (m_ggiPrint[i][j] >= SNAKE && !(m_cDouble&&m_ggiPrint[i][j] >= SNAKE2))
						m_ggiPrint[i][j]--;
					if (m_ggiPrint[i][j] == SNAKE)
						m_ggiPrint[i][j] = BLANK;
				}
		}
		m_ggiPrint[m_Snake.Get_Y()][m_Snake.Get_X()] = m_iLength + SNAKE;
	}
	else
	{
		m_bOver = true;
		return;
	}
	Refresh();
}

bool CRetroSnake::SafeCheck(CPosition&P)
{
	if (m_cWall && !P.SafeCheck())
	{
		return false;
	}
	else
		P.Safetify();
	if (m_ggiPrint[P.Get_Y()][P.Get_X()] > SNAKE + 1 && !(m_cDouble&&m_ggiPrint[P.Get_Y()][P.Get_X()] == SNAKE2 + 1))
		return false;
	return true;
}

void CRetroSnake::Initialize()
{
	m_bWin = false;
	m_bOver = false;
	m_bChangeFlag = false;
	m_bPause = false;
	memset(m_ggiPrint, BLANK, sizeof(m_ggiPrint));
	memset(m_ggiPrintSave, BLANK, sizeof(m_ggiPrintSave));
	m_Direction = NOOPERATION;
	m_Direction2 = NOOPERATION;
	m_iLength = 4;
	m_iLength2 = m_cDouble ? 4 : 0;
	for (int i = 0; i < m_iLength; i++)
	{
		m_ggiPrint[1 + i][iXEDGE / 2] = i + SNAKE + 1;
		if (m_cDouble)
			m_ggiPrint[iYEDGE - i - 2][iXEDGE / 2] = i + SNAKE2 + 1;
	}
	m_Snake = CPosition(iXEDGE / 2, m_iLength);
	m_Snake2 = CPosition(iXEDGE / 2, iYEDGE - m_iLength - 1);
	m_bWin = 0;
	m_bOver = 0;
	CLS;
	AddFood();
	Refresh();

}

void CRetroSnake::AddFood()
{
	int t = rand() % (iXEDGE*iYEDGE - m_iLength - m_iLength2) + 1;
	int i = 0, j = 0;
	while (true)
	{
		if (m_ggiPrint[j][i] == BLANK)
		{
			t--;
			if (!t)
				break;
		}
		i++;
		if (i == iXEDGE)
		{
			i = 0;
			j++;
		}
	}
	m_Food = CPosition(i, j);

}

void CRetroSnake::Refresh()
{
	m_ggiPrint[m_Food.Get_Y()][m_Food.Get_X()] = FOOD;
	Print(DRETROSNAKE, m_ggiPrint, m_ggiPrintSave);
	memcpy(m_ggiPrintSave, m_ggiPrint, sizeof(m_ggiPrint));
}
