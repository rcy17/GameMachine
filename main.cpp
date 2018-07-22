#include"player.h"

CPlayer player;

#ifdef WIN32
int main()
{
	//get the handle of standard output
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	//declear a structure to save the cursor information
	CONSOLE_CURSOR_INFO CursorInfo;
	//give the current cursor information to the structure
	GetConsoleCursorInfo(handle, &CursorInfo);
	//make the cursor unvisible
	CursorInfo.bVisible = false; 
	//give the handle the new information
	SetConsoleCursorInfo(handle, &CursorInfo);
	while (true)
	{
		player.Update();
	}
    return 0;
}
#endif
