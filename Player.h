#pragma once
//------------------------------------------------------------------------
//
//	Name:	Player.h
//
//	Desc:	defining the CPlayer class and declearing some functions
//
//------------------------------------------------------------------------

#include"Compatible.h"

class CState;

//here is the decleartion of the player's class
class CPlayer
{
public:
	CPlayer();

	//every turn refresh the data
	void Update();

	//called when the game is changed(or reset)
	void ChangeState(CState *pnew_state);

	//called when it's needed to know the game name
	void ChangeGameName(const Game_Name game);

	//calling the function to get the variable m_Game
	Game_Name GetGameName();
private:
	//a pointer to the current game
	CState *m_pCurrentState;

	//current game
	Game_Name m_Game;
};

//a function to get the game's instance by its name
CState* GetInstance(Game_Name Name);


