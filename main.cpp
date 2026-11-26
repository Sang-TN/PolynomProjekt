#include "Eingabe.h"
#include "Darstellung.h"

int main() {
    PolynomFenster fenster;
    fenster.starten();
    if (!fenster.bestaetigt()) return 0;
    zeichne(fenster.alsPolynom(), -10, 10);
    return 0;
}