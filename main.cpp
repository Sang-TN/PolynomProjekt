#include "Eingabe.h"
#include "Darstellung.h"
#include "Ableitung.h"
#include <iostream>

int main() {
    PolynomFenster fenster;
    fenster.starten();
    if (!fenster.bestaetigt()) return 0;
    Polynom p = fenster.alsPolynom();
    zeichne(p, -10, 10);
    return 0;
}