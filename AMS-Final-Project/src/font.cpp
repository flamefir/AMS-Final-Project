#include "font.hpp"
#include <string.h>

font::font(){

}

font::~font(){
    
}

bool font::charToPixels8(char letter, char * instring){

	int ascii_value = (int)letter; // Get the ASCII value of the letter
    char pattern[8] = {};
    for (int i = 0; i < 8; i++) {
        pattern[i] = myFont_[ascii_value][i]; // Retrieve the binary pattern from myFont
    }

    int index = 0;

    for (int i = 0; i < 8; i++) {
        for (int j = 7; j >= 0; j--) {
            unsigned char bit = (pattern[i] >> j) & 0b00000001; // Get the j-th bit of the i-th byte

            if (bit != 0) {
                instring[index] = '#';
            } else {
                instring[index] = '.';
            }
            index++;
            if (index >= 64) // Check if index exceeds the array size
            break;
        }
    }

    instring[index] = '\0'; // Add null terminator at the end

    return 0;
}

bool font::charToPixels16(char letter, char * instring)
{
	int ascii_value = ((int)letter - 32 >= 0) ? (int)letter - 32 : (int)letter; // Get the ASCII value of the letter

    char pattern[32] = {};
    for (int i = 0; i < 32; i++) {
        pattern[i] = myFont16_[ascii_value][i]; // Retrieve the binary pattern from myFont
    }

    int index = 0;

    for (int i = 0; i < 32; i++) {
        for (int j = 7; j >= 0; j--) {
            unsigned char bit = (pattern[i] >> j) & 0b00000001; // Get the j-th bit of the i-th byte

            if (bit != 0) {
                instring[index] = '#';
            } else {
                instring[index] = '.';
            }
            index++;
        }
    }

    instring[index] = '\0'; // Add null terminator at the end

    return 0;
}