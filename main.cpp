#ifndef ARDUINO
#include"player.h"
#endif

CPlayer player;
#ifdef WINDOWS_SYSTEM
int main()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO CursorInfo;

	GetConsoleCursorInfo(handle, &CursorInfo);

	CursorInfo.bVisible = false; 

	SetConsoleCursorInfo(handle, &CursorInfo);
	while (true)
	{
		player.Update();
	}
    return 0;
}
#else
void setup()
{
	matrix.begin();
	welcome();
	Serial.begin(9600);
}
void loop()
{
	player.Update();
}
#endif
