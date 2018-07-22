#ifndef ARDUINO
#pragma once
#define WINDOWS_SYSTEM
#endif

//#define DEBUG_RETROSNAKE
//#define DEBUG_GOBANG
//#define DEBUG_TETRIS
//#define DEBUG_STARTREK

#ifndef WINDOWS_SYSTEM
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
#include <Keypad.h>
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
	{ 'E', '1', '2', 'O' },      //R:Restart    r:reset
{ '*', '*', '*', '*' },
{ 'B', 'U', 'N', '*' },
{ 'L', 'D', 'R', 'P' }
};
byte rowPins[ROWS] = { 2, 3, 4, 5 }; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 6, 7, 8, 10 }; //connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


#define CLK 11  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
#define CP_GLOBAL(x)		x//pgm_read_byte(&x)
#define CP_GLOBALSTATE		const static//PROGMEM
#define CP_SLEEP(time) delay(time)
#define CLS			matrix.fillScreen(matrix.Color333(0, 0, 0))
#define CP_PAUSE
#define CP_CLOCK	millis()
#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr
const char str[] PROGMEM = "Welcome!";
int    textX = matrix.width(),
textMin = sizeof(str) * -12,
hue = 500;
int8_t ball[3][4] = {
	{ 3,  0,  1,  1 }, // Initial X,Y pos & velocity for 3 bouncy balls
{ 17, 15,  1, -1 },
{ 27,  4, -1,  1 }
};
static const uint16_t PROGMEM ballcolor[3] = {
	0x0080, // Green=1
	0x0002, // Blue=1
	0x1000  // Red=1
};
void welcome()
{
	matrix.setTextWrap(false); // Allow text to run off right edge
	matrix.setTextSize(2);
	int start = millis();
	while ((millis() - start) <= 5000)
	{
		matrix.fillScreen(0);
		byte i;
		// Bounce three balls around
		for (i = 0; i < 3; i++)
		{
			// Draw 'ball'
			matrix.fillCircle(ball[i][0], ball[i][1], 5, pgm_read_word(&ballcolor[i]));
			// Update X, Y position
			ball[i][0] += ball[i][2];
			ball[i][1] += ball[i][3];
			// Bounce off edges
			if ((ball[i][0] == 0) || (ball[i][0] == (matrix.width() - 1)))
				ball[i][2] *= -1;
			if ((ball[i][1] == 0) || (ball[i][1] == (matrix.height() - 1)))
				ball[i][3] *= -1;
		}
		delay(100);
	}
	while (1)
	{
		// Clear background
		matrix.fillScreen(0);

		// Draw big scrolly text on top
		matrix.setTextColor(matrix.ColorHSV(hue, 255, 255, true));
		matrix.setCursor(textX, 1);
		matrix.print(F2(str));

		// Move text left (w/wrap), increase hue
		if ((--textX) < textMin)
			break;
		hue += 14;
		if (hue >= 1536) hue -= 1536;

		delay(50);
	}
}

void print_retrosnake()
{
	matrix.fillScreen(matrix.Color333(0, 0, 0));

	matrix.setCursor(1, 0);
	matrix.setTextSize(1);

	matrix.setTextColor(matrix.Color333(7, 1, 0));
	matrix.print('R');
	matrix.setTextColor(matrix.Color333(7, 4, 0));
	matrix.print('E');
	matrix.setTextColor(matrix.Color333(7, 7, 0));
	matrix.print('T');
	matrix.setTextColor(matrix.Color333(4, 7, 0));
	matrix.print('R');
	matrix.setTextColor(matrix.Color333(0, 7, 0));
	matrix.print('O');

	matrix.setCursor(1, 9);   // next line
	matrix.setTextColor(matrix.Color333(0, 7, 7));
	matrix.print('S');
	matrix.setTextColor(matrix.Color333(0, 3, 7));
	matrix.print('N');
	matrix.setTextColor(matrix.Color333(0, 0, 7));
	matrix.print('A');
	matrix.setTextColor(matrix.Color333(4, 0, 7));
	matrix.print('K');
	matrix.setTextColor(matrix.Color333(7, 0, 4));
	matrix.print('E');
}

