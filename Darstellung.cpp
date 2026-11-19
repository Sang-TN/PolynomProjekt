// Gruppe Visuelle Darstellung
// Zeichnet ein Polynom mit raylib: Graph, Ableitungen und besondere Punkte.
// Benutzt nur die Schnittstellen aus Polynom.h und Nullstellenberechnung.h.
//
// Bedienung: Mausrad = Zoom, Ziehen = Verschieben, R = Ansicht zuruecksetzen,
//            Checkboxen rechts = Kurven/Punkte ein- und ausblenden, ESC = Schliessen
#include "Darstellung.h"
#include "Nullstellenberechnung.h"
#include <raylib.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>

namespace {

const int PANEL_BREITE = 320;

struct Punkt {
    double x, y;
};

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

struct Kurve {
    const char* name;
    std::function<double(double)> fn;
    Color farbe;
    bool sichtbar;
};

struct Kategorie {
    const char* name;
    const char* kuerzel;  // Beschriftung im Graphen, z.B. "H"
    Color farbe;
    std::vector<Punkt> punkte;
    bool sichtbar;
};

// ---------------------------------------------------------------------
// Texte zum Polynom
// ---------------------------------------------------------------------
std::string Zahl(double z) {
    char puffer[32];
    std::snprintf(puffer, sizeof(puffer), "%g", z);
    return puffer;
}

// z.B. "f(x) = 2x^3 - 3x + 1"
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

std::vector<std::string> Infos(const Polynom& p) {
    const std::vector<double>& k = p.koeffizienten();
    std::vector<std::string> infos;
    int n = p.grad();
    infos.push_back("Grad: " + std::to_string(n));
    infos.push_back("y-Achsenabschnitt: (0 | " + Zahl(p.wert(0)) + ")");

    bool nurGerade = true, nurUngerade = true;
    for (size_t i = 0; i < k.size(); i++) {
        if (k[i] == 0) continue;
        if (i % 2 == 0) nurUngerade = false;
        else            nurGerade = false;
    }
    if (n >= 1 && nurGerade)        infos.push_back("Symmetrie: zur y-Achse");
    else if (n >= 1 && nurUngerade) infos.push_back("Symmetrie: zum Ursprung");
    else                            infos.push_back("Symmetrie: keine");

    // Verhalten im Unendlichen haengt vom Leitkoeffizienten und vom Grad ab
    if (n >= 1 && n < (int)k.size()) {
        bool rechtsPlus = k[n] > 0;
        bool linksPlus  = (n % 2 == 0) ? rechtsPlus : !rechtsPlus;
        infos.push_back(std::string("x -> +oo: f(x) -> ") + (rechtsPlus ? "+oo" : "-oo"));
        infos.push_back(std::string("x -> -oo: f(x) -> ") + (linksPlus ? "+oo" : "-oo"));
    }
    return infos;
}

// ---------------------------------------------------------------------
// Besondere Punkte (ueber berechneNullstellen der anderen Gruppe)
// ---------------------------------------------------------------------
// Vorzeichenwechsel von g an der Stelle x: -1 (+ nach -), +1 (- nach +), 0 (keiner)
int Vorzeichenwechsel(const Polynom& g, double x) {
    const double h = 1e-4;
    double links = g.wert(x - h), rechts = g.wert(x + h);
    if (links > 0 && rechts < 0) return -1;
    if (links < 0 && rechts > 0) return +1;
    return 0;
}

void BerechnePunkte(const Polynom& p, std::vector<Kategorie>& kategorien) {
    Polynom f1 = p.ableitung();
    Polynom f2 = f1.ableitung();

    if (p.grad() >= 1)
        for (double x : berechneNullstellen(p))
            kategorien[0].punkte.push_back({x, 0});

    // Extremstellen: f'(x) = 0 mit Vorzeichenwechsel
    if (p.grad() >= 2)
        for (double x : berechneNullstellen(f1)) {
            int vzw = Vorzeichenwechsel(f1, x);
            if (vzw < 0) kategorien[1].punkte.push_back({x, p.wert(x)});  // Hochpunkt
            if (vzw > 0) kategorien[2].punkte.push_back({x, p.wert(x)});  // Tiefpunkt
        }

    // Wendestellen: f''(x) = 0 mit Vorzeichenwechsel
    if (p.grad() >= 3)
        for (double x : berechneNullstellen(f2))
            if (Vorzeichenwechsel(f2, x) != 0)
                kategorien[3].punkte.push_back({x, p.wert(x)});
}

// ---------------------------------------------------------------------
// Startansicht: x-Bereich wie vorgegeben, y-Bereich passend zur Kurve
// ---------------------------------------------------------------------
void Einpassen(Ansicht& a, const Polynom& p, double xMin, double xMax,
               const std::vector<Kategorie>& kategorien) {
    double yMin = INFINITY, yMax = -INFINITY;
    const int SCHRITTE = 400;
    for (int i = 0; i <= SCHRITTE; i++) {
        double y = p.wert(xMin + (xMax - xMin) * i / SCHRITTE);
        if (!std::isfinite(y)) continue;
        yMin = std::min(yMin, y);
        yMax = std::max(yMax, y);
    }
    for (const Kategorie& k : kategorien)
        for (const Punkt& pt : k.punkte)
            if (pt.x >= xMin && pt.x <= xMax) {
                yMin = std::min(yMin, pt.y);
                yMax = std::max(yMax, pt.y);
            }
    if (yMin > yMax) { yMin = -5; yMax = 5; }

    double rand = std::max((yMax - yMin) * 0.1, 1.0);
    a.xMin = xMin;        a.xMax = xMax;
    a.yMin = yMin - rand; a.yMax = yMax + rand;
}

// ---------------------------------------------------------------------
// Eingabe
// ---------------------------------------------------------------------
void VerarbeiteZoomUndVerschieben(Ansicht& a) {
    Vector2 maus = GetMousePosition();
    bool imGraph = CheckCollisionPointRec(maus, a.bereich);

    // Zoomen mit Mausrad (um die Mausposition herum)
    float rad = GetMouseWheelMove();
    if (rad != 0 && imGraph) {
        double mx = a.ZuWeltX(maus.x), my = a.ZuWeltY(maus.y);
        double faktor = rad > 0 ? 0.9 : 1.1;
        a.xMin = mx + (a.xMin - mx) * faktor;  a.xMax = mx + (a.xMax - mx) * faktor;
        a.yMin = my + (a.yMin - my) * faktor;  a.yMax = my + (a.yMax - my) * faktor;
    }

    // Verschieben mit gedrueckter linker Maustaste
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && imGraph) {
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
        DrawLineV({px, oben}, {px, unten}, i == 0 ? DARKGRAY : Fade(LIGHTGRAY, 0.6f));
        DrawText(TextFormat("%g", i * sx), (int)px + 3, (int)unten - 14, 10, GRAY);
    }
    double sy = Schrittweite(a.yMax - a.yMin);
    for (long long i = (long long)std::ceil(a.yMin / sy); i * sy <= a.yMax; i++) {
        float py = a.ZuBildschirm(0, i * sy).y;
        DrawLineV({links, py}, {rechts, py}, i == 0 ? DARKGRAY : Fade(LIGHTGRAY, 0.6f));
        DrawText(TextFormat("%g", i * sy), (int)links + 3, (int)py - 12, 10, GRAY);
    }
}

