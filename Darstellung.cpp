// Gruppe Visuelle Darstellung
// Zeichnet ein Polynom mit raylib: Graph mit Achsen, Gitter und Nullstellen.
// Benutzt nur die Schnittstellen aus Polynom.h und Nullstellenberechnung.h.
//
// Bedienung: Mausrad = Zoom, Ziehen = Verschieben, R = Ansicht zuruecksetzen,
//            ESC = Schliessen
#include "Darstellung.h"
#include "Nullstellenberechnung.h"
#include <raylib.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

namespace {

const int PANEL_BREITE = 280;

// ---------------------------------------------------------------------
// Ansicht: Welt-Koordinaten <-> Bildschirm-Pixel
// ---------------------------------------------------------------------
struct Ansicht {
    Rectangle bereich;              // Bildschirm-Rechteck des Graphen
    double xMin, xMax, yMin, yMax;  // Welt-Koordinaten

    Vector2 ZuBildschirm(double x, double y) const {
        return {
            (float)(bereich.x + (x - xMin) / (xMax - xMin) * bereich.width),
            (float)(bereich.y + (yMax - y) / (yMax - yMin) * bereich.height)  // y nach oben
        };
    }
    double ZuWeltX(float sx) const { return xMin + (sx - bereich.x) / bereich.width * (xMax - xMin); }
    double ZuWeltY(float sy) const { return yMax - (sy - bereich.y) / bereich.height * (yMax - yMin); }
};

// ---------------------------------------------------------------------
// Funktionsterm als Text, z.B. "f(x) = 2x^3 - 3x + 1"
// ---------------------------------------------------------------------
std::string Zahl(double z) {
    char puffer[32];
    std::snprintf(puffer, sizeof(puffer), "%g", z);
    return puffer;
}

std::string FunktionText(const Polynom& p) {
    const std::vector<double>& k = p.koeffizienten();
    std::string text;
    for (int i = (int)k.size() - 1; i >= 0; i--) {
        double a = k[i];
        if (a == 0) continue;
        if (!text.empty())  text += a < 0 ? " - " : " + ";
        else if (a < 0)     text += "-";
        double betrag = std::fabs(a);
        if (betrag != 1 || i == 0) text += Zahl(betrag);
        if (i >= 1) text += "x";
        if (i >= 2) text += "^" + std::to_string(i);
    }
    return "f(x) = " + (text.empty() ? std::string("0") : text);
}

// ---------------------------------------------------------------------
// Startansicht: x-Bereich wie vorgegeben, y-Bereich passend zur Kurve
// ---------------------------------------------------------------------
void Einpassen(Ansicht& a, const Polynom& p, double xMin, double xMax) {
    double yMin = INFINITY, yMax = -INFINITY;
    const int SCHRITTE = 400;
    for (int i = 0; i <= SCHRITTE; i++) {
        double y = p.wert(xMin + (xMax - xMin) * i / SCHRITTE);
        if (!std::isfinite(y)) continue;
        yMin = std::min(yMin, y);
        yMax = std::max(yMax, y);
    }
    if (yMin > yMax) { yMin = -5; yMax = 5; }

    double rand = std::max((yMax - yMin) * 0.1, 1.0);
    a.xMin = xMin;        a.xMax = xMax;
    a.yMin = yMin - rand; a.yMax = yMax + rand;
}

// ---------------------------------------------------------------------
// Eingabe: Zoom und Verschieben
// ---------------------------------------------------------------------
void VerarbeiteZoomUndVerschieben(Ansicht& a) {
    Vector2 maus = GetMousePosition();
    if (!CheckCollisionPointRec(maus, a.bereich)) return;

    // Zoomen mit Mausrad (um die Mausposition herum)
    float rad = GetMouseWheelMove();
    if (rad != 0) {
        double mx = a.ZuWeltX(maus.x), my = a.ZuWeltY(maus.y);
        double faktor = rad > 0 ? 0.9 : 1.1;
        a.xMin = mx + (a.xMin - mx) * faktor;  a.xMax = mx + (a.xMax - mx) * faktor;
        a.yMin = my + (a.yMin - my) * faktor;  a.yMax = my + (a.yMax - my) * faktor;
    }

    // Verschieben mit gedrueckter linker Maustaste
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 d = GetMouseDelta();
        double dx = d.x / a.bereich.width * (a.xMax - a.xMin);
        double dy = d.y / a.bereich.height * (a.yMax - a.yMin);
        a.xMin -= dx; a.xMax -= dx;
        a.yMin += dy; a.yMax += dy;
    }
}

// ---------------------------------------------------------------------
// Zeichnen
// ---------------------------------------------------------------------
// "Schoene" Gitterweite: 1, 2 oder 5 mal Zehnerpotenz
double Schrittweite(double spanne) {
    double roh = spanne / 10.0;
    double p = std::pow(10, std::floor(std::log10(roh)));
    double n = roh / p;
    if (n < 1.5) return p;
    if (n < 3.5) return 2 * p;
    if (n < 7.5) return 5 * p;
    return 10 * p;
}