void print_gobang()
{
	matrix.fillScreen(0);

	matrix.setCursor(10, 0);
	matrix.setTextSize(1);

	matrix.setTextColor(matrix.Color333(7, 1, 0));
	matrix.print('G');
	matrix.setTextColor(matrix.Color333(7, 4, 0));
	matrix.print('O');

	matrix.setCursor(4, 9);
	matrix.setTextColor(matrix.Color333(0, 7, 7));
	matrix.print('B');
	matrix.setTextColor(matrix.Color333(0, 4, 7));
	matrix.print('A');
	matrix.setTextColor(matrix.Color333(0, 0, 7));
	matrix.print('N');
	matrix.setTextColor(matrix.Color333(4, 0, 7));
	matrix.print('G');
}

void print_tetris()
{
	matrix.fillScreen(0);

	matrix.setCursor(2, 4);
	matrix.setTextColor(matrix.Color333(0, 7, 7));
	matrix.print('T');
	matrix.setCursor(7, 4);
	matrix.setTextColor(matrix.Color333(0, 4, 7));
	matrix.print('E');
	matrix.setCursor(12, 4);
	matrix.setTextColor(matrix.Color333(0, 0, 7));
	matrix.print('T');
	matrix.setCursor(17, 4);
	matrix.setTextColor(matrix.Color333(4, 0, 7));
	matrix.print('R');
	matrix.setCursor(22, 4);
	matrix.setTextColor(matrix.Color333(7, 0, 4));
	matrix.print('I');
	matrix.setCursor(26, 4);
	matrix.setTextColor(matrix.Color333(7, 0, 3));
	matrix.print('S');
}

void print_sokoban()
{
	matrix.fillScreen(matrix.Color333(0, 0, 0));

	matrix.setCursor(4, 0);   // start at top left, with one pixel of spacing
	matrix.setTextSize(1);    // size 1 == 8 pixels high

	matrix.setTextColor(matrix.Color333(7, 1, 0));
	matrix.print('S');
	matrix.setTextColor(matrix.Color333(7, 4, 0));
	matrix.print('O');
	matrix.setTextColor(matrix.Color333(7, 7, 0));
	matrix.print('K');
	matrix.setTextColor(matrix.Color333(4, 7, 0));
	matrix.print('O');

	matrix.setCursor(7, 9);   // next line
	matrix.setTextColor(matrix.Color333(0, 7, 7));
	matrix.print('B');
	matrix.setTextColor(matrix.Color333(0, 4, 7));
	matrix.print('A');
	matrix.setTextColor(matrix.Color333(0, 0, 7));
	matrix.print('N');
}

void print_startrek()
{
	matrix.fillScreen(matrix.Color333(0, 0, 0));

	matrix.setCursor(4, 0);
	matrix.setTextSize(1);

	matrix.setTextColor(matrix.Color333(7, 4, 0));
	matrix.print('S');
	matrix.setTextColor(matrix.Color333(7, 7, 0));
	matrix.print('T');
	matrix.setTextColor(matrix.Color333(4, 7, 0));
	matrix.print('A');
	matrix.setTextColor(matrix.Color333(0, 7, 0));
	matrix.print('R');

	matrix.setCursor(4, 9);   // next line
	matrix.setTextColor(matrix.Color333(0, 7, 7));
	matrix.print('T');
	matrix.setTextColor(matrix.Color333(0, 4, 7));
	matrix.print('R');
	matrix.setTextColor(matrix.Color333(0, 0, 7));
	matrix.print('E');
	matrix.setTextColor(matrix.Color333(4, 0, 7));
	matrix.print('K');
}

void print_backtomenu()
{
	matrix.fillScreen(matrix.Color333(0, 0, 0));

	matrix.setCursor(0, 0);
	matrix.setTextSize(1);
	matrix.setTextColor(matrix.Color333(7, 0, 7));
	matrix.print('B');
	matrix.setCursor(5, 0);
	matrix.print('a');
	matrix.setCursor(10, 0);
	matrix.print('c');
	matrix.setCursor(15, 0);
	matrix.print('k');
	matrix.setCursor(21, 0);
	matrix.print('T');
	matrix.setCursor(26, 0);
	matrix.print('o');

	matrix.setCursor(4, 9);
	matrix.print("Menu");
}

