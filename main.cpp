#include "Nullstellenberechnung.h"
#include "Eingabe.h"
using namespace std;

int main() {
    PolynomFenster fenster;
    fenster.starten();
    vector<double> ergebnisse = regel({1, 4, -12});
    cout << "ergebnisse: ";
    for (int i = 0; i < ergebnisse.size(); i++) {
        cout << ergebnisse[i] << " ";
    }
    cout << endl;

    return 0;
}
