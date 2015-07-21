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
#include <limits>       //for numeric_limits<double>

#include "../../../lib/json/json/json.h"

#include "body.hpp"

static unsigned int frameNb = 0;
using namespace std;

/**/
vector<body> getBodies();
void appendJsonFrame(const vector<body>& , Json::Value& , unsigned int);
Json::Value getSystemJson(vector<body>&);
void printJson(Json::Value&, string, string);
void printTime(double);
/**/

/**/
double tidalForce(body&, body&);
void computeTimeScales(vector<body>&);
void computeInterval(vector<body>&, const unsigned int&);
void computeFrame(vector<body>&, const unsigned int&);
void accel(body&, body&, const unsigned int, const unsigned int);
/**/

/**/
double systemEnergy(vector<body>&);
void printSimulationLog(vector<body>&, double);
void printTidalForces(vector<body>&);
/**/

int main(int argc, char** argv) {
    const unsigned int maxFrames = 365;
    const unsigned int frameResolution = 60 * 60 * 24;

    Json::Value jsonFrameBuffer;
    string jsonOutputFile = "data/out.json";
    string jsonFrameFile = "data/frames.json";

    clock_t begin = clock();
    vector <body> bodies = getBodies();

    unsigned int currentFrame = 0;
    double energy = systemEnergy(bodies);

    appendJsonFrame(bodies, jsonFrameBuffer, currentFrame);
    while (currentFrame ++ < maxFrames) {
        computeFrame(bodies, frameResolution);
        appendJsonFrame(bodies, jsonFrameBuffer, currentFrame);
    }

    remove(jsonOutputFile.c_str());
    printJson(jsonFrameBuffer, "frameArray", jsonFrameFile);
    Json::Value tmp = getSystemJson(bodies);
    printJson(tmp, "bodies", jsonOutputFile);

    printSimulationLog(bodies, energy);
//  printTidalForces(bodies);

    cout << "simulation took ";
    printTime(clock() - begin);
    cout << "\n";

    return 0;
}

void printJson(Json::Value& json, string rootName, string filename) {
    Json::Value root;
    root[rootName] = json;
    ofstream jsonOut;

    jsonOut.open(filename.c_str());
    jsonOut << root;
    jsonOut.close();
}

void appendJsonFrame(const vector<body>& bodies, Json::Value& json, unsigned int frameNumber) {
    Json::Value currentFrame;
    stringstream ss;
    ss << setw(10) << right << setfill('0') << frameNumber;

    for (unsigned int i = 0; i < bodies.size(); i++) {
        currentFrame[ss.str()].append(bodies[i].toJsonLight());
    }

    json.append(currentFrame);
}

vector<body> getBodies() {
    vector <body> bodies;
    ifstream inFile("data/data.json");

    Json::Value root;
    Json::Reader reader;

    reader.parse( inFile, root);
    unsigned int count = root["bodies"].size();

    for(unsigned int i = 0; i < count; i++){
        bodies.push_back(body(root["bodies"][i]));
    }
    inFile.close();
    return bodies;
}

void accel(body& b1, body& b2, const unsigned int i, const unsigned int j) {

    if( b1.memo[j].timer != 0) return;

    double tmpX, tmpY, tmpZ, tmp;

    tmpX = b1.r.x - b2.r.x;
    tmpY = b1.r.y - b2.r.y;
    tmpZ = b1.r.z - b2.r.z;

    tmp = pow(tmpX, 2) + pow(tmpY, 2) + pow(tmpZ, 2);

    tmp = ((b1.m) * (b2.m)) / (tmp * sqrt(tmp));

    tmpX *= tmp;
    tmpY *= tmp;
    tmpZ *= tmp;

    b1.memo[j].a.x = tmpX;
    b1.memo[j].a.y = tmpY;
    b1.memo[j].a.z = tmpZ;

    b2.memo[i].a.x = -tmpX;
    b2.memo[i].a.y = -tmpY;
    b2.memo[i].a.z = -tmpZ;
}

double tidalForce(body& b1, body& b2) {
    double tmpX, tmpY, tmpZ, tmp;

    tmpX = b1.r.x - b2.r.x;
    tmpY = b1.r.y - b2.r.y;
    tmpZ = b1.r.z - b2.r.z;

    tmp = pow(tmpX, 2) + pow(tmpY, 2) + pow(tmpZ, 2);

    tmp = ((b1.m) * (b2.m)) / (tmp * sqrt(tmp));

    return tmp;
}

