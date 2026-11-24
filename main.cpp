#include "Nullstellenberechnung.h"
#include "Eingabe.h"
using namespace std;

int main() {
    PolynomFenster fenster;
    fenster.starten();
    vector<double> ergebnisse = regel(fenster.koeffizienten());

    return 0;
}
