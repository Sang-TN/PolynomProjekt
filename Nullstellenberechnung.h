#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

inline vector<double> ergebnisse;
inline vector<double> regel(vector<double> k)
{
    int grad = k.size() - 1;

    if (grad == 1)
    {
        double a = k[0];
        double b = k[1];

        if (a == 0 && b != 0)
        {
        }
        else if (a != 0)
        {
            double x = -b / a;
            ergebnisse.push_back(x);
        }
    }

    if (grad == 2)
    {
        double b = k[1] / k[0];
        double c = k[2] / k[0];

        double vor_pq = -(b / 2);
        double fehler_inwurzel = pow(b / 2, 2) - c;

        if (fehler_inwurzel < 0)
        {
        }
        else
        {
            double nach_pq = sqrt(fehler_inwurzel);
            vector<double> x(2);
            x[0] = vor_pq + nach_pq;
            x[1] = vor_pq - nach_pq;

            ergebnisse.push_back(x[0]);
            ergebnisse.push_back(x[1]);
        }
    }
    if (grad == 3)
    {
        if (k[k.size() - 1] == 0)
        {
            ergebnisse.push_back(0);                       // x ausklammern x = 0
            regel({k[0], k[1], k[2]});        // Rest x² - 2x + 2 lösen (pusht selbst in ergebnisse)
        }
        else
        {
            // Polynomdivision
            double d = fabs(k[3]);
            for (int t = 1; t <= d; t++)
            {
                if (fmod(d, t) != 0)
                {
                    continue;
                }
                for (int vorzeichen = 1; vorzeichen >= -1; vorzeichen -= 2)
                {
                    double r = t * vorzeichen;
                    double y = ((k[0] * r + k[1]) * r + k[2]) * r + k[3];
                    if (fabs(y) < 1e-9)
                    {
                        ergebnisse.push_back(r);
                        // Horner-Schema
                        double q0 = k[0];
                        double q1 = k[1] + r * q0;
                        double q2 = k[2] + r * q1;
                        regel({q0, q1, q2}); 
                        return ergebnisse;
                    }
                }
            }
        }
    }
    if (grad == 4 && k[1] == 0 && k[3] == 0)
    {
        // Substitution 
        double b = k[2] / k[0];
        double c = k[4] / k[0];

        double vor_pq = -(b / 2);
        double fehler_inwurzel = pow(b / 2, 2) - c;

        if (fehler_inwurzel < 0)
        {
        }
        else
        {
            double nach_pq = sqrt(fehler_inwurzel);
            vector<double> z(2);
            z[0] = vor_pq + nach_pq;
            z[1] = vor_pq - nach_pq;

            for (int i = 0; i < 2; i++)
            {
                if (z[i] >= 0)
                {
                    ergebnisse.push_back(sqrt(z[i]));
                    ergebnisse.push_back(-sqrt(z[i]));
                }
            }
        }
    }
    return ergebnisse;
}
