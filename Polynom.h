#pragma once
#include <vector>

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
