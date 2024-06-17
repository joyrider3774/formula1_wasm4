#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <string.h>
#include "helperfuncs.h"
#include "graphics.h"
#include "wasm4.h"
#include "sound.h"

#define GSQuit 0 
#define GSIntro 1 
#define GSGame 2 
#define GSGameOver 3

#define GSInitDiff 50

#define GSIntroInit (GSIntro + GSInitDiff)
#define GSGameInit (GSGame + GSInitDiff)
#define GSGameOverInit (GSGameOver + GSInitDiff)

int GameState = GSIntroInit;
bool EnemyStates[3][3];
bool PlayerStates[3];
bool TvOutMode = false;
int HitPosition, LivesLost;
long Score = 0, HiScore = 0;
uint32_t framesdrawn = 0;
int Teller,FlashesDelay,Flashes,X,Delay;
bool CanMove,CrashSoundPlayed;
	
void LoadSettings()
{
	diskr(&HiScore, sizeof(HiScore));
}

// Save the settings
void SaveSettings()
{
 	diskw(&HiScore, sizeof(HiScore));
}

void setHiScore(long value)
{
	if(value > HiScore)
	{
    	HiScore = value;
    	SaveSettings();
	}
}

void DrawScoreBar(bool Empty, long ScoreIn, long HighScoreIn, int LivesLostIn)
{
	if(Empty)
	 	return;
	setDrawColor(0,0,0,4);
	char ScoreStr[20];
	citoa(ScoreIn, ScoreStr, 10);
	if (strlen(ScoreStr) > 0)
	{
		text(ScoreStr, 21,2);
	}
	citoa(HighScoreIn, ScoreStr, 10);
	if (strlen(ScoreStr) > 0)
	{
		text(ScoreStr, 65,2);
	}
	if (LivesLostIn >=1)
	{
		for(int X = 0;X<LivesLostIn;X++)
			ScoreStr[X] = 'X';
		ScoreStr[LivesLostIn] = 0;
		text(ScoreStr, 115,2);
	}
}

void DrawGame()
{
	//(cockpit, background, body,tires)
	setDrawColor(2,1,3,4);
	int X,Y;
	for (X=0;X<3;X++)
		for (Y=0;Y<3;Y++)
			if (EnemyStates[X][Y])
			{
				blit(enemy,27 + (X * 42), 16 + (Y * 40), enemyWidth, enemyHeight, enemyFlags);
			}
	for (X=0;X<3;X++)
	{
		if (PlayerStates[X])
		{
			blit(player,27 + (X * 42), 130, playerWidth, playerHeight, playerFlags);
		}
	}
}

void MoveEnemy()
{
	int X,Y;
	for (X=0;X<3;X++)
		for (Y=2;Y>=1;Y--)
			EnemyStates[X][Y] = EnemyStates[X][Y-1];
	for (X=0;X<3;X++)
		EnemyStates[X][0] = false;
	for (X=0;X<=1;X++)
		EnemyStates[rand()%3][0] = true;
}

void MoveLeft()
{
	int X;
	for (X=0;X<2;X++)
	{
		if (PlayerStates[X+1])
		{
			PlayerStates[X] = true;
			PlayerStates[X+1] = false;
		}
	}
}

void MoveRight()
{
	int X;
	for (X=2;X>=1;X--)
	{
		if (PlayerStates[X-1])
		{
			PlayerStates[X] = true;
			PlayerStates[X-1] = false;
		}
	}
}

bool IsCollided()
{
	int X;
	bool Temp;
	Temp=false;
	for (X = 0;X<3;X++)
		if (PlayerStates[X] && EnemyStates[X][2])
		{
			Temp = true;
			HitPosition = X;
		}
	return Temp;
}

void HitFlash()
{
	PlayerStates[HitPosition] = !PlayerStates[HitPosition];
	EnemyStates[HitPosition][2] = !EnemyStates[HitPosition][2];
}

void InitialiseStates()
{
	int X,Y;
	for(X = 0;X<3;X++)
		for(Y=0;Y<3;Y++)
			EnemyStates[X][Y] = false;
	for(X=0;X<3;X++)
		PlayerStates[X] = false;
	PlayerStates[1] = true;
}

void GameInit()
{
	srand(framesdrawn);
	Teller = 25;
	FlashesDelay = 14;
	Flashes = 0;
	CanMove = true;
	Score = 0;
	Delay = 60;
	LivesLost = 0;
	CrashSoundPlayed = false;
	InitialiseStates();
}

