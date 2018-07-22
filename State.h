#pragma once

class CPlayer;

class CState
{
public:
	CState(){ }
	virtual void Enter(CPlayer*) = 0;
	virtual void Execute(CPlayer*) = 0;
	virtual void Exit(CPlayer*) = 0;
};
