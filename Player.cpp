
#include"Player.h"
#include"PlayerOwnedGames.h"


//one will work if respective debug-grand is defined
#ifdef DEBUG_GOBANG
CPlayer::CPlayer() :m_Game(GOBANG),m_pCurrentState(CGobang::Instance())

#elif defined DEBUG_TETRIS
CPlayer::CPlayer() : m_Game(TETRIS),m_pCurrentState(CTetris::Instance())

#elif defined DEBUG_STARTREK
CPlayer::CPlayer() : m_Game(STARTREK),m_pCurrentState(CStartrek::Instance())

#elif defined DEBUG_RETROSNAKE
CPlayer::CPlayer() : m_Game(RETROSNAKE),m_pCurrentState(CRetroSnake::Instance())

#else
//generally speaking, the default location is main menu
CPlayer::CPlayer() : m_Game(MAIN),
m_pCurrentState(CMain::Instance())
#endif
{
	
}

//called when the game is changed(or reset)
void CPlayer::ChangeState(CState*pNewState)
{
	m_pCurrentState->Exit(this);
	m_pCurrentState = pNewState;
	m_pCurrentState->Enter(this);
}


//every turn refresh the data
void CPlayer::Update()
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Execute(this);
	}
}

//called when it's needed to know the game name
void CPlayer::ChangeGameName(const Game_Name game)
{
	m_Game = game;
}

//calling the function to get the variable m_Game
Game_Name CPlayer::GetGameName()
{
	return m_Game;
}

//a function to get the game's instance by its name
CState* GetInstance(Game_Name Name)
{
	switch (Name)
	{
	case RETROSNAKE:
		return CRetroSnake::Instance();
	case GOBANG:
		return CGobang::Instance();
	case TETRIS:
		return CTetris::Instance();
	case SOKOBAN:
		return CSokoban::Instance();
	case STARTREK:
		return CStartrek::Instance();
	default:
		return CMain::Instance();
	}
}