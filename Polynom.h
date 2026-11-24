#include "Polynom.h"


Polynom Polynom::ableitung() const {
    vector<double> d;

    // Konstante: Ableitung ist 0
    if (m_koeffizienten.size() <= 1) {
        d.push_back(0);
        return Polynom(d);
    }

    // a0 faellt weg, jeder andere Koeffizient mal seinen Exponenten
    for (int i = 1; i < m_koeffizienten.size(); i++) {
        d.push_back(m_koeffizienten[i] * i);
    }

    return Polynom(d);
}