void print_speed(char Speed)
{
	matrix.fillScreen(matrix.Color333(0, 0, 0));

	matrix.setCursor(0, 0);
	matrix.setTextSize(1);
	matrix.setTextColor(matrix.Color333(7, 0, 7));
	matrix.print("SPEED");

	matrix.setCursor(12, 9);
	matrix.print((char)(48 + Speed));
}
void print_wall(char Wall)
{
	matrix.fillScreen(matrix.Color333(0, 0, 0));

	matrix.setCursor(4, 0);
	matrix.setTextSize(1);
	matrix.setTextColor(matrix.Color333(7, 0, 7));
	matrix.print("WALL");

	matrix.setCursor(8, 9);
	matrix.print(Wall ? "ON" : "OFF");
}

void print_levelup(char Levelup)
{
	matrix.fillScreen(matrix.Color333(0, 0, 0));

	matrix.setCursor(1, 0);
	matrix.setTextSize(1);
	matrix.setTextColor(matrix.Color333(7, 0, 7));
	matrix.print("LEVEL");

	matrix.setCursor(8, 9);
	matrix.print(Levelup ? "ON" : "OFF");
}

void print_double(char Double)
{
	matrix.fillScreen(matrix.Color333(0, 0, 0));

	matrix.setCursor(8, 0);
	matrix.setTextSize(1);
	matrix.setTextColor(matrix.Color333(7, 0, 7));
	matrix.print("PC2");

	matrix.setCursor(8, 9);
	matrix.print(Double ? "ON" : "OFF");
}
void print_score(int score)
{
	matrix.setRotation(3);
	matrix.fillRect(0, 23, 16, 7, 0);

	matrix.setTextSize(1);
	matrix.setTextColor(matrix.Color333(7, 0, 7));
	if (score < 10)
	{
		matrix.setCursor(5, 23);
		matrix.print(score);
	}
	else if (score < 100)
	{
		matrix.setCursor(2, 23);
		matrix.print(score);
	}
	else
	{
		matrix.setCursor(0, 23);
		matrix.print(score / 100);
		matrix.setCursor(5, 23);
		matrix.print((score / 10) % 10);
		matrix.setCursor(10, 23);
		matrix.print(score % 10);
	}
	matrix.setRotation(0);
}

unsigned char m_ggiPrint[16][32];
unsigned char m_ggiPrintSave[16][32];
#else

#include<iostream>
#include<conio.h>
#include<windows.h>
#include<time.h>
#define CP_GLOBAL(x)	x
#define CP_GLOBALSTATE	const static	
#define CP_SLEEP(time)	_sleep(time)
#define CP_PAUSE		system("pause")
#define CP_CLOCK		clock()
#define CLS				system("cls")
#endif

#ifndef ARDUINO
#pragma once
#endif

#define iYEDGE  (16)
#define iXEDGE  (32)
#define REFRESHTIME (150)


enum keyin
{
	NOOPERATION,//no operation
	UP,		//for game pad
	UP2,	//for joysticks
	DOWN,	//down
	DOWN2,
	LEFT,	//left
	LEFT2,
	RIGHT,	//right
	RIGHT2,
	OK,		//ensure the choice
	CANCEL,	//cancel present operation(maybe no use)
	START,	//start the game
	PAUSE,	//pause the game
	EXIT,	//exit the game
	RESTART,//restart this round
	BACK,	//back to last menu
	RESET,	//reload this game
	RIGHTUP,//these four direction not neccessary, only for some judge
	RIGHTDOWN,
	LEFTUP,
	LEFTDOWN,
	NEXT,



};

enum Game_Name
{
	NONE,
	RETROSNAKE,
	GOBANG,
	TETRIS,
	SOKOBAN,
	STARTREK,

	END_LABEL,	//just to label the end of enum
	DRETROSNAKE //just a flag to print, shouldn't be counted
};

enum item
{
	BLANK,
	FOOD,
	SNAKE,

	HIGHLIGHT,
	BLACKCHESS,
	WHITECHESS,
	BLACKHIGHLIGHT,
	WHITEHIGHLIGHT,

	BLOCK,
	EDGE,

	PATH,
	OBSTACLE,
	PERSON,
	BOX,
	AIM,
	EMPTY_LABEL,
	PERSON_AIM,
	BOX_AIM,

