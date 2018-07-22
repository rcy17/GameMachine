#pragma once
#include"Compatible.h"

class CState;

class CPlayer
{
public:
	void Update();
	void ChangeState(CState *pnew_state);
	//void ChangeState(const Game_Name&name);
	void ChangeGameName(const Game_Name game);
	CPlayer();
	Game_Name GetGameName();
private:
	
	CState *m_pCurrentState;
	Game_Name m_Game;
};

CState* GetInstance(Game_Name Name);