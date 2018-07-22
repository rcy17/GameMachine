#pragma once
//------------------------------------------------------------------------
//
//  Name:   State.h
//
//  Desc:   abstract base class to define an interface for a state(game)
//
//------------------------------------------------------------------------

class CPlayer;

//a abstract class as the basic class of all game classes
class CState
{
public:
	CState(){ }
	//called when entering a new game
	virtual void Enter(CPlayer*) = 0;
	//called when nomally executing the game
	virtual void Execute(CPlayer*) = 0;
	//called when exit a game
	virtual void Exit(CPlayer*) = 0;
};
