#include"player.h"


CPlayer player;
#ifdef WIN32
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
#endif
