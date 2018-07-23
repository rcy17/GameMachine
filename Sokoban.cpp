
#include"PlayerOwnedGames.h"
#include"Player.h"



#ifdef _WIN32
#pragma warning(disable:4996)
#include<conio.h>
#include<iostream>
#include<fstream>
using std::cout;
#endif


CSokoban* CSokoban::Instance()
{
	static CSokoban Instance;
	return &Instance;

}

CSokoban::CSokoban():m_gcMap(NULL)
{

}

void CSokoban::Enter(CPlayer*player)
{
	CLS;
	//if the pointer points to some memory, delete it
	if (m_gcMap)
		delete[]m_gcMap;
	m_gcMap = NULL;
	m_gcMap = new unsigned char[513];

	//if failed to new, turn back to main menu
	if (!m_gcMap)
	{
		player->ChangeState(CMain::Instance());
		return;
	}
	
	m_cLevel = 0;
	ReadMap(m_cLevel);
	Refresh();
}

void CSokoban::Execute(CPlayer*player)
{
	if (JudgeWin())
	{
		ReadMap(++m_cLevel);
		Refresh();
	}
	auto start = CP_CLOCK;
	auto finish = start + REFRESHTIME;
	keyin tem = NOOPERATION;
	while ((!(tem = PressKey()) && CP_CLOCK < finish))
	{
		//avoid high CPU occupation
		Sleep(1);
	}
	if (tem)
		switch (tem)
		{
		case UP:
		case DOWN:
		case LEFT:
		case RIGHT:
		case UP2:
		case DOWN2:
		case LEFT2:
		case RIGHT2:
			Move(tem);
			break;
		case OK:
			Turnback();
			break;
		case CANCEL:
			Withdraw();
			break;
		case EXIT:
			player->ChangeState(CMain::Instance());
			break;
		case NEXT:
			//turn to the next level
			ReadMap(++m_cLevel);
			break;
		case BACK:
			//turn to the last level
			ReadMap(--m_cLevel);
			break;
		case RESTART:
		case RESET:
			//reload this level, no need to read the file
			ReadMap(m_cLevel,false);
			break;
		}
	Refresh();
	int RefreshTime = finish - CP_CLOCK;
	if (RefreshTime < 1)
		RefreshTime = 1;
	CP_SLEEP(RefreshTime);
}

void CSokoban::Exit(CPlayer*player)
{
	if (m_gcMap)
		delete []m_gcMap;
	m_gcMap = NULL;
#ifdef _WIN32
	//release the memery
	m_qSave.clear();
	m_qSave.shrink_to_fit();
#endif
}

//judge if the player wins
bool CSokoban::JudgeWin()
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if (g_ggcPrint[i][j] == AIM || g_ggcPrint[i][j] == PERSON_AIM)
			{
				return 0;
			}
		}
	}
	return 1;
}

//read the new map from the file
void CSokoban::ReadMap(char& level,bool Change)
{
	//clear the deque
	m_qSave.clear();
	m_qSave.shrink_to_fit();

	//reset the count
	m_iCount = 0;

	//limit the range of level
	if (level == 7)
		level = 0;
	if (level == -1)
		level = 6;
	CLS;
#ifdef  _WIN32
	MoveCursor(20, 8);
	cout << "²½Êý:" << m_iCount;

	//read the map if the map changed
	if (Change)
	{
		ifstream file("map.dat");
		for (char i = 0; i <= level; i++)
			file >> m_gcMap;
		file.close();
	}
#endif

	//tanslate the map data
	for (short i = 0; i < 512; i++)
	{
		switch (m_gcMap[i])
		{
		case '0':
			m_gcMap[i] = BLANK;
			break;
		case '1':
			m_gcMap[i] = PATH;
			break;
		case '2':
			m_gcMap[i] = OBSTACLE;
			break;
		case '3':
			m_gcMap[i] = PERSON;
			m_Person = CPosition(i % 32, i / 32);
			break;
		case '4':
			m_gcMap[i] = BOX;
			break;
		case '5':
			m_gcMap[i] = AIM;
			break;
		case '6':
			m_gcMap[i] = BOX_AIM;
			break;
		case '7':
			m_gcMap[i] = PERSON_AIM;
			break;
		}
	}

	//copy the map to the print
	memset(g_ggcPrintSave, BLANK, sizeof(g_ggcPrint));
	memcpy(g_ggcPrint, m_gcMap, sizeof(g_ggcPrint));
}

