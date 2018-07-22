#pragma once
//------------------------------------------------------------------------
//
//	Name:	Position.h
//
//	Desc:	defining a class to operate positions
//
//------------------------------------------------------------------------
#include"Compatible.h"

//this is an important class which gives some good mthods to deal with positions
class CPosition
{
public:
	CPosition(char x = 0, char y = 0) :m_x(x), m_y(y)
	{
	}
	CPosition(const CPosition&A)
	{
		m_x = A.m_x;
		m_y = A.m_y;
	}

	//this function will make the position stay in the safe area
	void Safetify(char XLimit = iXEDGE, char YLimit = iYEDGE)
	{
		if (m_x >= XLimit) m_x = 0;
		if (m_x < 0) m_x = XLimit - 1;
		if (m_y >= YLimit) m_y = 0;
		if (m_y < 0) m_y = YLimit - 1;
	}

	//this function will check if the position is in the safe area
	bool SafeCheck(char XLimit = iXEDGE, char YLimit = iYEDGE,char XMin = 0,char YMin = 0)
	{
		return m_x >= XMin && m_y >= YMin && m_x < XLimit && m_y < YLimit;
	}

	//compare if the two positions are equal
	bool operator==(const CPosition&A)
	{
		return (this->m_x == A.m_x) && (this->m_y == A.m_y);
	}

	//compare if the two positions are unequal
	bool operator!=(const CPosition&A)
	{
		return !((*this) == A);
	}

	//add two positions
	CPosition operator+(const CPosition&A)
	{
		return CPosition(m_x + A.m_x, m_y + A.m_y);
	}

	//assign a position
	CPosition& operator=(const CPosition&A)
	{
		m_x = A.m_x;
		m_y = A.m_y;
		return *this;
	}

	//move the position to the aimed direction,this method will change the origin position, 
	//and the return value is the position itself after moving
	CPosition& Move(keyin Direction)
	{
		switch (Direction)
		{
		case UP:
		case UP2:
			m_y--;
			break;
		case DOWN:
		case DOWN2:
			m_y++;
			break;
		case LEFT:
		case LEFT2:
			m_x--;
			break;
		case RIGHT:
		case RIGHT2:
			m_x++;
			break;
		case RIGHTUP:
			m_x++;
			m_y--;
			break;
		case RIGHTDOWN:
			m_x++;
			m_y++;
			break;
		case LEFTUP:
			m_x--;
			m_y--;
			break;
		case LEFTDOWN:
			m_x--;
			m_y++;
			break;
		}
		return *this;
	}

	//move the position to the aimed direction, and the origin position won't be changed,
	//the new position will be saved in the second parameter
	void Move(keyin Direction, CPosition&P)
	{
		switch (Direction)
		{
		case UP:
		case UP2:
			P.m_y = m_y - 1;
			P.m_x = m_x;
			break;
		case DOWN:
		case DOWN2:
			P.m_y = m_y + 1;
			P.m_x = m_x;
			break;
		case LEFT:
		case LEFT2:
			P.m_x = m_x - 1;
			P.m_y = m_y;
			break;
		case RIGHT:
		case RIGHT2:
			P.m_x = m_x + 1;
			P.m_y = m_y;
				break;
		case RIGHTUP:
			P.m_x = m_x + 1;
			P.m_y = m_y - 1;;
			break;
		case RIGHTDOWN:
			P.m_x = m_x + 1;
			P.m_y = m_y + 1;
			break;
		case LEFTUP:
			P.m_x = m_x - 1;
			P.m_y = m_y - 1;
			break;
		case LEFTDOWN:
			P.m_x = m_x - 1;
			P.m_y = m_y + 1;
			break;
		}
	}
	~CPosition()
	{
	}

	//get the X-coordinate of the position
	int Get_X()
	{
		return m_x;
	}

	//get the Y-coordinate of the position
	int Get_Y()
	{
		return m_y;
	}

private:
	char m_x;
	char m_y;
};

