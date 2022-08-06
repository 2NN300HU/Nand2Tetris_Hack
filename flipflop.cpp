#include "flipflop.h"

// Bit
void Bit::setInput(const bool in, const bool load) {
    bool input;
    Mux(this->dff->get(), in, load, input);
    this->dff->setInput(input);
}

// Register
void Register::setInput(const bool in[16], const bool load) {
    for (int i = 0; i < 16; i++) {
        this->bits[i].setInput(in[i], load);
    }
}

void Register::finishClock() {
    for (int i = 0; i < 16; i++) {
        this->bits[i].finishClock();
        this->current[i] = this->bits[i].get();
    }
}

// Memory Bank
// 8
void RAM8::setInput(const bool in[16], const bool address[3], const bool load) {
    auto& cur = this->reg;
    bool bus[8];
    for (int i = 0; i < 3; i++) {
        this->add[i] = address[i];
    }
    DMux8Way(load, address, bus[0], bus[1], bus[2], bus[3], bus[4], bus[5], bus[6], bus[7]);
    for (int i = 0; i < 8; i++) {
        cur[i].setInput(in, bus[i]);
    }
    Mux8Way16(cur[0].get(), cur[1].get(), cur[2].get(), cur[3].get(), cur[4].get(), cur[5].get(), cur[6].get(), cur[7].get(), this->add, this->current);
}

void RAM8::finishClock() {
    for (int i = 0; i < 8; i++) {
        this->reg[i].finishClock();
    }
    auto& cur = this->reg;
    Mux8Way16(cur[0].get(), cur[1].get(), cur[2].get(), cur[3].get(), cur[4].get(), cur[5].get(), cur[6].get(), cur[7].get(), this->add, this->current);
}

// 64
void RAM64::setInput(const bool in[16], const bool address[6], const bool load) {
    auto& cur = this->ram;
    bool bus[8], addLeft[3];
    for (int i = 0; i < 3; i++) {
        addLeft[i] = address[i];
    }
    for (int i = 0; i < 3; i++) {
        this->add[i] = address[i + 3];
    }

    DMux8Way(load, add, bus[0], bus[1], bus[2], bus[3], bus[4], bus[5], bus[6], bus[7]);
    for (int i = 0; i < 8; i++) {
        cur[i].setInput(in, addLeft, bus[i]);
    }
    Mux8Way16(cur[0].get(), cur[1].get(), cur[2].get(), cur[3].get(), cur[4].get(), cur[5].get(), cur[6].get(), cur[7].get(), add, this->current);
}

void RAM64::finishClock() {
    for (int i = 0; i < 8; i++) {
        this->ram[i].finishClock();
    }
    auto& cur = this->ram;
    Mux8Way16(cur[0].get(), cur[1].get(), cur[2].get(), cur[3].get(), cur[4].get(), cur[5].get(), cur[6].get(), cur[7].get(), add, this->current);
}

// 512
void RAM512::setInput(const bool in[16], const bool address[9], const bool load) {
    auto& cur = this->ram;
    bool bus[8], addLeft[6];
    for (int i = 0; i < 6; i++) {
        addLeft[i] = address[i];
    }
    for (int i = 0; i < 3; i++) {
        add[i] = address[i + 6];
    }

    DMux8Way(load, add, bus[0], bus[1], bus[2], bus[3], bus[4], bus[5], bus[6], bus[7]);
    for (int i = 0; i < 8; i++) {
        cur[i].setInput(in, addLeft, bus[i]);
    }

    Mux8Way16(cur[0].get(), cur[1].get(), cur[2].get(), cur[3].get(), cur[4].get(), cur[5].get(), cur[6].get(), cur[7].get(), add, this->current);
}

void RAM512::finishClock() {
    for (int i = 0; i < 8; i++) {
        this->ram[i].finishClock();
    }
    auto& cur = this->ram;
    Mux8Way16(cur[0].get(), cur[1].get(), cur[2].get(), cur[3].get(), cur[4].get(), cur[5].get(), cur[6].get(), cur[7].get(), add, this->current);
}

// 4k
void RAM4k::setInput(const bool in[16], const bool address[12], const bool load) {
    for (int i = 0; i < 16; i++) {
        this->in[i] = in[i];
    }

    this->load = load;

    int add = 0;
    for (int i = 0; i < 12; i++) {
        add = add * 2 ;
        if(address[11 - i]){
            add++;
        }

    }
    add *= 16;
    this->address = add;

    for (int i = 0; i < 16; i++) {
        this->current[i] = this->ram[add + i];
    }
}

void RAM4k::finishClock() {
    if (this->load) {
        for (int i = 0; i < 16; i++) {
            this->ram[this->address + i] = this->in[i];
        }
    }
}

// 16k
void RAM16k::setInput(const bool in[16], const bool address[14], const bool load) {
    auto& cur = this->ram;
    bool bus[4], addLeft[12];
    for (int i = 0; i < 12; i++) {
        addLeft[i] = address[i];
    }
    for (int i = 0; i < 2; i++) {
        add[i] = address[i + 12];
    }

    DMux4Way(load, add, bus[0], bus[1], bus[2], bus[3]);
    for (int i = 0; i < 4; i++) {
        cur[i].setInput(in, addLeft, bus[i]);
    }
    Mux4Way16(cur[0].get(), cur[1].get(), cur[2].get(), cur[3].get(), add, this->current);
}

void RAM16k::finishClock() {
    for (int i = 0; i < 4; i++) {
        this->ram[i].finishClock();
    }
    auto& cur = this->ram;
    Mux4Way16(cur[0].get(), cur[1].get(), cur[2].get(), cur[3].get(), add, this->current);
}

// Counter Chip
void PC::setInput(const bool in[16], const bool inc, const bool load, const bool reset) {
    bool notReset, notResets[16], inInc[16], in1[16], in2[16], loadFin;
    Not(reset, notReset);
    for (int i = 0; i < 16; i++) {
        notResets[i] = notReset;
    }

    Inc16(this->reg.get(), inInc);

    Mux16(inInc, in, load, in1);

    And16(in1, notResets, in2);

    Or(inc, load, loadFin);
    Or(reset, loadFin, loadFin);

    this->reg.setInput(in2, loadFin);
}

void PC::finishClock() {
    this->reg.finishClock();
    this->current = this->reg.get();
}
