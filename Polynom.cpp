#include "Polynom.h"

#include <utility>

// Koeffizienten aufsteigend: {a0, a1, a2, ...} = a0 + a1*x + a2*x^2 + ...

Polynom::Polynom(std::vector<double> koeffizienten)
    : m_koeffizienten(std::move(koeffizienten))
{
    // Leere Eingabe (z. B. bei Eingabefehler) -> Nullpolynom
    if (m_koeffizienten.empty())
        m_koeffizienten.push_back(0.0);

    // Fuehrende Nullen entfernen, damit grad() stimmt: {1, 2, 0, 0} -> {1, 2}
    while (m_koeffizienten.size() > 1 && m_koeffizienten.back() == 0.0)
        m_koeffizienten.pop_back();
}

// Horner-Schema: 2x^3 - 3x^2 + 4x + 1 = ((2x - 3)x + 4)x + 1
double Polynom::wert(double x) const {
    double ergebnis = 0.0;
    for (int i = grad(); i >= 0; --i)
        ergebnis = ergebnis * x + m_koeffizienten[i];
    return ergebnis;
}

// f'(x) = a1 + 2*a2*x + 3*a3*x^2 + ...  ->  {a1, 2*a2, 3*a3, ...}
Polynom Polynom::ableitung() const {
    std::vector<double> d;

    // Konstante: Ableitung ist 0
    if (m_koeffizienten.size() <= 1) {
        d.push_back(0);
        return Polynom(d);
    }

    // a0 faellt weg, jeder andere Koeffizient mal seinen Exponenten
    for (size_t i = 1; i < m_koeffizienten.size(); i++) {
        d.push_back(m_koeffizienten[i] * i);
    }

    return Polynom(d);
}

int Polynom::grad() const {
    return static_cast<int>(m_koeffizienten.size()) - 1;
}

const std::vector<double>& Polynom::koeffizienten() const {
    return m_koeffizienten;
}
