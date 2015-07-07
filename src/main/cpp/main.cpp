#include <stdio.h>
#include <cstdio>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>

#include "body.hpp"

using namespace std;

void accel(body& b1, body& b2);

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

void accel(body& b1, body& b2) {
    double tempx, tempy, tempz, tempo;

    tempx = b1.r.x - b2.r.x;
    tempy = b1.r.y - b2.r.y;
    tempz = b1.r.z - b2.r.z;

    double tmp = pow(tempx, 2) + pow(tempy, 2) + pow(tempz, 2);

    tempo = (b2.m) / (tmp * sqrt(tmp));

    b1.a.x += (tempo) * (tempx);
    b1.a.y += (tempo) * (tempy);
    b1.a.z += (tempo) * (tempz);
}

void computeInterval(vector<body>& bodies) {
    int bodyCount = (int) bodies.size();
    for (int i = 0; i < bodyCount; i++){ // pour chaque astre
        // Calcul de l'acceleration
        bodies[i].a.reset();

        for (int j = 0; j < bodyCount; j++) { // Pour l'interaction avec chaque autre astre
            if (i != j) accel(bodies[i], bodies[j]);
        }

        bodies[i].a.mult(G);
    }
    for (int i = 0; i < bodyCount; i++) { // pour chaque astre
        bodies[i].actualise();
    }
}

void computeFrame(vector<body>& bodies, int intervalCount) {
    clock_t begin = clock();

    int currentIteration = 0;
    while (currentIteration++ < intervalCount) {
        computeInterval(bodies);
    }
    clock_t end = clock();

    cout << "frame took " << double(end - begin) / 1000 << " ms\n";
}

void printSystem(vector<body>& bodies) {
    for (int i =0; i < int(bodies.size()); i++) {
        cout << bodies[i].nom << " x:" << bodies[i].r.x << " y:" << bodies[i].r.y << " z:" << bodies[i].r.z<< "\n";
    }
}

int main(int argc, char** argv) {
    vector <body> bodies = getBodies();
    int maxFrames = 365;
    int currentFrame = 0;

    printSystem(bodies);

    while (currentFrame++ < maxFrames) {
        computeFrame(bodies, 60 * 60 * 24);
    }
    printSystem(bodies);
}
