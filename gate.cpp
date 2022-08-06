#include "gate.h"

// Nand Gate
void Nand(const bool a, const bool b, bool &out) {
    if (a && b) {
        out = false;
        return;
    }
    out = true;
}

// Not gate
void Not(const bool in, bool &out) {
    Nand(in, in, out);
}

// And gate
void And(const bool a, const bool b, bool &out) {
    bool nand;
    Nand(a, b, nand);
    Nand(nand, nand, out);
}

// Or gate
void Or(const bool a, const bool b, bool &out) {
    bool nandA, nandB;
    Nand(a, a, nandA);
    Nand(b, b, nandB);
    Nand(nandA, nandB, out);
}

// Xor gate
void Xor(const bool a, const bool b, bool &out) {
    bool notA, notB, andOut1, andOut2;
    Not(a, notA);
    Not(b, notB);
    And(a, notB, andOut1);
    And(b, notA, andOut2);
    Or(andOut1, andOut2, out);
}

// Multiplexor
void Mux(const bool a, const bool b, const bool sel, bool &out) {
    bool notSel, out1, out2;
    Not(sel, notSel);
    And(notSel, a, out1);
    And(sel, b, out2);
    Or(out1, out2, out);
}

// Demultiplexor
void DMux(const bool in, const bool sel, bool &a, bool &b) {
    bool notSel, out1, out2;
    Not(sel, notSel);
    And(notSel, in, a);
    And(sel, in, b);
}

// Multibit Not
void Not16(const bool in[16], bool out[16]) {
    for (int i = 0; i < 16; i++) {
        Not(in[i], out[i]);
    }
}

// Multibit And
void And16(const bool a[16], const bool b[16], bool out[16]) {
    for (int i = 0; i < 16; i++) {
        And(a[i], b[i], out[i]);
    }
}

// Multibit Or
void Or16(const bool a[16], const bool b[16], bool out[16]) {
    for (int i = 0; i < 16; i++) {
        Or(a[i], b[i], out[i]);
    }
}

// Multibit Multiplexor
void Mux16(const bool a[16], const bool b[16], const bool sel, bool out[16]) {
    bool notSel, out1, out2;
    Not(sel, notSel);
    for (int i = 0; i < 16; i++) {
        And(notSel, a[i], out1);
        And(sel, b[i], out2);
        Or(out1, out2, out[i]);
    }
}
// Multi-way Or
void Or8Way(const bool a[8], bool &out) {
    bool res;
    res = a[0];
    for (int i = 1; i < 8; i++) {
        Or(res, a[i], res);
    }
    out = res;
}

// Multi-way / Multibit Multipelxor (4-16)
void Mux4Way16(const bool a[16], const bool b[16], const bool c[16], const bool d[16], const bool sel[2], bool out[16]) {
    bool notSel[2], res, out1, out2, out3, out4;
    for (int i = 0; i < 2; i++) {
        Not(sel[i], notSel[i]);
    }
    for (int i = 0; i < 16; i++) {
        And(a[i], notSel[0], res);
        And(notSel[1], res, out1);
        And(b[i], sel[0], res);
        And(notSel[1], res, out2);
        And(c[i], notSel[0], res);
        And(sel[1], res, out3);
        And(d[i], sel[0], res);
        And(sel[1], res, out4);

        Or(out1, out2, res);
        Or(out3, res, res);
        Or(out4, res, out[i]);
    }
}
// Multi-way / Multibit Multipelxor (8-16)
void Mux8Way16(const bool a[16], const bool b[16], const bool c[16], const bool d[16], const bool e[16], const bool f[16], const bool g[16], const bool h[16], const bool sel[3], bool out[16]) {
    bool notSel[3], res, out1, out2, out3, out4, out5, out6, out7, out8;
    for (int i = 0; i < 3; i++) {
        Not(sel[i], notSel[i]);
    }
    for (int i = 0; i < 16; i++) {
        And(a[i], notSel[0], res);
        And(notSel[1], res, res);
        And(notSel[2], res, out1);

        And(b[i], sel[0], res);
        And(notSel[1], res, res);
        And(notSel[2], res, out2);

        And(c[i], notSel[0], res);
        And(sel[1], res, res);
        And(notSel[2], res, out3);

        And(d[i], sel[0], res);
        And(sel[1], res, res);
        And(notSel[2], res, out4);

        And(e[i], notSel[0], res);
        And(notSel[1], res, res);
        And(sel[2], res, out5);

        And(f[i], sel[0], res);
        And(notSel[1], res, res);
        And(sel[2], res, out6);

        And(g[i], notSel[0], res);
        And(sel[1], res, res);
        And(sel[2], res, out7);

        And(h[i], sel[0], res);
        And(sel[1], res, res);
        And(sel[2], res, out8);

        Or(out1, out2, res);
        Or(out3, res, res);
        Or(out4, res, res);
        Or(out5, res, res);
        Or(out6, res, res);
        Or(out7, res, res);
        Or(out8, res, out[i]);
    }
}
// Multi-way / Multibit DeMultipelxor (4-1)
void DMux4Way(const bool in, const bool sel[2], bool &a, bool &b, bool &c, bool &d) {
    bool notSel[2], res;
    for (int i = 0; i < 2; i++) {
        Not(sel[i], notSel[i]);
    }

    And(in, notSel[0], res);
    And(notSel[1], res, a);
    And(in, sel[0], res);
    And(notSel[1], res, b);
    And(in, notSel[0], res);
    And(sel[1], res, c);
    And(in, sel[0], res);
    And(sel[1], res, d);
}

// Multi-way / Multibit DeMultipelxor (8-1)
void DMux8Way(const bool in, const bool sel[3], bool &a, bool &b, bool &c, bool &d, bool &e, bool &f, bool &g, bool &h) {
    bool notSel[3], res;
    for (int i = 0; i < 3; i++) {
        Not(sel[i], notSel[i]);
    }

    And(in, notSel[0], res);
    And(notSel[1], res, res);
    And(notSel[2], res, a);

    And(in, sel[0], res);
    And(notSel[1], res, res);
    And(notSel[2], res, b);

    And(in, notSel[0], res);
    And(sel[1], res, res);
    And(notSel[2], res, c);

    And(in, sel[0], res);
    And(sel[1], res, res);
    And(notSel[2], res, d);

    And(in, notSel[0], res);
    And(notSel[1], res, res);
    And(sel[2], res, e);

    And(in, sel[0], res);
    And(notSel[1], res, res);
    And(sel[2], res, f);

    And(in, notSel[0], res);
    And(sel[1], res, res);
    And(sel[2], res, g);

    And(in, sel[0], res);
    And(sel[1], res, res);
    And(sel[2], res, h);
}