void ZeichneKurve(const Ansicht& a, const Kurve& k) {
    Vector2 vorher = {};
    bool hatVorher = false;
    for (int px = 0; px <= (int)a.bereich.width; px++) {  // ein Wert pro Pixel
        double x = a.ZuWeltX(a.bereich.x + px);
        double y = k.fn(x);
        if (!std::isfinite(y)) { hatVorher = false; continue; }
        Vector2 p = a.ZuBildschirm(x, y);
        if (hatVorher) DrawLineEx(vorher, p, 2.0f, k.farbe);
        vorher = p;
        hatVorher = true;
    }
}

void ZeichneGraph(const Ansicht& a, const std::vector<Kurve>& kurven, const std::vector<Kategorie>& kategorien) {
    DrawRectangleRec(a.bereich, WHITE);
    BeginScissorMode((int)a.bereich.x, (int)a.bereich.y, (int)a.bereich.width, (int)a.bereich.height);

    ZeichneGitter(a);
    for (const Kurve& k : kurven)
        if (k.sichtbar) ZeichneKurve(a, k);

    // Besondere Punkte mit Beschriftung, z.B. "H(-1|3)"
    for (const Kategorie& k : kategorien) {
        if (!k.sichtbar) continue;
        for (const Punkt& p : k.punkte) {
            Vector2 s = a.ZuBildschirm(p.x, p.y);
            DrawCircleV(s, 6, k.farbe);
            DrawCircleLinesV(s, 6, WHITE);
            DrawText(TextFormat("%s(%.2f|%.2f)", k.kuerzel, p.x, p.y), (int)s.x + 9, (int)s.y - 18, 14, k.farbe);
        }
    }

    // Fadenkreuz an der Mausposition
    Vector2 maus = GetMousePosition();
    if (CheckCollisionPointRec(maus, a.bereich)) {
        double x = a.ZuWeltX(maus.x);
        DrawLineV({maus.x, a.bereich.y}, {maus.x, a.bereich.y + a.bereich.height}, Fade(GRAY, 0.5f));
        for (const Kurve& k : kurven)
            if (k.sichtbar) DrawCircleV(a.ZuBildschirm(x, k.fn(x)), 4, k.farbe);
    }

    EndScissorMode();
    DrawRectangleLinesEx(a.bereich, 1, GRAY);
}

