#pragma once

#include "types.h"
#include "memory.h"
#include <string>

// Video memory display for 6502 emulator
// Maps memory region $0200-$05FF to a 32x32 character display
// Each byte represents a character/pixel on screen
// Values 0-15 represent different colors/characters

#define DISPLAY_WIDTH 32
#define DISPLAY_HEIGHT 32
#define DISPLAY_START_ADDR 0x0200
#define DISPLAY_SIZE (DISPLAY_WIDTH * DISPLAY_HEIGHT)

class Display {
public:
    Display(Memory* memory);
    
    // Render the display to the terminal
    void render();
    
    // Clear the display (fill with spaces)
    void clear();
    
    // Enable/disable display rendering
    void setEnabled(bool enabled);
    bool isEnabled() const { return enabled_; }
    
private:
    Memory* mem_;
    bool enabled_;
    
    // Get character for a given value (0-255)
    const char* getChar(Byte value);
    
    // Get ANSI color code for a given value
    std::string getColor(Byte value);
};
