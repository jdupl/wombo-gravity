/*
 * N-Body simulation
 *
 * Authors: Justin Duplessis, Hugo Côté
 *
 * This file is licensed under GPLv3.
 */

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>

using namespace std;

double dist(double r1[], double r2[]) {
    return sqrt(
          pow((r1[0] - r2[0]), 2)
        + pow((r1[1] - r2[1]), 2)
        + pow((r1[2] - r2[2]), 2)
    );
}

void accel(double m, double r1[], double r2[], double a[]) {
    double distance = pow(dist(r1, r2), 3);
    a[0] += (m/distance) * (r1[0] - r2[0]);
    a[1] += (m/distance) * (r1[1] - r2[1]);
    a[2] += (m/distance) * (r1[2] - r2[2]);
}

int main(int argc, char **argv) {
    ofstream csv_file;
    csv_file.open("data.csv", ios::out);

    double interval = 10;
    double interval_2 = interval * interval;
    double end = 3600 * 24 * 365.25 * 1; // in seconds
    double save_res = 3600 * 24;
    double G = -6.6738480 * pow(10, -11);

    double m[2] = { 1.9891 * pow(10, 30), 5.9736 * pow(10, 24)};
    double v[2][3] = {
            {0, 0, 0},
            {0, 30287, 0}
        };
    double r[2][3] = {
            {0, 0, 0},
            {147098074000, 0, 0}
        };
    long nb = sizeof(m) / sizeof(*m);
    double acc[2][3] = {0};

    csv_file << fixed << r[1][0] << ","<< fixed << r[1][1] << "," << fixed << r[1][2] << '\n'; // Imprime les conditions initiales
    int it = 0;
    for (double t = 0; t < end; t += interval) { // temps qui s'ecoule
        if (it++ * interval >= save_res) {
            it = 0;
            //for (int j = 0; j < nb; j++) {
            csv_file << fixed << r[1][0] << ","<< fixed << r[1][1] << "," << fixed << r[1][2] << '\n';
            //}
        }
        for (int i = 0; i < nb; i++){ // pour chaque astre
            // Calcul de l'acceleration
            acc[i][0] = 0;
            acc[i][1] = 0;
            acc[i][2] = 0;

            for (int j = 0; j < nb; j++) { // Pour l'interaction avec chaque autre astre
                if (i != j) accel(m[j], r[i], r[j], acc[i]);
            }

            acc[i][0] *= G;
            acc[i][1] *= G;
            acc[i][2] *= G;
        }
        for (int i = 0; i < nb; i++) { // pour chaque astre
            // Calcul de la nouvelle position
            r[i][0] += 0.5 * acc[i][0] * interval_2 + v[i][0] * interval;
            r[i][1] += 0.5 * acc[i][1] * interval_2 + v[i][1] * interval;
            r[i][2] += 0.5 * acc[i][2] * interval_2 + v[i][2] * interval;
            // Calcul de la nouvelle vitesse
            v[i][0] += acc[i][0] * interval;
            v[i][1] += acc[i][1] * interval;
            v[i][2] += acc[i][2] * interval;
        }
    }
    csv_file.close();
    return 0;
}