void ZeichneGitter(const Ansicht& a) {
    float links = a.bereich.x, rechts = a.bereich.x + a.bereich.width;
    float oben = a.bereich.y, unten = a.bereich.y + a.bereich.height;

    double sx = Schrittweite(a.xMax - a.xMin);
    for (long long i = (long long)std::ceil(a.xMin / sx); i * sx <= a.xMax; i++) {
        float px = a.ZuBildschirm(i * sx, 0).x;
        DrawLineV({px, oben}, {px, unten}, i == 0 ? DARKGRAY : Fade(LIGHTGRAY, 0.6f));  // i == 0: y-Achse
        DrawText(TextFormat("%g", i * sx), (int)px + 3, (int)unten - 14, 10, GRAY);
    }
    double sy = Schrittweite(a.yMax - a.yMin);
    for (long long i = (long long)std::ceil(a.yMin / sy); i * sy <= a.yMax; i++) {
        float py = a.ZuBildschirm(0, i * sy).y;
        DrawLineV({links, py}, {rechts, py}, i == 0 ? DARKGRAY : Fade(LIGHTGRAY, 0.6f));  // i == 0: x-Achse
        DrawText(TextFormat("%g", i * sy), (int)links + 3, (int)py - 12, 10, GRAY);
    }
}

// Ein Funktionswert pro Pixelspalte, benachbarte Punkte mit Linien verbunden
void ZeichneKurve(const Ansicht& a, const Polynom& p) {
    Vector2 vorher = {};
    bool hatVorher = false;
    for (int px = 0; px <= (int)a.bereich.width; px++) {
        double x = a.ZuWeltX(a.bereich.x + px);
        double y = p.wert(x);
        if (!std::isfinite(y)) { hatVorher = false; continue; }
        Vector2 punkt = a.ZuBildschirm(x, y);
        if (hatVorher) DrawLineEx(vorher, punkt, 2.0f, BLUE);
        vorher = punkt;
        hatVorher = true;
    }
}

void ZeichneGraph(const Ansicht& a, const Polynom& p, const std::vector<double>& nullstellen) {
    DrawRectangleRec(a.bereich, WHITE);
    BeginScissorMode((int)a.bereich.x, (int)a.bereich.y, (int)a.bereich.width, (int)a.bereich.height);

    ZeichneGitter(a);
    ZeichneKurve(a, p);

    // Nullstellen als Punkte auf der x-Achse, z.B. "N(1.73|0)"
    for (double x : nullstellen) {
        Vector2 s = a.ZuBildschirm(x, 0);
        DrawCircleV(s, 6, RED);
        DrawText(TextFormat("N(%.2f|0)", x), (int)s.x + 9, (int)s.y - 18, 14, RED);
    }

    EndScissorMode();
    DrawRectangleLinesEx(a.bereich, 1, GRAY);
}

void ZeichnePanel(const std::string& funktionText, const std::vector<double>& nullstellen, const Ansicht& a) {
    const int x = GetScreenWidth() - PANEL_BREITE + 15;
    const int yEnde = GetScreenHeight() - 40;
    int y = 15;

    DrawText(funktionText.c_str(), x, y, 20, BLACK);
    y += 40;

    DrawText(TextFormat("Nullstellen (%d):", (int)nullstellen.size()), x, y, 18, BLACK);
    y += 26;
    if (nullstellen.empty()) {
        DrawText("keine", x + 10, y, 16, DARKGRAY);
    }
    for (double n : nullstellen) {
        if (y > yEnde) break;
        DrawText(TextFormat("x = %.3f", n), x + 10, y, 16, DARKGRAY);
        y += 20;
    }

    // Unten: Koordinaten der Maus oder Bedienhinweis
    Vector2 maus = GetMousePosition();
    if (CheckCollisionPointRec(maus, a.bereich))
        DrawText(TextFormat("x = %.3f   y = %.3f", a.ZuWeltX(maus.x), a.ZuWeltY(maus.y)),
                 x, GetScreenHeight() - 30, 14, GRAY);
    else
        DrawText("Rad: Zoom  Ziehen: Verschieben  R: Reset", x, GetScreenHeight() - 30, 12, GRAY);
}

}  // namespace

// ---------------------------------------------------------------------
// Schnittstelle aus Darstellung.h
// ---------------------------------------------------------------------
void zeichne(const Polynom& p, double xMin, double xMax) {
    if (xMin >= xMax) { xMin = -10; xMax = 10; }

    // Einmal vorbereiten: Text und Nullstellen (von der Nullstellen-Gruppe)
    std::string funktionText = FunktionText(p);
    std::vector<double> nullstellen;
    if (p.grad() >= 1) nullstellen = berechneNullstellen(p);

    // Falls z.B. die Eingabe schon ein raylib-Fenster offen hat, dieses benutzen
    bool eigenesFenster = !IsWindowReady();
    if (eigenesFenster) {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
        InitWindow(1100, 650, "Polynom - Darstellung");
        SetTargetFPS(60);
    }

    Ansicht ansicht = {};
    Einpassen(ansicht, p, xMin, xMax);

    // Hauptschleife: 60-mal pro Sekunde Eingabe verarbeiten und alles neu zeichnen
    while (!WindowShouldClose()) {
        ansicht.bereich = {10, 10,
                           (float)(GetScreenWidth() - PANEL_BREITE - 20),
                           (float)(GetScreenHeight() - 20)};

        VerarbeiteZoomUndVerschieben(ansicht);
        if (IsKeyPressed(KEY_R)) Einpassen(ansicht, p, xMin, xMax);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        ZeichneGraph(ansicht, p, nullstellen);
        ZeichnePanel(funktionText, nullstellen, ansicht);
        EndDrawing();
    }

    if (eigenesFenster) CloseWindow();
}
