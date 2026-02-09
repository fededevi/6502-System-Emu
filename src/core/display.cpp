#include "display.h"
#include <iostream>
#include <sstream>
#include <cstring>

Display::Display(Memory* memory) : mem_(memory), enabled_(false) {
}

void Display::render() {
    if (!enabled_) return;
    
    // Clear screen and move cursor to top-left
    std::cout << "\033[2J\033[H";
    
    // Draw top border
    std::cout << "┌";
    for (int i = 0; i < DISPLAY_WIDTH; i++) {
        std::cout << "─";
    }
    std::cout << "┐\n";
    
    // Render each row
    for (int y = 0; y < DISPLAY_HEIGHT; y++) {
        std::cout << "│";
        for (int x = 0; x < DISPLAY_WIDTH; x++) {
            Word addr = DISPLAY_START_ADDR + (y * DISPLAY_WIDTH) + x;
            Byte value = mem_->read(addr);
            
            // Output colored character
            std::cout << getColor(value);
            const char* ch = getChar(value);
            std::cout << ch << "\033[0m";
        }
        std::cout << "│\n";
    }
    
    // Draw bottom border
    std::cout << "└";
    for (int i = 0; i < DISPLAY_WIDTH; i++) {
        std::cout << "─";
    }
    std::cout << "┘\n";
    
    std::cout << std::flush;
}

void Display::clear() {
    for (Word addr = DISPLAY_START_ADDR; addr < DISPLAY_START_ADDR + DISPLAY_SIZE; addr++) {
        mem_->write(addr, 0);
    }
}

void Display::setEnabled(bool enabled) {
    enabled_ = enabled;
}

const char* Display::getChar(Byte value) {
    // Map values to different characters
    if (value == 0) return " ";  // Empty space
    if (value == 1) return "█";  // Solid block (snake body/walls)
    if (value == 2) return "●";  // Circle (apple/food)
    if (value == 3) return "▓";  // Medium shade
    if (value == 4) return "▒";  // Light shade
    if (value == 5) return "░";  // Lighter shade
    if (value == 6) return "♦";  // Diamond
    if (value == 7) return "♥";  // Heart
    if (value == 8) return "♣";  // Club
    if (value == 9) return "♠";  // Spade
    if (value == 10) return "•"; // Bullet
    if (value == 11) return "○"; // Empty circle
    if (value == 12) return "◘"; // Inverse bullet
    if (value == 13) return "◙"; // Inverse circle
    if (value == 14) return "☺"; // Smiley
    if (value == 15) return "☻"; // Inverse smiley
    
    // For values 16-31, use block characters with different intensities
    if (value >= 16 && value < 32) return "▓";
    
    // For higher values, use ASCII characters
    if (value >= 32 && value < 127) {
        static char ascii_char[2];
        ascii_char[0] = (char)value;
        ascii_char[1] = '\0';
        return ascii_char;
    }
    
    // Default to solid block for other values
    return "█";
}

std::string Display::getColor(Byte value) {
    // ANSI color codes based on value
    // Colors: 0=black, 1=red, 2=green, 3=yellow, 4=blue, 5=magenta, 6=cyan, 7=white
    
    int colorCode = (value & 0x0F) % 8;  // Use lower 4 bits for color
    bool bright = (value & 0xF0) != 0;   // Use upper 4 bits for brightness
    
    std::ostringstream oss;
    oss << "\033[";
    if (bright && colorCode > 0) {
        oss << "1;";  // Bold/bright
    }
    oss << (30 + colorCode) << "m";  // Foreground color
    
    return oss.str();
}
