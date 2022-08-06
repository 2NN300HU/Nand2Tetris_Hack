#ifndef ADDER_H
#define ADDER_H

#include "gate.h"

void HalfAdder(const bool a, const bool b, bool& sum, bool& carry);
void FullAdder(const bool a, const bool b, const bool c, bool& sum, bool& carry);
void Add16(const bool a[16], const bool b[16], bool out[16]);
void Inc16(const bool in[16], bool out[16]);
void ALU(const bool x[16], const bool y[16], const bool zx, const bool nx, const bool zy, const bool ny, const bool f, const bool no, bool out[16], bool& zr, bool& ng);

#endif