void computeTimeScales(vector<body>& bodies) {
    int count = int(bodies.size());
    double tmp1, tmp2, tmp3;
    gravAcc tmp;

    for (int i = 0; i < count; i++) {
        bodies[i].memo.clear();
    }

    for (int i = 0; i < count; i++) {
        tmp.limit = 0;
        bodies[i].timeStep = 0;
        bodies[i].memo.push_back(tmp);
        for (int j = i + 1; j < count; j++) {
            tmp3 = - tidalForce(bodies[i], bodies[j]);
            tmp1 = tmp3 / bodies[i].m;
            tmp2 = tmp3 / bodies[j].m;
            tmp3 = max( tmp1, tmp2);
//time steps must be divisor of 60 * 60 * 24 * 1000 = 86 400 000
            int odg = 0;
            while(1){
                if( tmp3 > pow(100, -7 -odg) ){
                    //tmp.limit = MDT;
                    tmp.limit = pow( 2, odg);
                    break;
                }
                odg ++;
            }
            if (tmp.limit == 0 ) cout << "gros probleme" << endl;
            if( tmp.timer != 0 ) cout << "gros probleme" << endl;

            bodies[i].memo.push_back(tmp);
            bodies[j].memo.push_back(tmp);

        }
    }
}

void computeInterval(vector<body>& bodies) {
    const unsigned int count = bodies.size();

    for (unsigned int i = 0; i < count; i++) {
        for (unsigned int j = i + 1; j < count; j++) {
            accel(bodies[i], bodies[j], i ,j);
        }
    }

    for (unsigned int i = 0; i < count; i++) {
        bodies[i].actualise();
    }
}

void computeFrame(vector<body>& bodies, const unsigned int& intervalCount) {

    clock_t begin = clock();


    computeTimeScales(bodies);

    double  currentIteration = 0;
    while(currentIteration < intervalCount) {
        computeInterval(bodies);
        currentIteration += DT;
    }


    cout << "frame " << setfill(' ') << setw(3) << right << frameNb++ << " took ";
    printTime(clock() - begin);
    cout << "\n";


}

Json::Value getSystemJson(vector<body>& bodies) {
    Json::Value bodies_node;

    for (unsigned int i = 0; i < bodies.size(); i++) {
        bodies_node.append(bodies[i].toJson());
    }

    return bodies_node;
}

void printTime(double t) {
    cout.precision(1);
    cout << setw(5) << fixed << left << setfill('0') << t / 1000 << " ms" << flush;
}

double systemEnergy(vector<body>& bodies) {
    int count = int(bodies.size());
    double totalEnergy = 0;
    double tmpX, tmpY, tmpZ, tmpD, tmp;

    for (int i = 0; i < count; i++) {
        tmp =
            pow(bodies[i].v.x, 2) +
            pow(bodies[i].v.y, 2) +
            pow(bodies[i].v.z, 2);

        tmp *= bodies[i].m * 0.5 * G_INV;

        for (int j = i + 1; j < count; j++) {

            tmpX = bodies[i].r.x - bodies[j].r.x;
            tmpY = bodies[i].r.y - bodies[j].r.y;
            tmpZ = bodies[i].r.z - bodies[j].r.z;
            tmpD = sqrt(pow(tmpX, 2) + pow(tmpY, 2) + pow(tmpZ, 2));
            tmp += (bodies[i].m * bodies[j].m) / tmpD / G;
        }
        totalEnergy += tmp;
    }

    return totalEnergy;
}

void printSimulationLog(vector<body>& bodies, double initialEnergy){
    double finalEnergy = systemEnergy(bodies);
    int count = int(bodies.size());

    cout.precision(numeric_limits<double>::digits10);

    cout
        << setfill(' ') << showpos << scientific
        << "\nSyteme de " << count << " corps, energie totale : "
        << "\nAu debut   : " << initialEnergy
        << "\nA la fin   : " << finalEnergy
        << "\nDifference : "
        << 100 * (initialEnergy - finalEnergy) / initialEnergy << " %"
        << noshowpos << endl;

}

void printTidalForces(vector<body>& bodies){
    int count = bodies.size();
    double tabRel[20][20], tabAbs[20][20];
    double tmp, sum;

    for(int i = 0; i < count; i++){
        sum = 0;
        for(int j = 0; j < count; j++){
            if( i!= j ) tmp = - tidalForce( bodies[i], bodies[j]) / bodies[i].m;
            else tmp = +0.0;

            tabAbs[i][j] = tmp;
            sum += tmp;
        }
        for(int j = 0; j < count; j++){
            if( i!= j ) tabRel[i][j] = 100 * tabAbs[i][j] / sum;
        }
    }

    cout
        << endl
        << "Provenance de l'effet de maree en pourcentage de l'effet total"
        << endl << endl;

    cout << setw(18) << "";
    for(int i = 0; i < count; i++){
        cout << setw(18) << left << bodies[i].nom;
    }
    cout << endl;

    cout.precision(1);
    cout << noshowpos;

    for(int i = 0; i < count; i++){
        cout << setw(18) << left << bodies[i].nom;
        for(int j = 0; j < count; j++){
            cout
                << left << setw(6) << scientific << tabAbs[j][i] << " :"
                << left << setw(5) << fixed << tabRel[j][i] << setw(4) << left << "%";
        }
        cout << endl;
    }

}
