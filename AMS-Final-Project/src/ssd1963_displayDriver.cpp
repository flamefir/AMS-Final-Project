/************************************************************
  File name: "TFTdriver.c"

  Driver for "ITDB02 320 x 240 TFT display module, Version 2"
  mounted at "ITDB02 Arduino Mega2560 Shield".
  Display controller = ILI 9341.
  
  Max. uC clock frequency = 1/240 MHz (Tclk = 4.1667 ns)

  Connections:
  DB15-DB8:   PORT A
  DB7-DB0:    PORT C

  RESETx:     PORT G, bit 0
  CSx:        PORT G, bit 1
  WRx:        PORT G, bit 2
  RS (=D/Cx): PORT D, bit 7

  Henning Hargaard, February 18, 2022
************************************************************/  

//#define F_CPU 16000000
#include "ssd1963_displayDriver.hpp"

// Data port definitions:
#define DB0 2
#define DB1 0
#define DB2 4
#define DB3 16
#define DB4 17
#define DB5 5
#define DB6 18
#define DB7 19

// Control port definitions:
#define RS_BIT 34
#define WR_BIT 35
#define RD_BIT 32
#define CS_BIT 27

#define NOP __asm__ __volatile__("nop") //1 NOP = 4.166667 NS

static inline void nop(unsigned n)
{
    while(n--){
        NOP;
    }
}


// Data port definitions:
unsigned char DATABUS[8] = {DB0, DB1, DB2, DB3, DB4, DB5, DB6, DB7};

// LOCAL FUNCTIONS /////////////////////////////////////////////////////////////

// ILI 9341 data sheet, page 238
void WriteCommand(unsigned char command)
{
}

// ILI 9341 data sheet, page 238
void WriteData(unsigned int data)
{
}

// PUBLIC FUNCTIONS ////////////////////////////////////////////////////////////

// Initializes (resets) the display
void DisplayInit()
{
}

void DisplayOff()
{
}

void DisplayOn()
{
}

void SleepOut()
{
}

void MemoryAccessControl(unsigned char parameter)
{
}

void InterfacePixelFormat(unsigned char parameter)
{
}

void MemoryWrite()
{
}

// Red 0-31, Green 0-63, Blue 0-31
void WritePixel(unsigned char Red, unsigned char Green, unsigned char Blue)
{
}

// Set Column Address (0-239), Start > End
void SetColumnAddress(unsigned int Start, unsigned int End)
{
}

// Set Page Address (0-319), Start > End
void SetPageAddress(unsigned int Start, unsigned int End)
{
}

// Fills rectangle with specified color
// (StartX,StartY) = Upper left corner. X horizontal (0-319) , Y vertical (0-239).
// Height (1-240) is vertical. Width (1-320) is horizontal.
// R-G-B = 5-6-5 bits.
void FillRectangle(unsigned int StartX, unsigned int StartY, unsigned int Width, unsigned int Height, unsigned char Red, unsigned char Green, unsigned char Blue)
{
}