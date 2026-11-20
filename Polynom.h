#pragma once
#include <iostream>
#include <vector>

// Eingabe liefert die Koeffizienten aufsteigend: {a0, a1, a2, ...} = a0 + a1*x + a2*x^2 + ...
// Ableitung: f'(x) = a1 + 2*a2*x + 3*a3*x^2 + ...  ->  {a1, 2*a2, 3*a3, ...}
Polynom Polynom::ableitung() const {
    // Konstante (oder leer, z. B. bei Eingabefehler): Ableitung ist 0
    if (m_koeffizienten.size() <= 1)
        return Polynom(std::vector<double>{0.0});

// Gruppe Polynom
// Koeffizienten in aufsteigender Reihenfolge: {a0, a1, a2, ...} = a0 + a1*x + a2*x^2 + ...
class Polynom {
public:
    Polynom(std::vector<double> koeffizienten);

    double  wert(double x) const;   // Y(x)
    Polynom ableitung() const;
    int     grad() const;
    const std::vector<double>& koeffizienten() const;

private:
    std::vector<double> m_koeffizienten;
};

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

    // Fuehrende Nullen entfernen, damit grad() stimmt
    while (d.size() > 1 && d.back() == 0.0)
        d.pop_back();

    return Polynom(d);
}