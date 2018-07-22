#ifndef ARDUINO
#include"music.h"
void MusicManage(music Music)
{
	;
}

#else
int tuneJLJT[] =
{
	NOTE_D1, NOTE_D1, NOTE_D1, NOTE_D3, NOTE_D5,
	NOTE_D6, NOTE_D0, NOTE_D0, NOTE_D5, NOTE_D6, NOTE_D0, NOTE_D0, NOTE_D5,
	NOTE_D6, NOTE_DH1, NOTE_D5, NOTE_D6, NOTE_D3, NOTE_D0, NOTE_D3, NOTE_D5,
	NOTE_D6, NOTE_D0, NOTE_D0, NOTE_D5, NOTE_D6, NOTE_D0, NOTE_D0, NOTE_D5,
	NOTE_D6, NOTE_DH3, NOTE_DH1, NOTE_DH2, NOTE_D6, NOTE_D0, NOTE_D3, NOTE_D5,
	NOTE_D6, NOTE_D0, NOTE_D0, NOTE_D5, NOTE_D6, NOTE_D0, NOTE_D0,
	NOTE_D5,
	NOTE_D6, NOTE_DH1, NOTE_D5, NOTE_D6, NOTE_D3, NOTE_D5, NOTE_D1, NOTE_D2,
	NOTE_D3, NOTE_DH1, NOTE_D6, NOTE_DH3,
	NOTE_DH2, NOTE_DH3, NOTE_DH2, NOTE_DH1, NOTE_DH2, NOTE_D6, NOTE_D0,
	NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_DH1, NOTE_DH2, NOTE_DH3,
	NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D6,
	/*NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_DH1, NOTE_DH2, NOTE_DH3,
	NOTE_D6, NOTE_D6, NOTE_D6, NOTE_DH4, NOTE_DH4, NOTE_DH3,
	NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_DH1, NOTE_DH2, NOTE_DH3,
	NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D6,
	NOTE_D6, NOTE_D6, NOTE_D6, NOTE_D6, NOTE_DH1, NOTE_DH2, NOTE_DH3,
	NOTE_DH6, NOTE_D5, NOTE_D5, NOTE_D6, NOTE_D6,
	NOTE_D0, NOTE_D0, NOTE_D6, NOTE_D6, NOTE_D5, NOTE_D5, NOTE_D5, NOTE_D6,
	NOTE_D5, NOTE_D3, NOTE_D3, NOTE_D5, NOTE_D3, NOTE_D3, NOTE_D0, NOTE_D5,
	NOTE_D5, NOTE_D6, NOTE_D0, NOTE_D5, NOTE_D5, NOTE_D6, NOTE_D0, NOTE_D6, NOTE_D7,
	NOTE_DH1, NOTE_DH2, NOTE_D6, NOTE_D3, NOTE_D3, NOTE_D5,
	NOTE_D5, NOTE_D6, NOTE_D0, NOTE_DH3, NOTE_DH2, NOTE_D0, NOTE_D6,
	NOTE_D0, NOTE_D6, NOTE_D6, NOTE_DH3, NOTE_DH2, NOTE_D0,
	NOTE_D0, NOTE_D0, NOTE_D0, NOTE_D0,*/
};

float durationJLJT[] =
{
	1, 1, 1, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
	0.5,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
	1, 1, 1, 1,
	0.5, 0.5, 0.5, 0.5, 0.5, 1, 1,
	1, 1, 1, 0.25, 0.25, 0.25, 0.25,
	1, 1, 0.5, 0.5, 0.5, 0.5,
	/*1, 1, 1, 0.25, 0.25, 0.25, 0.25,
	1, 1, 0.5, 0.5, 0.5, 0.5,
	1, 1, 1, 0.25, 0.25, 0.25, 0.25,
	1, 1, 0.5, 0.5, 0.5, 0.5,
	1, 1, 1, 0.25, 0.25, 0.25, 0.25,
	1, 0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1,
	1, 0.5, 0.25, 0.25, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 1, 0.5, 0.5, 0.5, 0.5, 0.5,
	0.5, 0.5, 0.5, 1, 0.5, 0.5,
	1, 1, 1, 1,*/
};//½ÚÅÄ
int tonePin = 13; //·äÃùÆ÷µÄpin
int tuneOper[] = { NOTE_D3 };
float durationOper[] = { 0.25 };
int tuneSet[] = { NOTE_D2, NOTE_D1 };
float durationSet[] = { 0.25, 0.5 };
int tuneScore[] = { NOTE_D5, NOTE_DH3 };
float durationScore[] = { 0.25, 1 };
void MusicManage(music Music)
{
	static auto finish = CP_CLOCK;
	static music MusicNow = NOMUSIC;
	static int *pMusic = NULL;
	static float *pDuration = NULL;
	static int SaveLocation = 0;
	static int length = 0;
	if (Music == MusicNow)
	{
		if (Music == NOMUSIC)
			return;
		if (CP_CLOCK >= finish)
		{
			noTone(tonePin);
			SaveLocation++;
			if (SaveLocation == length)
			{
				SaveLocation = 0;
				if (MusicNow != BGM)
				{
					MusicNow = NOMUSIC;
					return;
				}

			}
			tone(tonePin, pMusic[SaveLocation]);
			finish = CP_CLOCK + pDuration[SaveLocation] * 400;
		}
	}
	else
	{
		switch (Music)
		{
		case BGM:
			pMusic = tuneJLJT;
			pDuration = durationJLJT;
			length = sizeof(tuneJLJT) / sizeof(tuneJLJT[0]);
			break;
		case CHESSDOWN:
			pMusic = tuneSet;
			pDuration = durationSet;
			length = sizeof(tuneSet) / sizeof(tuneSet[0]);
			break;
		case GETSCORE:
			pMusic = tuneScore;
			pDuration = durationScore;
			length = sizeof(tuneScore) / sizeof(tuneScore[0]);
			break;
		case OPERATE:
			pMusic = tuneOper;
			pDuration = durationOper;
			length = sizeof(tuneOper) / sizeof(tuneOper[0]);
			break;
		default:
			pMusic = NULL;
			pDuration = NULL;
			length = 0;
			return;
		}

		SaveLocation = 0;
		MusicNow = Music;
		noTone(tonePin);
		tone(tonePin, pMusic[SaveLocation]);
		finish = CP_CLOCK + pDuration[SaveLocation] * 400;
	}


}

#endif
