#pragma once
#include <vector>
#include "Polynom.h"

// Gruppe Nullstellenberechnung
// Gibt alle reellen Nullstellen aufsteigend sortiert zurueck (leer, wenn keine).
std::vector<double> berechneNullstellen(const Polynom& p);
