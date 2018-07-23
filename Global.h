#pragma once
//------------------------------------------------------------------------
//
//	Name:	Global.h
//
//	Desc:	defining some grands and global variables for the whole games
//
//------------------------------------------------------------------------

//defining some grands to debug
//#define DEBUG_RETROSNAKE
//#define DEBUG_GOBANG
//#define DEBUG_TETRIS
//#define DEBUG_SOKOBAN
//#define DEBUG_STARTREK


//two global variables grids for screen
extern unsigned char g_ggcPrint[16][32];
extern unsigned char g_ggcPrintSave[16][32];

//here define some const number
#define iYEDGE  (16)
#define iXEDGE  (32)
#define REFRESHTIME (100)

//here defining some keys involved
enum keyin
{
	//no operation
	NOOPERATION,
	UP,		//for game pad
	UP2,	//for joysticks
	DOWN,	//down
	DOWN2,
	LEFT,	//left
	LEFT2,
	RIGHT,	//right
	RIGHT2,
	RIGHTUP,//these four direction not neccessary, only for some judge
	RIGHTDOWN,
	LEFTUP,
	LEFTDOWN,

	//Ensure the choice
	OK,
	//Cancel present operation or withdraw
	CANCEL,
	//Start or go on the game
	START,
	//Pause the game
	PAUSE,
	//Exit the game
	EXIT,
	//Restart this round
	RESTART,
	//Back to the last menu or level
	BACK,
	//Get to the next one
	NEXT,
	//Reload this game
	RESET,


};

//all the games'name in this programme
enum Game_Name
{
	MAIN,
	RETROSNAKE,
	GOBANG,
	TETRIS,
	SOKOBAN,
	STARTREK,

	//Label the end of enum
	END_LABEL,
	//Just a flag to print, shouldn't be counted
	DRETROSNAKE
};

//every pixel should be an item
enum item
{
	//Basic blank
	BLANK,

	//These two for RetroSnake
	FOOD,
	SNAKE,

	//These five for Gobang
	HIGHLIGHT,
	BLACKCHESS,
	WHITECHESS,
	BLACKHIGHLIGHT,
	WHITEHIGHLIGHT,


	//These two for Tetris
	BLOCK,
	EDGE,

	//These are for Sokoban
	PATH,
	OBSTACLE,
	PERSON,
	BOX,
	AIM,
	PERSON_AIM,
	BOX_AIM,

	//These two for Startrek
	FIGHTER,
	BULLET,


	//This is the second snake
	SNAKE2 = 129
};


