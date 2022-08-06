#ifndef GATE_H
#define GATE_H

void Nand(const bool a, const bool b, bool &out);
void Not(const bool in, bool &out);
void And(const bool a, const bool b, bool &out);
void Or(const bool a, const bool b, bool &out);
void Xor(const bool a, const bool b, bool &out);
void Mux(const bool a, const bool b, const bool sel, bool &out);
void DMux(const bool in, const bool sel, bool &a, bool &b);
void Not16(const bool in[16], bool out[16]);
void And16(const bool a[16], const bool b[16], bool out[16]);
void Or16(const bool a[16], const bool b[16], bool out[16]);
void Mux16(const bool a[16], const bool b[16], const bool sel, bool out[16]);
void Or8Way(const bool a[8], bool &out);
void Mux4Way16(const bool a[16], const bool b[16], const bool c[16], const bool d[16], const bool sel[2], bool out[16]);
void Mux8Way16(const bool a[16], const bool b[16], const bool c[16], const bool d[16], const bool e[16], const bool f[16], const bool g[16], const bool h[16], const bool sel[3], bool out[16]);
void DMux4Way(const bool in, const bool sel[2], bool &a, bool &b, bool &c, bool &d);
void DMux8Way(const bool in, const bool sel[3], bool &a, bool &b, bool &c, bool &d, bool &e, bool &f, bool &g, bool &h);

#endif