
#include"PlayerOwnedGames.h"
#include"Player.h"




#ifdef _WIN32
#pragma warning(disable:4996)
#include<conio.h>
#include<iostream>
using std::cout;
#endif

/************** methods for CRetroSnake ***************************/

//here are some assignments for const variables

//here are deffrent refresh periods for deffrent speeds
const int CRetroSnake::m_giRefreshTime[5] = { 1000,500,250,125,63 };

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
}

void CRetroSnake::Enter(CPlayer*player)
{
	m_HighLightOption = SPEED;
	m_cSpeed = 0;
	m_cWall = 0;
	m_cLevelup = 0;
	m_bStart = false;
	m_bReset = false;
	m_bExit = false;
	m_bGetFood = false;
	m_cDouble = false;

	CLS;
#ifdef _WIN32
	cout << "Welcome to RestroSnake!\n";
	system("cls");
	cout << "       options:\n";
	cout << "speed            " << (int)m_cSpeed << "\n";
	cout << "wall           " << (m_cWall ? " on  " : "off  ") << "\n";
	cout << "levelup        " << (m_cLevelup ? " on  " : "off  ") << "\n";
	cout << "PC2            " << (m_cDouble ? " on  " : "off  ") << "\n";
	MoveCursor(18, 1 + m_HighLightOption);
	cout << "¡û";
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

	//exit the game
	if (m_bExit)
		player->ChangeState(CMain::Instance());
	if (m_bStart)
	{
		//initialize at the first frame after start
		if (m_bChangeFlag)
		{
			Initialize();
			m_bChangeFlag = false;
		}

		//every frame refresh here
		Run();

		//reload the game
		if (m_bReset)
			player->ChangeState(CRetroSnake::Instance());

	}

	//change the options
	else
	{
		auto start = CP_CLOCK;
		auto finish = start + REFRESHTIME;
		if (m_bChangeFlag)
		{
			RetroSnakeOptionsCheck();
			m_bChangeFlag = false;

#ifdef _WIN32
			MoveCursor(15, 1);
			cout << "  " << (int)m_cSpeed << "  ";
			MoveCursor(15, 2);
			cout << (m_cWall ? " on  " : "off  ");
			MoveCursor(15, 3);
			cout << (m_cLevelup ? " on  " : "off  ");
			MoveCursor(15, 4);
			cout << (m_cDouble ? " on  " : "off  ");
			MoveCursor(18, m_HighLightOption + 1);
			cout << "¡û";
#endif
		}

		keyin tem = NOOPERATION;
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
			case LEFT:
			case LEFT2:
				(*m_pcRetroSnakeOptions[m_HighLightOption])--;
				break;
			case UP:
			case UP2:
				m_HighLightOption = RetroSnakeOptions(m_HighLightOption - 1);
				break;
			case RIGHT:
			case RIGHT2:
				(*m_pcRetroSnakeOptions[m_HighLightOption])++;
				break;
			case DOWN:
			case DOWN2:
				m_HighLightOption = RetroSnakeOptions(m_HighLightOption + 1);
				break;
			case OK:
			case START:
				m_bStart = true;
				break;
			case EXIT:
				player->ChangeState(CMain::Instance());
				return;
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

//this function will check and correct the options variables
void CRetroSnake::RetroSnakeOptionsCheck()
{
	if (m_cLevelup > 1) m_cLevelup = 0;
	if (m_cSpeed > 4) m_cSpeed = 0;
	if (m_cWall > 1) m_cWall = 0;
	if (m_cDouble > 1) m_cDouble = 0;
	if (m_cLevelup < 0) m_cLevelup = 1;
	if (m_cSpeed < 0) m_cSpeed = 4;
	if (m_cWall < 0) m_cWall = 1;
	if (m_cDouble < 0) m_cDouble = 1;
	if (m_cDouble) m_cLevelup = 0;
	if (m_HighLightOption >= END_LABEL_OPTIONS)  m_HighLightOption = RetroSnakeOptions(0);
	if (m_HighLightOption < 0)  m_HighLightOption = RetroSnakeOptions(END_LABEL_OPTIONS - 1);
}

//called every turn after start
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
			//avoid high CPU occupation
			Sleep(1);
			return;
		}
	}
	else
	{
		//calculate the frame's finish time, it's related to speed
		auto start = CP_CLOCK;
		auto finish = start + m_giRefreshTime[m_cSpeed];

		//for single player
		if (!m_cDouble)
		{
			//after starting moving, move to the aimed direction
			if (!m_bOver&&m_Direction)
			{
				Move();
				Refresh();
			}
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

		//for double players
		else
		{
			//use flag to save if the player has moved
			bool flag = true, flag2 = true;
			keyin press = NOOPERATION;

			//if the game over, then can't go on
			if (m_bOver)
			{
				while (CP_CLOCK < finish && !(press = PressKey()))
				{
					//avoid high CPU occupation
					Sleep(1);
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

			//wait to press until this frame time out of both players has moved
			while (CP_CLOCK < finish && (flag || flag2))
			{
				//a player can move once in a frame
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

			//if one player starts to move, then game starts
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

			//if a player didn't move, then move automatically
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

//move the snake, the parameter is the number of the snake
void CRetroSnake::Move(bool snake)
{

	CPosition NewPosition, NewPosition2;
	m_Snake2.Move(m_Direction2, NewPosition2);
	m_Snake.Move(m_Direction, NewPosition);

	//move for the player 2
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
						if (g_ggcPrint[i][j] >= SNAKE2)
							g_ggcPrint[i][j]--;
						if (g_ggcPrint[i][j] == SNAKE2)
							g_ggcPrint[i][j] = BLANK;
					}
			}
			g_ggcPrint[m_Snake2.Get_Y()][m_Snake2.Get_X()] = m_iLength2 + SNAKE2;
		}
		else
		{
			m_bOver = true;
			return;
		}
	}

	//move for the Player1
	else if (SafeCheck(NewPosition))
	{
		m_Snake = NewPosition;

		//if get the food, then eat it
		if (NewPosition == m_Food)
		{
			m_bGetFood = true;
			m_iLength++;

			//250 is the max length for single player, and half for double players
			if (m_iLength >= 250 || (m_cDouble&&m_iLength >= 125))
			{
				m_bWin = true;
				m_bOver = true;
				return;
			}

			//speed up if the m_cSpeed is true and get to duing length
			if (m_cLevelup && m_cSpeed < 4 && m_iLength >= (m_cSpeed + 1) * 40)
			{
				m_cSpeed++;
			}

			//add a new food
			AddFood();
		}
		else
		{
			//normally moving
			char xMin, xMax, yMin, yMax;
			if (m_cWall)
			{
				xMin = 1, xMax = iXEDGE - 1, yMin = 1, yMax = iYEDGE - 1;
			}
			else
			{
				xMin = 0, xMax = iXEDGE, yMin = 0, yMax = iYEDGE;
			}
			for (int i = yMin; i < yMax; i++)
				for (int j = xMin; j < xMax; j++)
				{
					if (g_ggcPrint[i][j] >= SNAKE && !(m_cDouble&&g_ggcPrint[i][j] >= SNAKE2))
						g_ggcPrint[i][j]--;
					if (g_ggcPrint[i][j] == SNAKE)
						g_ggcPrint[i][j] = BLANK;
				}
		}
		g_ggcPrint[m_Snake.Get_Y()][m_Snake.Get_X()] = m_iLength + SNAKE;
	}

	//if snake die, game over
	else
	{
		m_bOver = true;
		return;
	}
	Refresh();
}

//check if the snake is in the safe erea,
//or make it safe if there is no wall
bool CRetroSnake::SafeCheck(CPosition&P)
{
	//if there is wall, moving out of the range leads to die
	if (m_cWall && !P.SafeCheck(iXEDGE - 1, iYEDGE - 1, 1, 1))
	{
		return false;
	}
	//if there is no wall but move out of the range, then move to the other side of edge
	else
		P.Safetify();

	//if get to another snake or itself, this snake dies;
	if (g_ggcPrint[P.Get_Y()][P.Get_X()] > SNAKE + 1 && !(m_cDouble&&g_ggcPrint[P.Get_Y()][P.Get_X()] == SNAKE2 + 1))
		return false;
	return true;
}

//initialize the game
void CRetroSnake::Initialize()
{
	m_bWin = false;
	m_bOver = false;
	m_bChangeFlag = false;
	m_bPause = false;
	memset(g_ggcPrint, BLANK, sizeof(g_ggcPrint));
	memset(g_ggcPrintSave, BLANK, sizeof(g_ggcPrintSave));
	m_Direction = NOOPERATION;
	m_Direction2 = NOOPERATION;
	m_iLength = 4;
	m_iLength2 = m_cDouble ? 4 : 0;
	for (int i = 0; i < m_iLength; i++)
	{
		g_ggcPrint[1 + i][iXEDGE / 2] = i + SNAKE + 1;
		if (m_cDouble)
			g_ggcPrint[iYEDGE - i - 2][iXEDGE / 2] = i + SNAKE2 + 1;
	}
	if (m_cWall)
	{
		for (char i = 0; i < iXEDGE; i++)
			g_ggcPrint[0][i] = g_ggcPrint[iYEDGE - 1][i] = EDGE;
		for (char i = 1; i < iYEDGE - 1; i++)
			g_ggcPrint[i][0] = g_ggcPrint[i][iXEDGE - 1] = EDGE;
	}
	m_Snake = CPosition(iXEDGE / 2, m_iLength);
	m_Snake2 = CPosition(iXEDGE / 2, iYEDGE - m_iLength - 1);
	m_bWin = 0;
	m_bOver = 0;

	CLS;
	AddFood();
	Refresh();

}

//add the food when the snake get the last one
void CRetroSnake::AddFood()
{
	//use a random number to determine the new postion of the food
	int t;
	if (m_cWall)
	{
		t = rand() % ((iXEDGE - 2)*(iYEDGE - 2) - m_iLength - m_iLength2) + 1;
	}
	else
	{
		t = rand() % (iXEDGE*iYEDGE - m_iLength - m_iLength2) + 1;
	}
	int i = 0, j = 0;
	while (true)
	{
		if (g_ggcPrint[j][i] == BLANK)
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

//refresh the screen every frame
void CRetroSnake::Refresh()
{
	g_ggcPrint[m_Food.Get_Y()][m_Food.Get_X()] = FOOD;
	Print(DRETROSNAKE);
	memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
}
