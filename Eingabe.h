#pragma once
#include "raylib.h"
#include "Polynom.h"
#include <cctype>
#include <string>
#include <vector>

class PolynomEingabe {
public:
    void setzeText(const std::string& text) {
        eingabe = text;
        koeffizienten = lesePolynom(eingabe);
    }

    const std::string& text() const { return eingabe; }
    const std::string& meldung() const { return fehler; }
    const std::vector<double>& vector() const { return koeffizienten; }

private:
    std::string eingabe;
    std::string fehler;
    std::vector<double> koeffizienten;

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
            if (text.size() < 100)
                text += static_cast<char>(zeichen);
            zeichen = GetCharPressed();
        }
        polynom.setzeText(text);
    }

    void zeichnen() const {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        const int offsetX = IsWindowFullscreen() ? (GetScreenWidth() - breite) / 2 : 0;
        const int offsetY = IsWindowFullscreen() ? (GetScreenHeight() - hoehe) / 2 : 0;
        Rectangle feld = eingabeFeld;
        feld.x += offsetX;
        feld.y += offsetY;

        DrawText("Funktion f(x):", 20 + offsetX, 30 + offsetY, 20, BLACK);
        DrawRectangleRec(feld, LIGHTGRAY);
        DrawRectangleLinesEx(feld, 2, BLUE);
        DrawText(polynom.text().c_str(), 192 + offsetX, 30 + offsetY, 20, BLACK);

        if (!polynom.meldung().empty())
            DrawText(TextFormat("Fehler: %s", polynom.meldung().c_str()), 20 + offsetX, 70 + offsetY, 17, RED);

        DrawText("Beispiel: 2*x^3 - 3*x^2 + 4*x + 1 oder x^2 - 5", 20 + offsetX, 350 + offsetY, 18, DARKGRAY);
        if (polynom.meldung().empty()) {
            std::string vectorText = "Vector (aufsteigend): [";
            const auto& werte = polynom.vector();
            for (size_t i = 0; i < werte.size(); ++i) {
                if (i > 0) vectorText += ", ";
                vectorText += std::to_string(werte[i]);
            }
            vectorText += ']';
            DrawText(vectorText.c_str(), 20 + offsetX, 400 + offsetY, 17, MAROON);
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

