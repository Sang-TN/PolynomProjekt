#pragma once
#include <iostream>
#include <vector>


<<<<<<< HEAD
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
=======
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
>>>>>>> 41ead3dc4fba5b47f53c4d27c19a9ace2ea2dce2

    return Polynom(d);
}