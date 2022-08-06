#include "hardware.h"

// CPU
void CPU::setInput(const bool inM[16], const bool instruction[16], const bool reset) {
    bool instructionFirstBit, notInstructionFirstBit, DC = 1, Y[16], ALUOut[16], zr, ng, reverseIns[16], inA[16], loadA, loadD, inc;

    instructionFirstBit = instruction[0];
    Not(instructionFirstBit, notInstructionFirstBit);

    Mux16(this->A.get(), inM, instruction[3], Y);
    ALU(this->D.get(), Y, instruction[4], instruction[5], instruction[6], instruction[7], instruction[8], instruction[9], ALUOut, zr, ng);

    for (int i = 0; i < 16; i++) {
        reverseIns[i] = instruction[15 - i];
    }
    Mux16(reverseIns, ALUOut, instructionFirstBit, inA);

    Or(notInstructionFirstBit, instruction[10], loadA);
    this->A.setInput(inA, loadA);

    And(instructionFirstBit, instruction[11], loadD);
    this->D.setInput(ALUOut, loadD);

    And(instructionFirstBit, instruction[12], this->writeM);

    for (int i = 0; i < 16; i++) {
        this->outM[i] = ALUOut[i];
    }

    bool gt, notGt,andZr, andNg, andGt, orRes, isC, pcLoad;

    Or(zr, ng, notGt);
    Not(notGt,gt);
    And(instruction[13], ng, andNg);
    And(instruction[14], zr, andZr);
    And(instruction[15], gt, andGt);

    Or(andZr, andNg, orRes);
    Or(orRes, andGt, isC);
    And(isC, instructionFirstBit, pcLoad);

    inc = DC;

    pc.setInput(this->A.get(), inc, pcLoad, reset);
}

void CPU::finishClock() {
    this->pc.finishClock();
    this->A.finishClock();
    this->D.finishClock();

    for (int i = 0; i < 15; i++) {
        this->addressM[i] = this->A.get()[i];
    }

    for (int i = 0; i < 15; i++) {
        this->outPc[i] = this->pc.get()[i];
    }
}

// ROM
void ROM32k::setInput(const bool address[15]) {
    bool GND = 0;
    auto& cur = this->ram;
    bool addLeft[14];
    for (int i = 0; i < 14; i++) {
        addLeft[i] = address[i];
    }
    add = address[14];
    bool in[16] = {0};
    for (int i = 0; i < 2; i++) {
        cur[i].setInput(in, addLeft, GND);
    }
    Mux16(cur[0].get(), cur[1].get(), this->add, this->current);
}

void ROM32k::writeRom(const bool in[16], const bool address[15]) {
    bool DC = 1;
    auto& cur = this->ram;
    bool addLeft[14];
    for (int i = 0; i < 14; i++) {
        addLeft[i] = address[i];
    }
    add = address[14];
    for (int i = 0; i < 2; i++) {
        cur[i].setInput(in, addLeft, DC);
    }
}

void ROM32k::finishClock() {
    for (int i = 0; i < 2; i++) {
        this->ram[i].finishClock();
    }
    auto& cur = this->ram;
    Mux16(cur[0].get(), cur[1].get(), this->add, this->current);
}

void ScreenDiff::set(const bool in[16], const bool load, const bool address[13]) {
    for (int i = 0; i < 16; i++) {
        this->in[i] = in[i];
    }
    this->load = load;
    for (int i = 0; i < 13; i++) {
        this->address[i] = address[i];
    }
}
// Screen
void Screen::setInput(const bool in[16], const bool load, const bool address[13]) {
    this->future->set(in, load, address);
    auto& cur = this->ram;
    bool addLeft[12], bus[2];
    for (int i = 0; i < 12; i++) {
        addLeft[i] = address[i];
    }
    add = address[12];
    DMux(load, add, bus[0], bus[1]);
    for (int i = 0; i < 2; i++) {
        cur[i].setInput(in, addLeft, bus[i]);
    }
    Mux16(cur[0].get(), cur[1].get(), this->add, this->current);
}

void Screen::finishClock() {
    for (int i = 0; i < 2; i++) {
        this->ram[i].finishClock();
    }
    auto& cur = this->ram;
    Mux16(cur[0].get(), cur[1].get(), this->add, this->current);

    ScreenDiff* temp = this->screenDiff;
    this->screenDiff = this->future;
    this->future = temp;
}

// Keyboard
void Keyboard::keyInput(const bool in[16]) {
    for (int i = 0; i < 16; i++) {
        current[i] = in[i];
    }
}

// Memory
void Memory::setInput(const bool in[16], const bool load, const bool address[15]) {
    bool loadRam, loadRam1, loadRam2, loadKey, loadScreen, notAddress13, left13[13], left14[14];
    this->add[0] = address[13];
    this->add[1] = address[14];

    bool ram1[16], ram2[16];

    DMux4Way(load, add, loadRam1, loadRam2, loadScreen, loadKey);
    Or(loadRam1, loadRam2, loadRam);

    for (int i = 0; i < 14; i++) {
        left14[i] = address[i];
    }

    for (int i = 0; i < 13; i++) {
        left13[i] = address[i];
    }

    this->ram.setInput(in, left14, loadRam);
    this->screen.setInput(in, loadScreen, left13);

    Mux4Way16(this->ram.get(), this->ram.get(), this->screen.get(), this->keyboard.get(), add, this->current);
}

void Memory::finishClock() {
    this->ram.finishClock();
    this->screen.finishClock();
    Mux4Way16(this->ram.get(), this->ram.get(), this->screen.get(), this->keyboard.get(), this->add, this->current);
}