#include "computer.h"

// Computer
void Computer::setInput(const bool reset) {
    this->reset = reset;
    this->cpu.setInput(this->dataMemory.get(), this->instructionMemory.get(), reset);
}

void Computer::finishClock() {
    this->instructionMemory.setInput(this->cpu.getPc());
    this->dataMemory.setInput(this->cpu.getOutM(), this->cpu.getWriteM(), this->cpu.getAdderssM());
    this->cpu.setInput(this->dataMemory.get(), this->instructionMemory.get(), this->reset);
    this->dataMemory.setInput(this->cpu.getOutM(), this->cpu.getWriteM(), this->cpu.getAdderssM());

    this->cpu.finishClock();
    this->instructionMemory.finishClock();
    this->dataMemory.finishClock();
}

void Computer::writeProgram(const std::string fileName) {
    int length;
    unsigned char *buffer;
    std::ifstream is(fileName, std::ifstream::binary);
    if (is) {
        is.seekg(0, is.end);
        length = (int)is.tellg();
        is.seekg(0, is.beg);

        buffer = new unsigned char[length];
        is.read((char *)buffer, length);
        is.close();
    }
    bool program[16];
    bool address[15] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    int lines = length / 2;
    for (int k = 0; k < lines; k++) {
        unsigned char current = buffer[k * 2];
        for (int i = 0; i < 8; i++) {
            program[7 - i] = (bool)(current % 2);
            current /= 2;
        }
        current = buffer[k * 2 + 1];

        for (int i = 0; i < 8; i++) {
            program[15 - i] = (bool)(current % 2);
            current /= 2;
        }

        for (int i = 0; i < 15; i++) {
            if (address[i] == 0) {
                address[i] = 1;
                break;
            } else {
                address[i] = 0;
            }
        }
        this->instructionMemory.writeRom(program, address);
        this->instructionMemory.finishClock();
    }
}

// Monitor
void Monitor::gotoXY(const int x, const int y) {
    COORD pos = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void Monitor::printPixel(const int pos, const bool pixel) {
    gotoXY((pos % 512), pos / 512);
    if (pixel) {
        std::cout << "*";
    } else {
        std::cout << " ";
    }
}

void Monitor::smallFont() {
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 5;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    std::wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void Monitor::cursorOff() {
    HANDLE hConsole;
    CONSOLE_CURSOR_INFO consoleCursor;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    consoleCursor.bVisible = false;
    consoleCursor.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &consoleCursor);
}

Monitor::Monitor() {
    currentScreen = new bool[512 * 256];
    smallFont();
    cursorOff();
    system("mode con cols=512 lines=256 | title Hack");
}

void Monitor::printScreen(bool ******newScreen) {
    int i = 0;
    for (int a = 0; a < 2; a++) {
        for (int b = 0; b < 8; b++) {
            for (int c = 0; c < 8; c++) {
                for (int d = 0; d < 8; d++) {
                    for (int e = 0; e < 8; e++) {
                        for (int f = 0; f < 16; f++) {
                            if (newScreen[a][b][c][d][e][f] != this->currentScreen[i]) {
                                printPixel(i, newScreen[a][b][c][d][e][f]);
                                this->currentScreen[i] = newScreen[a][b][c][d][e][f];
                            }
                            i++;
                        }
                    }
                }
            }
        }
    }
}