	FIGHTER,
	BULLET,


	SNAKE2 = 129
};

enum music
{
	NOMUSIC,
	BGM,
	CHESSDOWN,
	GETSCORE,
	OPERATE,

};

#ifdef WINDOWS_SYSTEM
inline char Dot(int n)
{
	char tem;
	switch (n)
	{
	case BLANK:
		tem = ' ';
		break;
	case SNAKE:
		tem = '0';
		break;
	case FOOD:
		tem = '8';
		break;
	case BLACKCHESS:
		tem = 'x';
		break;
	case WHITECHESS:
		tem = 'y';
		break;
	case HIGHLIGHT:
		tem = '?';
		break;
	case BLACKHIGHLIGHT:
		tem = 'X';
		break;
	case WHITEHIGHLIGHT:
		tem = 'Y';
		break;
	case BLOCK:
		tem = '*';
		break;
	case EDGE:
		tem = '#';
	case OBSTACLE:
		tem = 'O';
		break;
	case PERSON:
		tem = 'p';
		break;
	case BOX:
		tem = 'b';
		break;
	case AIM:
		tem = '#';
		break;
	case PERSON_AIM:
		tem = 'P';
		break;
	case BOX_AIM:
		tem = 'B';
		break;
	case SNAKE2:
		tem = '*';
		break;
	case FIGHTER:
		tem = '5';
		break;
	case BULLET:
		tem = 'x';
		break;
	default:
		tem = ' ';
		break;
	}
	return tem;
}
#else
inline unsigned long long Dot(int n)
{
	unsigned long long tem;
	switch (n)
	{
	case BLANK:
		tem = 0;
		break;
	case SNAKE:
		tem = matrix.Color333(7, 0, 7);
		break;
	case SNAKE2:
		tem = matrix.Color333(7, 7, 0);
		break;
	case FOOD:
		tem = matrix.Color333(0, 0, 7);
		break;
	case BLACKCHESS:
		tem = matrix.Color333(7, 0, 0);
		break;
	case WHITECHESS:
		tem = matrix.Color333(0, 0, 7);
		break;
	case HIGHLIGHT:
		tem = matrix.Color333(0, 7, 0);
		break;
	case BLACKHIGHLIGHT:
		tem = matrix.Color333(7, 7, 0);
		break;
	case WHITEHIGHLIGHT:
		tem = matrix.Color333(0, 7, 7);
		break;
	case BLOCK:
		tem = matrix.Color333(0, 127, 0);
		break;
	case EDGE:
		tem = matrix.Color333(127, 0, 0);
		break;
	case PATH:
		tem = matrix.Color333(0, 0, 0);
		break;
	case OBSTACLE:
		tem = matrix.Color333(0, 0, 127);
		break;
	case PERSON:
		tem = matrix.Color333(0, 127, 0);
		break;
	case BOX:
		tem = matrix.Color333(7, 7, 0);
		break;
	case AIM:
		tem = matrix.Color333(7, 0, 7);
		break;
	case PERSON_AIM:
		tem = matrix.Color333(127, 127, 0);
		break;
	case BOX_AIM:
		tem = matrix.Color333(127, 0, 0);
		break;

	default:
		tem = 255;
		break;
	}
	return tem;
}
#endif


