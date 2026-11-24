#pragma once
#include <iostream>
#include <vector>

using namespace std;

// Gruppe Polynom
// Koeffizienten in aufsteigender Reihenfolge: {a0, a1, a2, ...} = a0 + a1*x + a2*x^2 + ...
class Polynom {
public:
    Polynom(std::vector<double> koeffizienten) : m_koeffizienten(std::move(koeffizienten)) {}

    // Y(x) per Horner-Schema
    double wert(double x) const {
        double y = 0.0;
        for (size_t i = m_koeffizienten.size(); i-- > 0;)
            y = y * x + m_koeffizienten[i];
        return y;
    }

    Polynom ableitung() const;
    int     grad() const { return m_koeffizienten.empty() ? 0 : static_cast<int>(m_koeffizienten.size()) - 1; }
    const std::vector<double>& koeffizienten() const { return m_koeffizienten; }

private:
    std::vector<double> m_koeffizienten;
};

// Eingabe liefert die Koeffizienten aufsteigend: {a0, a1, a2, ...} = a0 + a1*x + a2*x^2 + ...
// Ableitung: f'(x) = a1 + 2*a2*x + 3*a3*x^2 + ...  ->  {a1, 2*a2, 3*a3, ...}
inline Polynom Polynom::ableitung() const {
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

class PolynomAbleitung
{
public:
    static vector<double> berechneAbleitungGrad4(const vector<double> &koeffizienten)
    {
        vector<double> ableitung(4);

        ableitung[0] = koeffizienten[1] * 1;
        ableitung[1] = koeffizienten[2] * 2;
        ableitung[2] = koeffizienten[3] * 3;
        ableitung[3] = koeffizienten[4] * 4;

        return ableitung;
    }
};
