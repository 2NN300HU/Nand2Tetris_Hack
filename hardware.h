#ifndef HARAWARE_H
#define HARAWARE_H

#include "adder.h"
#include "flipflop.h"
#include "gate.h"

// CPU
class CPU {
   private:
    PC pc;
    Register A;
    Register D;
    bool writeM;
    bool addressM[15];
    bool outM[16];
    bool outPc[15];

   public:
    void setInput(const bool inM[16], const bool instruction[16], const bool reset);
    void finishClock();

    bool* getPc() {
        return this->outPc;
    }
    bool getWriteM() {
        return this->writeM;
    }

    bool* getAdderssM() {
        return this->addressM;
    }

    bool* getOutM() {
        return this->outM;
    }
};

// ROM
class ROM32k {
   private:
    RAM16k* ram;
    bool add;
    bool current[16];

   public:
    ROM32k() {
        this->ram = new RAM16k[2];
    }

    void setInput(const bool address[15]);
    void writeRom(const bool in[16], const bool address[15]);
    void finishClock();

    bool* get() {
        return this->current;
    }
    ~ROM32k() {
        delete[] this->ram;
    }
};

// For sending screen's diffrence
class ScreenDiff{
    public:
        bool in[16];
        bool load;
        bool address[13];
    void set(const bool in[16], const bool load, const bool address[13]);
};

// Screen
class Screen {
   private:
    RAM4k* ram;
    bool add;
    bool current[16];
    ScreenDiff *screenDiff;
    ScreenDiff *future;

   public:
    Screen() {
        this->ram = new RAM4k[2];
        this->screenDiff = new ScreenDiff;
        this->future = new ScreenDiff;
    }

    void setInput(const bool in[16], const bool load, const bool address[13]);
    void finishClock();

    ScreenDiff* getScreen(){
        return screenDiff;
    }

    bool* get() {
        return this->current;
    }

    ~Screen() {
        delete[] this->ram;
        delete screenDiff;
        delete future;
    }
};

// Keyboard
class Keyboard {
   private:
    bool current[16] = {0};

   public:
    void keyInput(const bool in[16]);

    bool* get() {
        return this->current;
    }
};

// Memory
class Memory {
   private:
    RAM16k ram;
    Screen screen;
    Keyboard keyboard;
    bool current[16] = {0};
    bool add[2];

   public:
    void setInput(const bool in[16], const bool load, const bool address[15]);
    void finishClock();

    ScreenDiff* getScreen(){
        return this->screen.getScreen();
    }

    void keyInput(const bool in[16]) {
        this->keyboard.keyInput(in);
    }

    bool* get() {
        return this->current;
    }
};

#endif