#pragma once
//------------------------------------------------------------------------
//
//	Name:	PlayerOwnedGames.h
//
//	Desc:	defining all the games in this programme
//
//------------------------------------------------------------------------

#include"Position.h"
#include"State.h"
#ifdef _WIN32
#include <stack>
#include <deque>
using namespace std;
#endif

class CPlayer;
enum RetroSnakeOptions
{
	SPEED,
	WALL,
	LEVELUP,
	DOUBLEPLAYER,

	//label the end of enum
	END_LABEL_OPTIONS
};

//this is the main menu before enter a game
class CMain :public CState
{
private:

	CMain();
	
	CMain(const CMain&);

#ifdef _WIN32
	//if the text has been printed, it will be true
	bool m_bPrintFlag;
#endif

	//save the highlighted game to choose 
	Game_Name HighLightGame;

	//save the last highlighted game to choose 
	Game_Name HighLightGameSave;
public:
	static CMain *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};

//this is the Retro Snake game
class CRetroSnake :public CState
{
private:
	CRetroSnake();
	CRetroSnake(const CRetroSnake&);

	//decide the speed of the snake
	char m_cSpeed;

	//decide if the snake will die when go out of the screen
	//or move to the other side of the screen
	char m_cWall;

	//decide if the snake will speed up when get some score
	char m_cLevelup;

	//decide if there second snake will enter
	char m_cDouble;

	//save the highlight option before start the game
	RetroSnakeOptions m_HighLightOption;

	//a array to save all the options
	char*m_pcRetroSnakeOptions[4];

	//the game will pause when it's true
	bool m_bPause;

	//the game will start when it's true
	bool m_bStart;

	//the game will over when it's true
	bool m_bOver;

	//the game will be reset when it's true
	bool m_bReset;

	//the game will exit when it's true
	bool m_bExit;

	//it will be true when the player win
	bool m_bWin;

	//it will be true when player change the option
	bool m_bChangeFlag;

	//it will be true when the snake get the food
	bool m_bGetFood;

	//the lenth of the snake
	int m_iLength;

	//the lenth of the second snake
	int m_iLength2;

	//here are deffrent refresh periods for deffrent speeds
	static const int m_giRefreshTime[5];

	//the derecition of the sanke
	keyin m_Direction;

	//the derecition of the second sanke
	keyin m_Direction2;

	//the postion of the food
	CPosition m_Food;

	//the postion of the head of the snake
	CPosition m_Snake;

	//the postion of the head of the secon snake
	CPosition m_Snake2;

	//this function will check and correct the options variables
	void RetroSnakeOptionsCheck();

	//initialize the game
	void Initialize();

	//called every turn after start
	void Run();

	//move the snake, the parameter is the number of the snake
	void Move(bool snake = false);

	//add the food when the snake get the last one
	void AddFood();

	//refresh the screen every frame
	void Refresh();

	//check if the snake is in the safe erea,
	//or make it safe if there is no wall
	bool SafeCheck(CPosition&P);
public:
	static CRetroSnake *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};

//this is the Gongbang game
class CGobang :public CState
{
private:
	CGobang();
	CGobang(const CGobang&);

	//it will be true when the game over
	bool m_bOver;

	//it will be true when the player2 go and false then player1
	bool m_bWhiteGo;

	//the number of the blanks
	int m_iBlankRest;

	//the current turns
	int m_iTurn;

	//the highlight position 
	CPosition m_HighLight;

	//the passed highlight postion
	CPosition m_HighLightBefore;

#ifdef _WIN32
	//save all going to withdraw
	stack<CPosition>m_sChessSave;

	//withdraw the last going
	void Withdraw();
#endif

	//check if this position can put a chess
	bool GoCheck();

	//judge if the game is over
	void JudgeOver();

	//refresh the screen every turn
	void Refresh();

	//count the continuous chess in a line
	int CountChess(const item &Now, const keyin &Direction1, const keyin &Direction2);

	//get the chess which is going now
	item GetItem();
public:
	static CGobang *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};

//this is the Tetris game
class CTetris :public CState
{
private:
	CTetris();
	CTetris(const CTetris&);

	//get the position number of a block by its storage value
	unsigned char GetLocation(const unsigned char*s, const char&Count);

	//clear the row when it's full
	void Clear();

	//initialize the game
	void Initialize();

	//refresh the screen every frame
	void Refresh();

	//try to move the block, return true if move successfully 
	bool SafeMove(const keyin&key);

	//the current shape
	unsigned char m_cShapeNow;

	//the next shape
	unsigned char m_cShapeNext;

	//the Leftup position of the current shape
	CPosition m_Shape;

	//the speed now
	char m_cSpeed;

	//the score now
	int m_iScore;

	//not clear
	bool m_bStay;

	//the game starts when it's true
	bool m_bStart;
	
	//it will be true if the game over
	bool m_bOver;

	//it will be true if the option is changed
	bool m_bChangeFlag;

	//the game pause when it's true
	bool m_bPause;

	//it will be true if the current shape reach the bottom
	bool m_bNewBlock;

	//the x range
	char m_iXEdge;

	//The y range
	char m_iYEdge;


public:
	static CTetris *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};

//this is the Sokoban game
class CSokoban :public CState
{
private:
	CSokoban();
	CSokoban(const CSokoban&);

	//the position of the player
	CPosition m_Person;

	//the current level
	char m_cLevel;

	//count the steps
	short m_iCount;

	//save the current map in memory
	unsigned char m_gcMap[512];

#ifdef _WIN32
	//a deque to withdraw
	deque<keyin> m_qSave;
#endif

	//read the new map from the file
	void ReadMap(char& level);

	//called when the player try to move
	void Move(const keyin &dir);

	//judge if the player wins
	bool JudgeWin();
	
	//refresh the print
	void Refresh();

	//change the item after move
	void ChangeItem(CPosition &P,const item&tem);

#ifdef _WIN32
	//withdraw the last operation
	void Withdraw();

	//turn back the next operation
	void Turnback();

	//find the negative direction
	keyin NegativeDirection(const keyin &dir);
#endif
public:
	static CSokoban *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);


};

//this is the Startrek game
class CStartrek :public CState
{
private:
	CStartrek();
	CStartrek(const CStartrek&);
	
	//the x range
	static const char m_cXEdge;

	//the Max y-coordinate
	static const char m_cYMax;

	//the Min y-coordinate
	static const char m_cYMin;

	//the delay time between two frame
	static const char m_cDelay;

	//the refresh time for deferent levels
	static const char m_cRefreshTruns[6];

	//the current y-coordinate
	char m_cY;

	//count the turns
	int m_iCount;

	//the current level, related to speed
	char m_cLevel;
	
	//the game over when it's true
	bool m_bOver;

	//the game pause when it's true
	bool m_bPause;

	//the game start when it's true
	bool m_bStart;

	//record the score
	int m_iScore;

	//refresh the screen every frame
	void Refresh();

	//initialize the game
	void Initialize();
public:
	static CStartrek *Instance();
	virtual void Enter(CPlayer* player);
	virtual void Execute(CPlayer* player);
	virtual void Exit(CPlayer* player);
};


/*class CRetroSnake :public CState
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
};*/
