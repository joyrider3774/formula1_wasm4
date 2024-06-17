#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "wasm4.h"
#include "sound.h"


//helper defines
#define PAUSE 235

uint8_t music_note, music_tempo, music_loop, music_on, sound_on, sfx_sustain,prev_music;
uint8_t sfx_setting_note, sfx_sustain_zero_set, selecting_music;
uint16_t musicArray[255];
unsigned long music_length;

const uint16_t music_failed[] ={
    850,PAUSE*2,
    0,PAUSE,
    850,PAUSE*1,
	0,PAUSE,
	850,PAUSE*1,
	0,PAUSE,
	850,PAUSE*2,
	0,PAUSE,
};


void setMusicOn(uint8_t value)
{
    music_on = value;
}

void processSound()
{
    if (sfx_setting_note || selecting_music)
    {
        return;
    }
    
    musicTimer();
    if(sfx_sustain == 0)
    {
        if (!sfx_sustain_zero_set)
        {
            tone(0, sfx_sustain, 0, TONE_PULSE1);
            sfx_sustain_zero_set = 1;
        }
    }
    else
    {
        sfx_sustain--;
    }
}


void SelectMusic(uint8_t musicFile, uint8_t loop)
{
    if (prev_music != musicFile)
    {
        selecting_music = 1;
		prev_music = musicFile;
		memset(musicArray, 0, sizeof(musicArray));
		switch (musicFile) 
		{			
			case musFailed:
				memcpy(musicArray, music_failed, sizeof(music_failed));
				music_length = sizeof(music_failed) / sizeof(uint16_t);
				break;
			default:
				break;
		}
		music_note = 0;
		music_tempo = 0;
		music_loop = loop;
        selecting_music = 0;
    }
}

void playNote()
{    
    if(music_note < music_length)
    {
        tone(musicArray[music_note], 100, 50, TONE_PULSE2 | TONE_MODE4);

        //Set the new delay to wait
        music_tempo = musicArray[music_note + 1] * 60/1000;

        //Skip to the next note
        music_note += 2;
               
        if (music_note > music_length - 1)
        {
            if(music_loop)
            {
                music_note = 0;
            }
        }
    }
}


void musicTimer()
{
    //for nintendo systems as sega one checks it earlier
    if (selecting_music)
    {
        return;
    }

    //Play some music
    if (music_tempo == 0)
    {
        if(music_on)
        {
            playNote();
        }
    }
    //Else wait for the next note to play
    else 
    {
        music_tempo--;        
    }
}

void initMusic()
{
	prev_music = 0;
	music_note = 0;
	music_length = 0;
	music_tempo = 0;
	music_loop = 0;
	//set to 1 so nothing plays until a music was selected
	selecting_music = 1;
}

void setSoundOn(uint8_t value)
{
    sound_on = value;
}

uint8_t isMusicOn()
{
    return music_on;
}

uint8_t isSoundOn()
{
    return sound_on;
}

void initSound()
{
    sound_on = 0;
}

void playSound(uint16_t toneFreq)
{
    if(!sound_on)
	{
        return;
	}
    tone(toneFreq, 5, 100, TONE_PULSE1);
}


void playTickSound()
{
    playSound(750);
}

