#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

vector<double> ergebnise;

vector<double> regel(vector<double> k)
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
            ergebnise.push_back(x);
        }
    }

    if (grad == 2)
    {
        double a = k[0] / a;
        double b = k[1] / a;
        double c = k[2] / a;

        double vor_pq = -(b / 2);
        double feher_inwurzel = pow(b / 2, 2) - c;

        if (feher_inwurzel < 0)
        {
        }
        else if
        {
            double nach_pq = sqrt(feher_inwurzel);
            vector<double> x(2);
            x[0] = vor_pq + nach_pq;
            x[1] = vor_pq - nach_pq;

            ergebnise.push_back(x[0]);
            ergebnise.push_back(x[1]);
        }
    }
    if (grad == 3)
    {
        if (k[k.size() - 1] == 0)
        { // macht y-aschenabschnit schau ob es gibt
            // ausklammern x³+4x²+ 0x+ 0 = x³+4x² =

            cout << k[0];

            cout << "test";
        }
        else
        {
            for (int i = 0; i < k.size(); i++)
            {
                if (k[i] != 0)
                {
                    // polynom devidsion
                    cout << "test " << k[i];
                }
            }
            for (int i = 0; i < k.size(); i++)
            {
                if (fmod(k[i], 2) == 0)
                {
                    // graden 2,4,6,8
                    cout << "test";
                    for (int i = 0; i < k.size(); i++)
                    {
                        k[i] = k[i] / k[0];
                    }
                }
            }
            if (k[0] != 0)
            {
                for (int i = k.size() - 1; i >= 0; i--)
                {
                    k[i] = k[i] / k[0];
                    return vector<double>(k[1]);
                }
            }
        }
    }
}
/*
int main()
{
    cout << regel({2, 3, 1, 3}) << endl;

    cout << "ergebnise: ";
    for (double e = 0; e < ergebnise.size(); e++) {
        cout << ergebnise[e] << " ";
    }
    cout << endl;

}*/