inline void Print(int GameNow, unsigned char now[iYEDGE][iXEDGE], unsigned char past[iYEDGE][iXEDGE])
{
	for (int y = 0; y < iYEDGE; y++)
		for (int x = 0; x < iXEDGE; x++)
		{
			if (now[y][x] != past[y][x])
			{
#ifdef  WINDOWS_SYSTEM
				static HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
				COORD coord;
				if (GameNow == TETRIS)
				{
					coord.X = y;
					coord.Y = x;
				}
				else if (GameNow == STARTREK)
				{
					coord.X = iYEDGE - y;
					coord.Y = x;
				}
				else
				{
					coord.X = x;
					coord.Y = y;
				}
				SetConsoleCursorPosition(hout, coord);
				if (GameNow == DRETROSNAKE && now[y][x] >= SNAKE2)
					putchar(Dot(SNAKE2));
				else if ((GameNow == RETROSNAKE || GameNow == DRETROSNAKE) && now[y][x] >= SNAKE)
					putchar(Dot(SNAKE));
				else
					putchar(Dot(now[y][x]));
#else
				//special condition for RetroSnake
				if (GameNow == DRETROSNAKE && now[y][x] >= SNAKE2)
					matrix.drawPixel(x, y, Dot(SNAKE2));
				else if ((GameNow == RETROSNAKE || GameNow == DRETROSNAKE) && now[y][x] >= SNAKE)
					matrix.drawPixel(x, y, Dot(SNAKE));
				else if (GameNow == TETRIS)
					matrix.drawPixel(x, iYEDGE - 1 - y, Dot(now[y][x]));
				else
				{
					matrix.drawPixel(x, y, Dot(now[y][x]));
				}
				Serial.println("!");
#endif
			}
		}
#ifdef WINDOWS_SYSTEM
	static HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord = { 0,iYEDGE };
	SetConsoleCursorPosition(hout, coord);
#endif
}

inline keyin PressKey(bool stickpermitted = true, bool stickpermitted2 = true)
{
	keyin KeyPressed = NOOPERATION;
#ifdef WINDOWS_SYSTEM
	unsigned char tem = '\0';
	if (_kbhit())
		tem = _getch();
	/*if (flush)
	while (_kbhit())
	_getch();*/
	switch (tem)
	{
	case'a':
		if (stickpermitted)
			KeyPressed = LEFT;
		break;
	case's':
		if (stickpermitted)
			KeyPressed = DOWN;
		break;
	case'd':
		if (stickpermitted)
			KeyPressed = RIGHT;
		break;
	case'w':
		if (stickpermitted)
			KeyPressed = UP;
		break;
	case'\r':
		KeyPressed = OK;
		break;
	case'x':
		KeyPressed = CANCEL;
		break;
	case'z':
		KeyPressed = START;
		break;
	case'p':
		KeyPressed = PAUSE;
		break;
	case 27://ESC
		KeyPressed = EXIT;
		break;
	case'r':
		KeyPressed = RESTART;
		break;
	case'\b':
		KeyPressed = BACK;
		break;
	case' ':
		KeyPressed = RESET;
		break;
	case 'q':
		KeyPressed = BACK;
		break;
	case 'e':
		KeyPressed = NEXT;
		break;
	case 224://for direction key
		tem = _getch();
		if (stickpermitted2)
			switch (tem)
			{
			case 72:
				KeyPressed = UP2;
				break;
			case 80:
				KeyPressed = DOWN2;
				break;
			case 75:
				KeyPressed = LEFT2;
				break;
			case 77:
				KeyPressed = RIGHT2;
				break;
			}
		break;
	default:
		KeyPressed = NOOPERATION;
		break;
	}
#else

	if (stickpermitted2)
	{
		int x = analogRead(A4);
		int y = analogRead(A5);
		if (y >= 1000)
			KeyPressed = LEFT2;
		else if (y <= 200)
			KeyPressed = RIGHT2;
		else if (x <= 200)
			KeyPressed = UP2;
		else if (x >= 1000)
			KeyPressed = DOWN2;
	}
	if (stickpermitted)
	{
		int x = analogRead(A6);
		int y = analogRead(A7);
		if (y >= 1000)
			KeyPressed = LEFT;
		else if (y <= 200)
			KeyPressed = RIGHT;
		else if (x <= 200)
			KeyPressed = UP;
		else if (x >= 1000)
			KeyPressed = DOWN;
	}
	char key = keypad.getKey();
	if (key == 'U')
		KeyPressed = UP;
	else if (key == 'D')
		KeyPressed = DOWN;
	else if (key == 'L')
		KeyPressed = LEFT;
	else if (key == 'R')
		KeyPressed = RIGHT;
	else if (key == 'O')
		KeyPressed = OK;
	else if (key == 'E')
		KeyPressed = EXIT;
	else if (key == '1')
		KeyPressed = RESTART;
	else if (key == '2')
		KeyPressed = RESET;
	else if (key == 'P')
		KeyPressed = PAUSE;
	else if (key == 'B')
		KeyPressed = BACK;
	else if (key == 'N')
		KeyPressed = NEXT;
#endif
	return KeyPressed;
}
