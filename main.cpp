#include "Eingabe.h"
#include "Darstellung.h"

int main() {
    Polynom p = lesePolynomEin();
    zeichne(p, -10, 10);

    return 0;
}
