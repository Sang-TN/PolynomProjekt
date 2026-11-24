#pragma once
#include "raylib.h"
#include "Polynom.h"
#include "Nullstellenberechnung.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

class PolynomEingabe {
public:
    void setzeText(const std::string& text) {
        // Nur bei Änderung neu rechnen, setzeText wird jeden Frame aufgerufen
        if (text == eingabe) return;
        eingabe = text;
        koeffizienten = lesePolynom(eingabe);
        nullstellenListe.clear();
        if (fehler.empty()) {
            ergebnisse.clear(); // regel() sammelt in der globalen Liste
            nullstellenListe = regel(koeffizienten);
        }
    }

    const std::string& text() const { return eingabe; }
    const std::string& meldung() const { return fehler; }
    const std::vector<double>& vector() const { return koeffizienten; }
    const std::vector<double>& nullstellen() const { return nullstellenListe; }

private:
    std::string eingabe;
    std::string fehler = "Bitte eine Funktion eingeben";
    std::vector<double> koeffizienten;
    std::vector<double> nullstellenListe;

    std::vector<double> lesePolynom(const std::string& eingabe) {
    fehler.clear();
    std::vector<double> koeffizienten(1, 0.0);
    std::string text;

    for (char zeichen : eingabe)
        if (!std::isspace(static_cast<unsigned char>(zeichen)))
            text += zeichen;

    if (text.empty()) {
        fehler = "Bitte eine Funktion eingeben";
        return {0.0};
    }

    if (text.front() != '+' && text.front() != '-')
        text = '+' + text;

    size_t start = 0;
    int hoechsterGrad = 0;

    for (size_t i = 1; i <= text.size(); ++i) {
        if (i == text.size() || text[i] == '+' || text[i] == '-') {
            std::string term = text.substr(start, i - start);
            char vorzeichen = term[0];
            term.erase(term.begin());
            if (term.empty()) {
                fehler = "Leerer Term";
                return {};
            }

            if (term.find('*') == 0) term.erase(0, 1);

            size_t xPosition = term.find('x');
            double koeffizient = 1.0;
            int grad = 0;

            if (xPosition == std::string::npos) {
                try { koeffizient = std::stod(term); }
                catch (...) {
                    fehler = "Nur Polynome sind erlaubt";
                    return {};
                }
            } else {
                std::string faktor = term.substr(0, xPosition);
                if (!faktor.empty()) {
                    if (faktor.back() == '*') faktor.pop_back();
                    try { koeffizient = std::stod(faktor); }
                    catch (...) {
                        fehler = "Ungueltiger Koeffizient";
                        return {};
                    }
                }

                std::string rest = term.substr(xPosition + 1);
                if (rest.empty()) {
                    grad = 1;
                } else {
                    if (rest[0] == '^') rest.erase(0, 1);
                    else if (rest[0] == '*') rest.erase(0, 1);
                    else {
                        fehler = "Ungültige Potenz in '" + term + "'";
                        return {};
                    }

                    try { grad = std::stoi(rest); }
                    catch (...) {
                        fehler = "Grad muss eine ganze Zahl sein";
                        return {};
                    }
                    if (grad < 0) {
                        fehler = "Negative Potenzen sind nicht erlaubt";
                        return {};
                    }
                }
            }

            if (vorzeichen == '-') koeffizient = -koeffizient;
            if (grad > hoechsterGrad) {
                hoechsterGrad = grad;
                koeffizienten.resize(static_cast<size_t>(hoechsterGrad) + 1, 0.0);
            }

            koeffizienten[static_cast<size_t>(grad)] += koeffizient;
            start = i;
        }
    }

    while (koeffizienten.size() > 1 && koeffizienten.back() == 0.0)
        koeffizienten.pop_back();

    // Absteigend speichern {an, ..., a1, a0}, so wie regel() es erwartet
    std::reverse(koeffizienten.begin(), koeffizienten.end());
    return koeffizienten;
    }
};

class PolynomFenster {
public:
    void starten() {
        InitWindow(breite, hoehe, "Polynomfunktion eingeben");
        SetExitKey(KEY_NULL);
        SetTargetFPS(30);

        while (!WindowShouldClose()) {
            aktualisieren();
            zeichnen();
        }

        CloseWindow();
    }

    const std::vector<double>& koeffizienten() const {
        return polynom.vector();
    }

    const std::vector<double>& nullstellen() const {
        return polynom.nullstellen();
    }

private:
    static constexpr int breite = 800;
    static constexpr int hoehe = 450;
    const Rectangle eingabeFeld = {180, 20, 600, 40};
    PolynomEingabe polynom;

    void aktualisieren() {
        std::string text = polynom.text();
        if (IsKeyPressed(KEY_BACKSPACE) && !text.empty())
            text.pop_back();

        int zeichen = GetCharPressed();
        while (zeichen > 0) {
            // Auf deutscher Tastatur ist ^ eine Tottaste und kommt teils als
            // U+02C6 bzw. U+0302 an; wir brauchen das ASCII-Zeichen '^'.
            if (zeichen == 0x02C6 || zeichen == 0x0302)
                zeichen = '^';
            // Nur druckbares ASCII übernehmen, alles andere würde als
            // ungültiges UTF-8 bei DrawText zu '?' werden.
            if (zeichen >= 32 && zeichen <= 126 && text.size() < 100)
                text += static_cast<char>(zeichen);
            zeichen = GetCharPressed();
        }
        polynom.setzeText(text);
    }

    void zeichnen() const {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Funktion f(x):", 20, 30, 20, BLACK);
        DrawRectangleRec(eingabeFeld, LIGHTGRAY);
        DrawRectangleLinesEx(eingabeFeld, 2, BLUE);
        DrawText(polynom.text().c_str(), 192, 30, 20, BLACK);

        if (!polynom.meldung().empty())
            DrawText(TextFormat("Fehler: %s", polynom.meldung().c_str()), 20, 70, 17, RED);

        DrawText("Beispiel: 2*x^3 - 3*x^2 + 4*x + 1 oder x^2 - 5", 20, 350, 18, DARKGRAY);
        if (polynom.meldung().empty()) {
            std::string vectorText = "Vector (absteigend): [";
            const auto& werte = polynom.vector();
            for (size_t i = 0; i < werte.size(); ++i) {
                if (i > 0) vectorText += ", ";
                vectorText += std::to_string(werte[i]);
            }
            vectorText += ']';
            DrawText(vectorText.c_str(), 20, 400, 17, MAROON);

            // Zum Testen: berechnete Nullstellen direkt anzeigen
            std::string nullstellenText = "Nullstellen: ";
            const auto& nullstellen = polynom.nullstellen();
            if (nullstellen.empty()) nullstellenText += "keine gefunden";
            for (size_t i = 0; i < nullstellen.size(); ++i) {
                if (i > 0) nullstellenText += ", ";
                nullstellenText += TextFormat("%g", nullstellen[i]);
            }
            DrawText(nullstellenText.c_str(), 20, 425, 17, DARKGREEN);
        }

        EndDrawing();
    }
};

// Gruppe Eingabe
// Liest ein Polynom vom Benutzer ein.
inline Polynom lesePolynomEin() {
    PolynomFenster fenster;
    fenster.starten();
    return Polynom(fenster.koeffizienten());
}

