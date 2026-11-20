// Gruppe Visuelle Darstellung
// Zeichnet den Graphen eines Polynoms mit raylib: Achsen, Kurve und Nullstellen.
#include "Darstellung.h"
#include "Nullstellenberechnung.h"
#include <raylib.h>
#include <vector>

const int BREITE = 700;
const int HOEHE  = 700;

void zeichne(const Polynom& p, double xMin, double xMax) {
    // y-Bereich genauso gross wie der x-Bereich (z.B. -10 bis 10)
    double yMin = xMin;
    double yMax = xMax;

    // Umrechnung: Mathe-Koordinate -> Pixel im Fenster
    auto pixelX = [&](double x) { return (float)((x - xMin) / (xMax - xMin) * BREITE); };
    auto pixelY = [&](double y) { return (float)((yMax - y) / (yMax - yMin) * HOEHE); };  // y nach oben

    // Nullstellen einmal von der Nullstellen-Gruppe holen
    std::vector<double> nullstellen;
    if (p.grad() >= 1) nullstellen = berechneNullstellen(p);

    InitWindow(BREITE, HOEHE, "Polynom - Darstellung");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {  // laeuft bis ESC oder Fenster schliessen
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // 1. Achsen
        DrawLineV({0, pixelY(0)}, {BREITE, pixelY(0)}, BLACK);  // x-Achse
        DrawLineV({pixelX(0), 0}, {pixelX(0), HOEHE}, BLACK);   // y-Achse

        // 2. Einteilung: kleiner Strich und Zahl bei jeder ganzen Zahl
        for (int i = (int)xMin; i <= (int)xMax; i++) {
            if (i == 0) continue;
            DrawLineV({pixelX(i), pixelY(0) - 4}, {pixelX(i), pixelY(0) + 4}, BLACK);
            DrawText(TextFormat("%d", i), (int)pixelX(i) - 4, (int)pixelY(0) + 8, 10, GRAY);
        }
        for (int i = (int)yMin; i <= (int)yMax; i++) {
            if (i == 0) continue;
            DrawLineV({pixelX(0) - 4, pixelY(i)}, {pixelX(0) + 4, pixelY(i)}, BLACK);
            DrawText(TextFormat("%d", i), (int)pixelX(0) + 8, (int)pixelY(i) - 5, 10, GRAY);
        }

        // 3. Graph: fuer jede Pixelspalte f(x) berechnen und mit der naechsten verbinden
        for (int px = 0; px < BREITE; px++) {
            double x1 = xMin + (xMax - xMin) * px / BREITE;
            double x2 = xMin + (xMax - xMin) * (px + 1) / BREITE;
            DrawLineEx({(float)px, pixelY(p.wert(x1))}, {(float)(px + 1), pixelY(p.wert(x2))}, 2, BLUE);
        }

        // 4. Nullstellen als rote Punkte auf der x-Achse
        for (double n : nullstellen) {
            DrawCircleV({pixelX(n), pixelY(0)}, 5, RED);
        }

        EndDrawing();
    }

    CloseWindow();
}
