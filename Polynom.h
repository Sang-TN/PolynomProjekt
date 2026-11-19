#pragma once
#include <iostream>
#include <vector>

using namespace std;

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

        return ableitung;
    }
};
