#include <iostream>
#include <vector>

using namespace std;

class PolynomAbleitung {
public:
    static vector<double> berechneAbleitungGrad4(const vector<double>& koeffizienten) {
        vector<double> ableitung(4);

        ableitung[0] = koeffizienten[1] * 1; 
        ableitung[1] = koeffizienten[2] * 2; 
        ableitung[2] = koeffizienten[3] * 3;
        ableitung[3] = koeffizienten[4] * 4; 

        return ableitung;
    }
};
