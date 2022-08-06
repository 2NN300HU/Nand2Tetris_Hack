#include <conio.h>

#include "computer.h"

int main() {
    Computer computer;
    Monitor monitor;
    std::cout << "writing ROM...";
    computer.writeProgram("some.dat");
    system("cls");
    computer.setInput(1);
    computer.finishClock();
    computer.setInput(0);
    bool key[16] = {0};
    ScreenDiff* newScreen;
    int keyValue, current;
    while (true){
        if (_kbhit()) {
            current = _getch();
            keyValue = current;
            if (current == 0 || current == 224) {
                current = _getch();
                switch (current) {
                    case 75:
                        keyValue = 130;
                        break;
                    case 72:
                        keyValue = 131;
                        break;
                    case 77:
                        keyValue = 132;
                        break;
                    case 80:
                        keyValue = 133;
                        break;
                    case 71:
                        keyValue = 134;
                        break;
                    case 79:
                        keyValue = 135;
                        break;
                    case 73:
                        keyValue = 136;
                        break;
                    case 81:
                        keyValue = 137;
                        break;
                    case 82:
                        keyValue = 138;
                        break;
                    case 83:
                        keyValue = 139;
                        break;
                    default:
                        keyValue = 0;
                        break;
                }
                if (current >= 59 && current <= 68) {
                    keyValue = current + 82;
                }
                if(current>=133){
                    keyValue = current+18;
                }
            }
        }
        for(int i = 0;i<16;i++){
            key[i] = keyValue%2;
            keyValue/=2;
        }
        keyValue = 0;

        computer.keyinput(key);
        computer.finishClock();
        newScreen = computer.getScreen();
        monitor.printScreen(newScreen);
    }
    return 0;
}