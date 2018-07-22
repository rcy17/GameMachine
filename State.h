#ifndef ARDUINO
#pragma once
#endif

class CPlayer;

class CState
{
public:
	CState(){ }
	virtual void Enter(CPlayer*) = 0;
	virtual void Execute(CPlayer*) = 0;
	virtual void Exit(CPlayer*) = 0;
#ifdef WINDOWS_SYSTEM
	unsigned char m_ggiPrint[16][32];
	unsigned char m_ggiPrintSave[16][32];
#endif
};
