#ifndef FLIPFLOP_H
#define FLIPFLOP_H

#include "adder.h"
#include "gate.h"

// Data-Flip-Flop
class DFF {
   private:
    bool current = 0;
    bool future = 0;

   public:
    void finishClock() {
        this->current = this->future;
    }

    void setInput(const bool in) {
        this->future = in;
    }

    bool get() {
        return this->current;
    }
};

// Bit
class Bit {
   private:
    DFF* dff;

   public:
    Bit() {
        this->dff = new DFF();
    }

    void finishClock() {
        this->dff->finishClock();
    }

    void setInput(const bool in, const bool load);

    bool get() {
        return this->dff->get();
    }

    ~Bit() {
        delete this->dff;
    }
};

// Register
class Register {
   private:
    Bit* bits;
    bool current[16] = {0};

   public:
    Register() {
        this->bits = new Bit[16];
    }
    void setInput(const bool in[16], const bool load);

    void finishClock();

    bool* get() {
        return this->current;
    }

    ~Register() {
        delete[] this->bits;
    }
};

// Memory Bank
// 8
class RAM8 {
   private:
    Register* reg;
    bool add[3] = {0};
    bool current[16] = {0};

   public:
    RAM8() {
        this->reg = new Register[8];
    }

    void setInput(const bool in[16], const bool address[3], const bool load);

    void finishClock();

    bool* get() {
        return this->current;
    }


    ~RAM8() {
        delete[] this->reg;
    }
};

// 64
class RAM64 {
   private:
    RAM8* ram;
    bool add[3] = {0};
    bool current[16] = {0};

   public:
    RAM64() {
        this->ram = new RAM8[8];
    }

    void setInput(const bool in[16], const bool address[6], const bool load);

    void finishClock();

    bool* get() {
        return this->current;
    }

    ~RAM64() {
        delete[] this->ram;
    }
};

// 512
class RAM512 {
   private:
    RAM64* ram;
    bool add[3] = {0};
    bool current[16] = {0};

   public:
    RAM512() {
        this->ram = new RAM64[8];
    }

    void finishClock();

    void setInput(const bool in[16], const bool address[9], const bool load);
    bool* get() {
        return this->current;
    }

    ~RAM512() {
        delete[] this->ram;
    }
};

// 4k
class RAM4k {
   private:
    bool *ram;
    int address = 0;
    bool in[16] = {0};
    bool load;
    bool current[16] = {0};

   public:
    RAM4k() {
        this->ram  = new bool[65536];
    }

    void setInput(const bool in[16], const bool address[12], const bool load);

    void finishClock();

    bool* get() {
        return this->current;
    }

    ~RAM4k() {
        delete[] this->ram;
    }
};

// 16k
class RAM16k {
   private:
    RAM4k* ram;
    bool add[2] = {0};
    bool current[16] = {0};

   public:
    RAM16k() {
        this->ram = new RAM4k[4];
    }

    void setInput(const bool in[16], const bool address[14], const bool load);

    bool* get() {
        return this->current;
    }

    void finishClock();

    ~RAM16k() {
        delete[] this->ram;
    }
};

// Counter Chip
class PC {
   private:
    Register reg;
    bool* current;

   public:
    PC() {
        bool temp[16];
        current = temp;
    }

    void setInput(const bool in[16], const bool inc, const bool load, const bool reset);

    void finishClock();

    bool* get() {
        return this->current;
    }
};

#endif