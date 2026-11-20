#include "Polynom.h"

// Eingabe liefert die Koeffizienten aufsteigend: {a0, a1, a2, ...} = a0 + a1*x + a2*x^2 + ...
// Ableitung: f'(x) = a1 + 2*a2*x + 3*a3*x^2 + ...  ->  {a1, 2*a2, 3*a3, ...}
Polynom Polynom::ableitung() const {
    // Konstante (oder leer, z. B. bei Eingabefehler): Ableitung ist 0
    if (m_koeffizienten.size() <= 1)
        return Polynom(std::vector<double>{0.0});

    std::vector<double> d;
    d.reserve(m_koeffizienten.size() - 1);

    // a0 faellt weg, jeder andere Koeffizient mal seinen Exponenten
    for (size_t i = 1; i < m_koeffizienten.size(); ++i)
        d.push_back(m_koeffizienten[i] * static_cast<double>(i));

    // Fuehrende Nullen entfernen, damit grad() stimmt
    while (d.size() > 1 && d.back() == 0.0)
        d.pop_back();

    return Polynom(d);
}