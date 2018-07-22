
#include"Player.h"
#include"PlayerOwnedGames.h"

#ifdef DEBUG_GOBANG
CPlayer::CPlayer() :m_Game(GOBANG),
m_pCurrentState(CGobang::Instance())
#elif defined DEBUG_TETRIS
CPlayer::CPlayer() : m_Game(TETRIS),
m_pCurrentState(CTetris::Instance())
#elif defined DEBUG_STARTREK
CPlayer::CPlayer() : m_Game(STARTREK),
m_pCurrentState(CStartrek::Instance())
#elif defined DEBUG_RETROSNAKE
CPlayer::CPlayer() : m_Game(RETROSNAKE),
m_pCurrentState(CRetroSnake::Instance())
#else
CPlayer::CPlayer() : m_Game(NONE),
m_pCurrentState(CMain::Instance())
#endif
{

}

void CPlayer::ChangeState(CState*pNewState)
{
	m_pCurrentState->Exit(this);
	m_pCurrentState = pNewState;
	m_pCurrentState->Enter(this);
}
/*void CPlayer::ChangeState(const Game_Name&name)
{
	m_pCurrentState->Exit(this);
	switch (name)
	{
	case NONE:
		m_pCurrentState = CMain::Instance();
		break;
	case RETROSNAKE:
		m_pCurrentState = CRetroSnake::Instance();
		break;
	case GOBANG:
		m_pCurrentState = CGobang::Instance();
		break;
	case TETRIS:
		m_pCurrentState = CTetris::Instance();
		break;
	case SOKOBAN:
		m_pCurrentState = CSokoban::Instance();
		break;
	case STARTREK:
		m_pCurrentState = CStartrek::Instance();
		break;
	default:
		m_pCurrentState = CMain::Instance();
		break;
	}
	m_pCurrentState->Enter(this);
}*/

void CPlayer::Update()
{
	if (m_pCurrentState)
	{
		m_pCurrentState->Execute(this);
	}
}

void CPlayer::ChangeGameName(const Game_Name game)
{
	m_Game = game;
}

Game_Name CPlayer::GetGameName()
{
	return m_Game;
}

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