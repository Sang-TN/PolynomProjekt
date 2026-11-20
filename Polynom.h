#include <iostream>
#include <vector>

using namespace std;
{
    static vector<double> berechneAbleitung(const vector<double>& koeffizienten) {
        vector<double> ableitung;

        // Konstante (oder leer): Ableitung ist 0
        if (koeffizienten.size() <= 1) {
            ableitung.push_back(0);
            return ableitung;
        }

        // a0 fällt weg, jeder andere Koeffizient mal seinen Exponenten
        for (int i = 1; i < koeffizienten.size(); i++) {
            ableitung.push_back(koeffizienten[i] * i);
        }

        return ableitung;
    }
};

int main() {
    // Test: 5 - 3x + 2x^2 + 4x^3  ->  Ableitung: -3 + 4x + 12x^2
    vector<double> p = {5, -3, 2, 4};
    vector<double> d = PolynomAbleitung::berechneAbleitung(p);

    for (int i = 0; i < d.size(); i++) {
        cout << d[i] << " ";   // Ausgabe: -3 4 12
    }
    cout << endl;

    return 0;
}