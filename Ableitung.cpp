#include "Ableitung.h"

#include <cmath>
#include <iostream>
#include <sstream>

Polynom ersteAbleitung(const Polynom& p) {
    return p.ableitung();
}

Polynom zweiteAbleitung(const Polynom& p) {
    return p.ableitung().ableitung();
}

// Absteigend wie bei der Eingabe: 2 x^3 - 3 x^2 + 4 x + 1
std::string alsText(const Polynom& p) {
    const std::vector<double>& c = p.koeffizienten();
    std::ostringstream text;
    bool erster = true;

    for (size_t i = c.size(); i-- > 0;) {
        if (c[i] == 0.0) continue;
        if (erster) {
            if (c[i] < 0) text << "-";
        } else {
            text << (c[i] < 0 ? " - " : " + ");
        }
        text << std::abs(c[i]);

        if (i == 1)      text << " x";
        else if (i > 1)  text << " x^" << i;

        erster = false;
    }

    if (erster) text << 0;
    return text.str();
}

void ausgabe(const Polynom& p) {
    std::cout << alsText(p) << std::endl;
}
