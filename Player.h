#ifndef ARDUINO
#pragma once
#include"Compatible.h"
#endif

class CState;

class CPlayer
{
public:
	void Update();
	//void ChangeState(CState *pnew_state);
	void ChangeState(const Game_Name&name);
	void ChangeGameName(const Game_Name game)
	{
		m_Game = game;
	}
	CPlayer();
	Game_Name GetGameName()
	{
		return m_Game;
	}
private:
	
	CState *m_pCurrentState;
	Game_Name m_Game;
};

