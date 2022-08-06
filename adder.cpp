#include "adder.h"

// Half-adder
void HalfAdder(const bool a, const bool b, bool& sum, bool& carry) {
    And(a, b, carry);
    Xor(a, b, sum);
}

// Full-adder
void FullAdder(const bool a, const bool b, const bool c, bool& sum, bool& carry) {
    bool sum1, carry1, carry2;
    HalfAdder(a, b, sum1, carry1);
    HalfAdder(sum1, c, sum, carry2);
    Or(carry1, carry2, carry);
}

// Adder
void Add16(const bool a[16], const bool b[16], bool out[16]) {
    bool carry;
    HalfAdder(a[0], b[0], out[0], carry);
    for (int i = 1; i < 16; i++) {
        FullAdder(a[i], b[i], carry, out[i], carry);
    }
}

// Incrementer
void Inc16(const bool in[16], bool out[16]) {
    bool carry = in[0];
    Not(in[0], out[0]);
    for (int i = 1; i < 16; i++) {
        HalfAdder(in[i], carry, out[i], carry);
    }
}

// ALU
void ALU(const bool x[16], const bool y[16], const bool zx, const bool nx, const bool zy, const bool ny, const bool f, const bool no, bool out[16], bool& zr, bool& ng) {
    bool notZx, notZxs[16], x1[16], notX1[16], x2[16], notZy, notZys[16], y1[16], notY1[16], y2[16], addRes[16], andRes[16], out1[16], notOut1[16], notZr, res;
    Not(zx, notZx);
    for (bool& i : notZxs) {
        i = notZx;
    }
    And16(x, notZxs, x1);
    Not16(x1, notX1);
    Mux16(x1, notX1, nx, x2);

    Not(zy, notZy);
    for (bool& i : notZys) {
        i = notZy;
    }
    And16(y, notZys, y1);
    Not16(y1, notY1);
    Mux16(y1, notY1, ny, y2);

    And16(x2, y2, andRes);
    Add16(x2, y2, addRes);
    Mux16(andRes, addRes, f, out1);

    Not16(out1, notOut1);
    Mux16(out1, notOut1, no, out);

    res = out[0];
    for (int i = 1; i < 16; i++) {
        Or(res, out[i], res);
    }
    notZr = res;
    Not(notZr, zr);
    ng = out[15];
}