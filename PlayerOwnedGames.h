#ifndef ARDUINO
#pragma once
#include"Position.h"
#include"State.h"
#include"Compatible.h"
#include"music.h"
#endif

class CPlayer;
enum RetroSnakeOptions
{
	SPEED,
	WALL,
	LEVELUP,
	DOUBLEPLAYER,

	END_LABEL_OPTIONS	//just to label the end of enum
};

class CMain :public CState
{
private:
	Game_Name HighLightGame, HighLightGameSave;
	CMain()
	{
		HighLightGame = RETROSNAKE;
		HighLightGameSave = NONE;
	}
	CMain(const CMain&);

public:
	static CMain *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};

class CRetroSnake :public CState
{
private:
	CRetroSnake();
	CRetroSnake(const CRetroSnake&);
	char m_cSpeed;
	char m_cWall;
	char m_cLevelup;
	char m_cDouble;
	RetroSnakeOptions HighLightOption;
	char*m_pcRetroSnakeOptions[4];
	bool m_bPause;
	bool m_bStart;
	bool m_bOver;
	bool m_bWin;
	bool m_bChangeFlag;
	bool m_bReset;
	bool m_bExit;
	bool m_bGetFood;
	
	int m_iLength;
	int m_iLength2;
	//int iXEDGE*iYEDGE;
	int m_gRefreshTime[5];
	//unsigned char m_ggcSnakeTurn[iYEDGE][iXEDGE];
	keyin m_Direction;
	keyin m_Direction2;
	CPosition m_Food,m_Snake,m_Snake2;

	void RetroSnakeOptionsCheck();
	void Initialize();
	void Run();
	void Move(bool snake=false);
	void AddFood();
	void Refresh();
	bool SafeCheck(CPosition&P);
public:
	static CRetroSnake *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};

class CGobang :public CState
{
private:
	CGobang();
	CGobang(const CGobang&);
	bool m_bWin;
	bool m_bOver;
	bool m_bWhiteGo;
	int m_iBlankRest;
	//int m_iTurn;
	CPosition m_HighLight;
	CPosition m_HighLightBefore;
	bool GoCheck();
	void JudgeOver();
	void Refresh();
	int CountChess(const item &Now, const keyin &Direction1, const keyin &Direction2);


	item GetItem();
public:
	static CGobang *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};

class CTetris :public CState
{
private:
	CTetris();
	CTetris(const CTetris&);
	char GetLocation(const unsigned char*s,const char&Count);
	void Clear();
	void Initialize();
	void Refresh();
	bool SafeMove(const keyin&key);
	unsigned char m_cShapeNow;
	unsigned char m_cShapeNext;
	CPosition m_Shape;
	char m_cSpeed;
	int m_iScore;
	bool m_bStay;
	bool m_bStart;
	bool m_bOver;
	bool m_bChangeFlag;
	bool m_bPause;
	bool m_bNewBlock;
	char m_iXEdge;
	char m_iYEdge;


public:
	static CTetris *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};

class CSokoban :public CState
{
private:
	CSokoban();
	CSokoban(const CSokoban&);
	CPosition person;
	CPosition next, next2;
	void SetMap(unsigned char& level);
	void Move(keyin &dir);
	bool win();
	unsigned char level;
public:
	static CSokoban *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
	

};

class CStartrek :public CState
{
private:
	CStartrek();
	CStartrek(const CStartrek&);
	char m_cXEdge;
	char m_cY;
	char m_cYMax;
	char m_cYMin;
	char m_cDelay;
	int m_iCount;
	char m_cLevel;
	bool m_bOver;
	bool m_bPause;
	bool m_bStart;
	int m_iScore;
	static char m_cRefreshTruns[6];
	void Refresh();
	void Initialize();
public:
	static CStartrek *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};

/*
class CRetroSnake :public CState
{
private:
	CRetroSnake()
	{

	}
	CRetroSnake(const CRetroSnake&);

public:
	static CRetroSnake *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};
*/