#pragma once
#include"Compatible.h"

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
	void Safetify(char XLimit = iXEDGE, char YLimit = iYEDGE)
	{
		if (m_x >= XLimit) m_x = 0;
		if (m_x < 0) m_x = XLimit - 1;
		if (m_y >= YLimit) m_y = 0;
		if (m_y < 0) m_y = YLimit - 1;
	}
	bool SafeCheck(char XLimit = iXEDGE, char YLimit = iYEDGE,char XMin = 0,char YMin = 0)
	{
		return m_x >= XMin && m_y >= YMin && m_x < XLimit && m_y < YLimit;
	}
	bool operator==(const CPosition&A)
	{
		return (this->m_x == A.m_x) && (this->m_y == A.m_y);
	}
	bool operator!=(const CPosition&A)
	{
		return !((*this) == A);
	}
	CPosition operator+(const CPosition&A)
	{
		return CPosition(m_x + A.m_x, m_y + A.m_y);
	}
	CPosition& operator=(const CPosition&A)
	{
		m_x = A.m_x;
		m_y = A.m_y;
		return *this;
	}
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
	int Get_X()
	{
		return m_x;
	}
	int Get_Y()
	{
		return m_y;
	}

private:
	char m_x;
	char m_y;
};

