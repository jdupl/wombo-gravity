#include <stdio.h>
#include <cstdio>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>

#include "body.hpp"

using namespace std;

vector<body> getBodies() {
    vector <body> bodies;
    ifstream infile("data/data.csv");

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        istringstream ss(s);
        vector <string> record;

        while (ss) {
            if (!getline(ss, s, ',')) break;
            record.push_back(s);
        }

        bodies.push_back(body(record));
    }
    if (!infile.eof()) {
        cerr << "Error reading file!\n";
    }
    return bodies;
}

int main(int argc, char** argv) {
    vector <body> bodies = getBodies();

    for (int i =0;i < int(bodies.size()); i++) {
        cout << bodies[i].nom;
    }
    /*
    for (int i = 0; i < nbbodies; i++){ // pour chaque astre
        // Calcul de l'acceleration
        bodies[i].a.reset();

        for (int j = 0; j < nbbodies; j++) { // Pour l'interaction avec chaque autre astre
            if (i != j) accel(bodies[i], bodies[j]);
        }

        bodies[i].a.mult(G);
    }
    for (int i = 0; i < nbbodies; i++) { // pour chaque astre
        bodies[i].actualise();
    }
    */

}