//called when the player try to move
void CSokoban::Move(const keyin &dir)
{
	CPosition next, next2;

	//try to move
	m_Person.Move(dir, next);
	next.Move(dir, next2);

	//can't go into the wall
	if (g_ggcPrint[next.Get_Y()][next.Get_X()] == OBSTACLE)
		return;

	//move to the path or aim is easy
	if (g_ggcPrint[next.Get_Y()][next.Get_X()] == PATH ||
		g_ggcPrint[next.Get_Y()][next.Get_X()] == AIM)
	{
		ChangeItem(m_Person, BLANK);
		ChangeItem(next, PERSON);
		m_Person = next;
#ifdef _WIN32
		//clear the next memory
		if (m_iCount != m_qSave.size())
		{
			m_qSave.erase(m_qSave.begin() + m_iCount, m_qSave.end());
		}
		m_qSave.push_back(dir);
#endif
		m_iCount++;
	}

	if (g_ggcPrint[next.Get_Y()][next.Get_X()] == BOX ||
		g_ggcPrint[next.Get_Y()][next.Get_X()] == BOX_AIM)
	{
		//can't push the box if there is another box or wall behind it
		if (g_ggcPrint[next2.Get_Y()][next2.Get_X()] == BOX ||
			g_ggcPrint[next2.Get_Y()][next2.Get_X()] == BOX_AIM ||
			g_ggcPrint[next2.Get_Y()][next2.Get_X()] == OBSTACLE)
			return;

		//else push the box
		ChangeItem(m_Person, BLANK);
		ChangeItem(next, PERSON);
		ChangeItem(next2, BOX);
		m_Person = next;
#ifdef _WIN32
		if (m_iCount != m_qSave.size())
		{
			m_qSave.erase(m_qSave.begin() + m_iCount, m_qSave.end());
		}
		//use OK to label the push
		m_qSave.push_back(keyin(dir + OK));
#endif

		m_iCount++;
	}
#ifdef _WIN32
	MoveCursor(25, 8);
	cout.width(4);
	cout.setf(ios_base::left);
	cout << m_iCount;
#endif
}

//refresh the print
void CSokoban::Refresh()
{
	Print(SOKOBAN);
	memcpy(g_ggcPrintSave, g_ggcPrint, sizeof(g_ggcPrint));
}

//change the item after move
void CSokoban::ChangeItem(CPosition &P, const item &tem)
{
	switch (tem)
	{
	case BLANK:
		switch (g_ggcPrint[P.Get_Y()][P.Get_X()])
		{
		case PERSON:
		case BOX:
			g_ggcPrint[P.Get_Y()][P.Get_X()] = PATH;
			break;
		case PERSON_AIM:
		case BOX_AIM:
			g_ggcPrint[P.Get_Y()][P.Get_X()] = AIM;
			break;
		}
		break;
	case PERSON:
		switch (g_ggcPrint[P.Get_Y()][P.Get_X()])
		{
		case PATH:
		case BOX:
			g_ggcPrint[P.Get_Y()][P.Get_X()] = PERSON;
			break;
		case AIM:
		case BOX_AIM:
			g_ggcPrint[P.Get_Y()][P.Get_X()] = PERSON_AIM;
			break;
		}
		break;
	case BOX:
		switch (g_ggcPrint[P.Get_Y()][P.Get_X()])
		{
		case PATH:
		case PERSON:
			g_ggcPrint[P.Get_Y()][P.Get_X()] = BOX;
			break;
		case AIM:
		case PERSON_AIM:
			g_ggcPrint[P.Get_Y()][P.Get_X()] = BOX_AIM;
			break;
		}
		break;
	}
}

#ifdef _WIN32
//withdraw the last operation
void CSokoban::Withdraw()
{
	//you can't withdraw if this is already the first one
	if (!m_iCount)
		return;
	m_iCount--;
	keyin Direction = m_qSave[m_iCount];

	//use ok label to judge if there is a box pushed
	if (Direction < OK)
	{
		ChangeItem(m_Person, BLANK);
		ChangeItem(m_Person.Move(NegativeDirection(Direction)), PERSON);
	}
	else
	{
		Direction = keyin(Direction - OK);
		CPosition tem = m_Person;
		tem.Move(Direction);
		ChangeItem(tem, BLANK);
		ChangeItem(m_Person, BOX);
		ChangeItem(m_Person.Move(NegativeDirection(Direction)), PERSON);
	}
#ifdef _WIN32
	MoveCursor(25, 8);
	cout.width(4);
	cout.setf(ios_base::left);
	cout << m_iCount;
#endif
}


//turn back the next operation
void CSokoban::Turnback()
{
	//you can't turn back if this is already the newest one
	if (m_iCount == m_qSave.size())
		return;
	keyin Direction = m_qSave[m_iCount];
	m_iCount++;
	
	//just push up, because it's absulotely safe
	if (Direction > OK)
		Direction = keyin(Direction - OK);
	CPosition tem = m_Person;
	tem.Move(Direction);
	if (g_ggcPrint[tem.Get_Y()][tem.Get_X()] == BOX || g_ggcPrint[tem.Get_Y()][tem.Get_X()] == BOX_AIM)
	{
		ChangeItem(tem.Move(Direction), BOX);
	}
	ChangeItem(m_Person, BLANK);
	ChangeItem(m_Person.Move(Direction), PERSON);
#ifdef _WIN32
	MoveCursor(25, 8);
	cout.width(4);
	cout.setf(ios_base::left);
	cout << m_iCount;
#endif
}

//find the negative direction
keyin CSokoban::NegativeDirection(const keyin &dir)
{
	switch (dir)
	{
	case UP:
	case UP2:
		return DOWN;
	case LEFT:
	case LEFT2:
		return RIGHT;
	case RIGHT:
	case RIGHT2:
		return LEFT;
	case DOWN:
	case DOWN2:
		return UP;
	default:
		return NOOPERATION;
	}
}
#endif