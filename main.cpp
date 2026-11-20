#include "Polynom.h"
#include "Nullstellenberechnung.h"
#include "Eingabe.h"
#include "Darstellung.h"

int main() {
    Polynom p = lesePolynomEin();   // Eingabe-Gruppe: Benutzer gibt die Funktion ein
    zeichne(p, -10, 10);            // Darstellung: Graph anzeigen
    return 0;
}