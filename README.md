# PolynomProjekt

Gemeinsames Klassenprojekt: Polynome einlesen, ableiten, Nullstellen berechnen und darstellen.

## Gruppen und Dateien

| Gruppe                 | Dateien                                            |
|------------------------|----------------------------------------------------|
| Polynom                | `Polynom.h`, `Polynom.cpp`                         |
| Nullstellenberechnung  | `Nullstellenberechnung.h`, `Nullstellenberechnung.cpp` |
| Eingabe                | `Eingabe.h`, `Eingabe.cpp`                         |
| Visuelle Darstellung   | `Darstellung.h`, `Darstellung.cpp`                 |

`main.cpp` verbindet alle Teile.

## Regeln

1. Nie direkt auf `main` arbeiten – jede Gruppe hat ihren eigenen Branch.
2. Nur eigene Dateien ändern. Header (`.h`) nur nach Absprache mit allen Gruppen ändern.
3. Vor dem Arbeiten `git pull` und `git merge origin/main`.
4. Nur Code pushen, der kompiliert.
5. Fertige Teile per Pull Request in `main` bringen.

## Kompilieren

```bash
g++ -std=c++17 *.cpp -o polynom
./polynom
```