// Einfache Checkbox, gibt den neuen Zustand zurueck
bool Checkbox(Rectangle r, const char* text, bool wert, Color farbe) {
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), r))
        wert = !wert;
    DrawRectangleLinesEx(r, 2, farbe);
    if (wert) DrawRectangle((int)r.x + 4, (int)r.y + 4, (int)r.width - 8, (int)r.height - 8, farbe);
    DrawText(text, (int)(r.x + r.width + 8), (int)r.y + 1, 18, DARKGRAY);
    return wert;
}

void ZeichnePanel(const std::string& funktionText, const std::vector<std::string>& infos, const Ansicht& a,
                  std::vector<Kurve>& kurven, std::vector<Kategorie>& kategorien) {
    const int x = GetScreenWidth() - PANEL_BREITE + 15;
    const int yEnde = GetScreenHeight() - 40;
    int y = 15;

    DrawText(funktionText.c_str(), x, y, 20, BLACK);  y += 36;

    // Kurven ein-/ausblenden
    for (Kurve& k : kurven) {
        k.sichtbar = Checkbox({(float)x, (float)y, 20, 20}, k.name, k.sichtbar, k.farbe);
        y += 26;
    }
    y += 10;

    // Punkte: pro Kategorie eine Checkbox und darunter die Liste
    for (Kategorie& k : kategorien) {
        if (y > yEnde) break;
        k.sichtbar = Checkbox({(float)x, (float)y, 20, 20},
                              TextFormat("%s (%d)", k.name, (int)k.punkte.size()), k.sichtbar, k.farbe);
        y += 24;
        for (const Punkt& p : k.punkte) {
            if (y > yEnde) break;
            DrawText(TextFormat("%s(%.3f | %.3f)", k.kuerzel, p.x, p.y), x + 28, y, 16, DARKGRAY);
            y += 20;
        }
        y += 6;
    }

    // Weitere Infos zum Polynom
    if (!infos.empty() && y < yEnde) {
        y += 6;
        DrawText("Weitere Infos:", x, y, 18, BLACK);  y += 24;
        for (const std::string& zeile : infos) {
            if (y > yEnde) break;
            DrawText(zeile.c_str(), x, y, 16, DARKGRAY);
            y += 20;
        }
    }

    // Werte an der Mausposition (ganz unten)
    Vector2 maus = GetMousePosition();
    if (CheckCollisionPointRec(maus, a.bereich)) {
        double mx = a.ZuWeltX(maus.x);
        DrawText(TextFormat("x = %.3f   y = %.3f", mx, a.ZuWeltY(maus.y)), x, GetScreenHeight() - 30, 14, GRAY);
    } else {
        DrawText("Rad: Zoom  Ziehen: Verschieben  R: Reset", x, GetScreenHeight() - 30, 12, GRAY);
    }
}

}  // namespace

// ---------------------------------------------------------------------
// Schnittstelle aus Darstellung.h
// ---------------------------------------------------------------------
void zeichne(const Polynom& p, double xMin, double xMax) {
    if (xMin >= xMax) { xMin = -10; xMax = 10; }

    Polynom f1 = p.ableitung();
    Polynom f2 = f1.ableitung();
    std::vector<Kurve> kurven = {
        {"f(x)",   [p](double x)  { return p.wert(x); },  BLUE,   true},
        {"f'(x)",  [f1](double x) { return f1.wert(x); }, ORANGE, false},
        {"f''(x)", [f2](double x) { return f2.wert(x); }, PURPLE, false},
    };

    std::vector<Kategorie> kategorien = {
        {"Nullstellen", "N", BLACK,     {}, true},
        {"Hochpunkte",  "H", RED,       {}, true},
        {"Tiefpunkte",  "T", DARKGREEN, {}, true},
        {"Wendepunkte", "W", MAGENTA,   {}, true},
    };
    BerechnePunkte(p, kategorien);

    std::string funktionText = FunktionText(p);
    std::vector<std::string> infos = Infos(p);

    // Falls z.B. die Eingabe schon ein raylib-Fenster offen hat, dieses benutzen
    bool eigenesFenster = !IsWindowReady();
    if (eigenesFenster) {
        SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
        InitWindow(1200, 700, "Polynom - Darstellung");
        SetTargetFPS(60);
    }

    Ansicht ansicht = {};
    Einpassen(ansicht, p, xMin, xMax, kategorien);

    while (!WindowShouldClose()) {
        // Graph-Bereich an Fenstergroesse anpassen (links Graph, rechts Panel)
        ansicht.bereich = {10, 10,
                           (float)(GetScreenWidth() - PANEL_BREITE - 20),
                           (float)(GetScreenHeight() - 20)};

        VerarbeiteZoomUndVerschieben(ansicht);
        if (IsKeyPressed(KEY_R)) Einpassen(ansicht, p, xMin, xMax, kategorien);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        ZeichneGraph(ansicht, kurven, kategorien);
        ZeichnePanel(funktionText, infos, ansicht, kurven, kategorien);
        EndDrawing();
    }

    if (eigenesFenster) CloseWindow();
}
