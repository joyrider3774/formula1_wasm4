#include <stdbool.h>
#include "wasm4.h"
#include "helperfuncs.h"

uint32_t PREVGAMEPAD1 = 0;
uint32_t PREVMOUSE_BUTTONS = 0;

//https://www.geeksforgeeks.org/implement-itoa/
// A utility function to reverse a string
void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}

// Implementation of citoa()
char* citoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0) {
        char rem =(char)(num % base);
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}

void setPalette(Palette pal)
{
	PALETTE[0] = pal.Col1;
	PALETTE[1] = pal.Col2;
	PALETTE[2] = pal.Col3;
	PALETTE[3] = pal.Col4;
}

void setPaletteColors(Color col1, Color col2, Color col3, Color col4)
{
	PALETTE[0] = col1;
	PALETTE[1] = col2;
	PALETTE[2] = col3;
	PALETTE[3] = col4;
}

bool buttonReleased(uint32_t Button)
{
	return ((PREVGAMEPAD1 & Button) && (!(*GAMEPAD1 & Button)));
}

bool mouseButtonReleased(uint32_t Button)
{
	return ((PREVMOUSE_BUTTONS & Button) && (!(*MOUSE_BUTTONS & Button)));
}


void setDrawColor(ColorIndex Index1, ColorIndex Index2, ColorIndex Index3, ColorIndex Index4)
{
	*DRAW_COLORS = (uint16_t)((Index1 << 12) + (Index2 << 8) + (Index3 << 4) + (Index4));
}

void getDrawColor(ColorIndex *Index1, ColorIndex *Index2, ColorIndex *Index3, ColorIndex *Index4)
{
	*Index1 = (*DRAW_COLORS & 0xF000) >> 12;
	*Index2 = (*DRAW_COLORS & 0x0F00) >> 8;
	*Index3 = (*DRAW_COLORS & 0x00F0) >> 4;
	*Index4 = (*DRAW_COLORS & 0x0000F) >> 0;
}

void setForeGroundColor(ColorIndex Index)
{
	ColorIndex Index1, Index2, Index3, Index4;
	getDrawColor(&Index1, &Index2, &Index3, &Index4);
	setDrawColor(Index1, Index2, Index, Index4);
}

void setBackGroundColor(ColorIndex Index)
{
	ColorIndex Index1, Index2, Index3, Index4;
	getDrawColor(&Index1, &Index2, &Index3, &Index4);
	setDrawColor(Index1, Index2, Index3, Index);
}

void setBackForeGroundColor(ColorIndex BackgroundIndex, ColorIndex ForGroundIndex)
{
	ColorIndex Index1, Index2, Index3, Index4;
	getDrawColor(&Index1, &Index2, &Index3, &Index4);
	setDrawColor(Index1, Index2, ForGroundIndex, BackgroundIndex);
}

void clear(ColorIndex Index)
{
	ColorIndex Index1, Index2, Index3, Index4;
	getDrawColor(&Index1, &Index2, &Index3, &Index4);
    setDrawColor(Index1, Index2, Index, Index);
	rect(0,0, SCREEN_SIZE, SCREEN_SIZE);
	setDrawColor(Index1, Index2, Index3, Index4);
}

bool mouseInGameBounds()
{
	return (*MOUSE_Y <= SCREEN_SIZE) && (*MOUSE_X <= SCREEN_SIZE) && (*MOUSE_Y >= 0) && (*MOUSE_X >=0);
}

bool anyButtonReleased()
{
	return buttonReleased(BUTTON_1) || buttonReleased(BUTTON_2) || buttonReleased(BUTTON_DOWN) || buttonReleased(BUTTON_LEFT) || buttonReleased(BUTTON_RIGHT) || buttonReleased(BUTTON_UP);
}