void Game()
{
	if(GameState == GSGameInit)
	{
		GameInit();
		GameState -= GSInitDiff;
	}

	if(buttonReleased(BUTTON_LEFT) || mouseButtonReleased(MOUSE_LEFT))
		if(CanMove)
			MoveLeft();
	if(buttonReleased(BUTTON_RIGHT) || mouseButtonReleased(MOUSE_RIGHT))		
		if(CanMove)
			MoveRight();

	setDrawColor(3,4,2,1);		
	blit(background, 0, 0, backgroundWidth, backgroundHeight, backgroundFlags);
	Teller++;
	if (Teller >= Delay)
	{
		if (!IsCollided() && CanMove)
		{
			Teller = 0;
			for (X = 0;X < 3;X++)
				if (EnemyStates[X][2])
				{
					Score += 10;
					setHiScore(Score);
					if ((Score % 100 ==0) && (Delay > 18))
						Delay--;
				}
			MoveEnemy();
			playTickSound();
		}
		else
		{
			if (!CrashSoundPlayed)
			{
				SelectMusic(musNone, 0);
				SelectMusic(musFailed, 0);
				CrashSoundPlayed = true;
			}
			CanMove = false;
			FlashesDelay++;
			if (FlashesDelay == 30)
			{
				Flashes++;
				HitFlash();
				FlashesDelay = 0;
				if (Flashes == 6)
				{
					Flashes = 0;
					CanMove = true;
					Teller = 0;
					CrashSoundPlayed=false;
					EnemyStates[HitPosition][2] = false;
					LivesLost++;
					FlashesDelay = 14;
					if (LivesLost ==3)
						GameState = GSGameOverInit;
				}
			}
		}
	}
	DrawGame();
	DrawScoreBar(false, Score, HiScore, LivesLost);
}

void GameOverInit()
{

}

void GameOver()
{
	if(GameState == GSGameOverInit)
	{
		GameOverInit();
		GameState -= GSInitDiff;
	}

	if(buttonReleased(BUTTON_LEFT) || buttonReleased(BUTTON_RIGHT) || buttonReleased(BUTTON_UP) ||
		buttonReleased(BUTTON_DOWN) || buttonReleased(BUTTON_1) || buttonReleased(BUTTON_2) || 
		mouseButtonReleased(MOUSE_LEFT) || mouseButtonReleased(MOUSE_RIGHT))
		GameState = GSGameInit;
	setDrawColor(3,4,2,1);
	blit(background, 0, 0, backgroundWidth, backgroundHeight, backgroundFlags);
	DrawGame();
	DrawScoreBar(false, Score, HiScore, LivesLost);
}

void FlashIntro()
{
	int X,Y;
	for (X = 0;X < 3;X++)
 		for (Y = 0;Y<3;Y++)
 			EnemyStates[X][Y] = ! EnemyStates[X][Y];
 	for (X=0;X<3;X++)
 		PlayerStates[X] = ! PlayerStates[X];
}

void IntroInit()
{
	FlashesDelay = 0;
}

void Intro()
{
	if(GameState == GSIntroInit)
	{
		IntroInit();
		GameState -= GSInitDiff;
	}

	if(buttonReleased(BUTTON_LEFT) || buttonReleased(BUTTON_RIGHT) || buttonReleased(BUTTON_UP) ||
		buttonReleased(BUTTON_DOWN) || buttonReleased(BUTTON_1) || buttonReleased(BUTTON_2) || 
		mouseButtonReleased(MOUSE_LEFT) || mouseButtonReleased(MOUSE_RIGHT))
		GameState = GSGameInit;
	setDrawColor(3,4,2,1);
	blit(background, 0, 0, backgroundWidth, backgroundHeight, backgroundFlags);
	FlashesDelay++;
	if (FlashesDelay == 25)
	{
		FlashesDelay = 0;
		FlashIntro();
	}
	DrawGame();
	DrawScoreBar(!PlayerStates[0], 88888, 88888, 3);
}

void update()
{
	processSound();
	switch(GameState)
	{
		case GSIntroInit:
		case GSIntro :
			Intro();
			break;
		case GSGameInit:
		case GSGame :
			Game();
			break;
		case GSGameOverInit:
		case GSGameOver:
			GameOver();
			break;
		default:
			break;
	}
	PREVGAMEPAD1 = *GAMEPAD1;
    PREVMOUSE_BUTTONS = *MOUSE_BUTTONS;
	framesdrawn++;
}

void start()
{
	setPaletteColors(0xacacac, 0xf3f3f3, 0x484848, 0x000000);
	LoadSettings();
	setSoundOn(true);
	setMusicOn(true);
}