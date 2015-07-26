#include "body.hpp"

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <string>

using namespace std;

body::~body() {
}

body::body() {
    nom = "";
    m = 0;
    mg = 0;

    rx = 0;
    ry = 0;
    rz = 0;

    vx = 0;
    vy = 0;
    vz = 0;

    ax = 0;
    ay = 0;
    az = 0;
}

body::body(Json::Value JsonBody) {
    this->nom = JsonBody["_name"].asString();

    this->m = JsonBody["m"].asDouble();
    this->mg = m * G;

    this->rx = JsonBody["rx"].asDouble();
    this->ry = JsonBody["ry"].asDouble();
    this->rz = JsonBody["rz"].asDouble();

    this->vx = JsonBody["vx"].asDouble();
    this->vy = JsonBody["vy"].asDouble();
    this->vz = JsonBody["vz"].asDouble();
}

body::body(vector<string> csv) {
    string::size_type sz;
    this->nom = csv[0];
    this->m = stod(csv[1]);
    this->mg = m * G;

    this->rx = stod(csv[2]);
    this->ry = stod(csv[3]);
    this->rz = stod(csv[4]);

    this->vx = stod(csv[5]);
    this->vy = stod(csv[6]);
    this->vz = stod(csv[7]);
}

Json::Value body::toJson() {
    Json::Value root;
    root["_name"] = this->nom;
    root["m"] = this->m;
    root["rx"] = this->rx;
    root["ry"] = this->ry;
    root["rz"] = this->rz;
    root["vx"] = this->vx;
    root["vy"] = this->vy;
    root["vz"] = this->vz;

    return root;
}

Json::Value body::toJsonLight() {
    Json::Value root;
    root["id"] = this->id;
    root["rx"] = this->rx;
    root["ry"] = this->ry;
    root["rz"] = this->rz;

    return root;
}

void body::actualise() {
    ax /= mg;
    ay /= mg;
    az /= mg;
    // Calcul de la nouvelle position
    rx += 0.5 * ax * interval_p2 + vx * interval;
    ry += 0.5 * ay * interval_p2 + vy * interval;
    rz += 0.5 * az * interval_p2 + vz * interval;
    // Calcul de la nouvelle vitesse
    vx += ax * interval;
    vy += ay * interval;
    vz += az * interval;
}
