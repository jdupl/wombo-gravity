#include <stdio.h>
#include <cstdio>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>
#include <limits>

#include "../../../lib/json/json/json.h"

#include "FrameWriter.hpp"
#include "BufferedBinaryWriter.hpp"
#include "JsonWriter.hpp"

using namespace std;

void appendJsonFrame(vector<body>& , Json::Value& , unsigned int);
vector<body> getBodies();
void accel(body&, body&);
void computeInterval(vector<body>&);
void computeFrame(vector<body>&, int&);
void printSystem(vector<body>&);
void printSimulationLog(vector<body>&, double);
double systemEnergy(vector<body>&);
Json::Value getSystemJson(vector<body>&);
void printTime(double);
void printJson(Json::Value&, string, string);

int main(int argc, char** argv) {
    int maxFrames = 365;
    int frameResolution = 60 * 60 * 24;
    bool useJson = false;
    string jsonFrameFile = "data/frames.json";
    string binFilename = "data/frames.bin";
    string outBinFile = "data/out.bin";

    FrameWriter *frameWriter = NULL ;
    BufferedBinaryWriter binWriter = BufferedBinaryWriter(outBinFile, 10);
    JsonWriter jsonWriter = JsonWriter(jsonFrameFile);

    vector <body> bodies = getBodies();

    if (useJson) {
        frameWriter = &jsonWriter;
    } else {
        frameWriter = &binWriter;
        remove(outBinFile.c_str());
    }

    int currentFrame = 0;

    while (currentFrame++ < maxFrames) {
        computeFrame(bodies, frameResolution);
        Frame frame = Frame(currentFrame, bodies);

        frameWriter->append(frame);
    }
    frameWriter->flush();

    Json::Value tmp = getSystemJson(bodies);
    printJson(tmp, "bodies", "data/out.json");

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

vector<body> getBodies() {
    vector <body> bodies;
    ifstream inFile("data/data.json");

    Json::Value root;
    Json::Reader reader;

    reader.parse(inFile, root);
    unsigned int count = root["bodies"].size();

    for (unsigned int i = 0; i < count; i++) {
        body b = body(root["bodies"][i]);
        b.id = int(i);
        bodies.push_back(b);
    }
    inFile.close();
    return bodies;
}

void accel(body& b1, body& b2) {
    double tmpX, tmpY, tmpZ, tmp;

    tmpX = b1.rx - b2.rx;
    tmpY = b1.ry - b2.ry;
    tmpZ = b1.rz - b2.rz;

    tmp = pow(tmpX, 2) + pow(tmpY, 2) + pow(tmpZ, 2);

    tmp = ((b1.mg) * (b2.mg)) / (tmp * sqrt(tmp));

    tmpX *= tmp;
    tmpY *= tmp;
    tmpZ *= tmp;

    b1.ax += tmpX;
    b1.ay += tmpY;
    b1.az += tmpZ;

    b2.ax -= tmpX;
    b2.ay -= tmpY;
    b2.az -= tmpZ;
}

void computeInterval(vector<body>& bodies) {
    int count = int(bodies.size());

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            accel(bodies[i], bodies[j]);
        }
    }

    for (int i = 0; i < count; i++) { // pour chaque astre
        bodies[i].actualise();
        bodies[i].ax = 0;
        bodies[i].ay = 0;
        bodies[i].az = 0;
    }
}

void computeFrame(vector<body>& bodies, int& intervalCount) {
    clock_t begin = clock();
    int currentIteration = 0;

    while (currentIteration++ < intervalCount) {
        computeInterval(bodies);
    }

    cout << "frame took ";
    printTime(clock() - begin);
    cout << "\n";
}

void printSystem(vector<body>& bodies) {
    for (int i =0; i < int(bodies.size()); i++) {
        cout
            << bodies[i].nom
            << " x:" << bodies[i].rx
            << " y:" << bodies[i].ry
            << " z:" << bodies[i].rz
            << endl;
    }
}

Json::Value getSystemJson(vector<body>& bodies) {
    Json::Value bodies_node;

    for (unsigned int i = 0; i < bodies.size(); i++) {
        bodies_node.append(bodies[i].toJson());
    }
    return bodies_node;
}

void printTime(double t) {
    cout << setw(6) << left << setfill('0') << t / 1000 << " ms" << flush;
}

double systemEnergy(vector<body>& bodies) {
    int count = int(bodies.size());
    double totalEnergy = 0;
    double tmpX, tmpY, tmpZ, tmpD, tmp;

    for (int i = 0; i < count; i++) {
        tmp =
            pow(bodies[i].vx, 2) +
            pow(bodies[i].vy, 2) +
            pow(bodies[i].vz, 2);

        tmp *= bodies[i].mg * 0.5 * G_INV;

        for (int j = i + 1; j < count; j++) {

            tmpX = bodies[i].rx - bodies[j].rx;
            tmpY = bodies[i].ry - bodies[j].ry;
            tmpZ = bodies[i].rz - bodies[j].rz;
            tmpD = sqrt(pow(tmpX, 2) + pow(tmpY, 2) + pow(tmpZ, 2));
            tmp += (bodies[i].mg * bodies[j].mg) / tmpD * G_INV;
        }
        totalEnergy += tmp;
    }

    return totalEnergy;
}

void printSimulationLog(vector<body>& bodies, double initialEnergy) {
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
