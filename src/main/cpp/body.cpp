#include "body.hpp"

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <string>
#include <limits>

using namespace std;

vect& vect::operator = (const vect& w){
    this->x = w.x;
    this->y = w.y;
    this->z = w.z;

    return (*this);
}

vect::~vect() {
}

vect::vect(double xi, double yi, double zi) {
    x = xi;
    y = yi;
    z = zi;
}

vect::vect(const vect& v) {
    x = v.x;
    y = v.y;
    z = v.z;
}

void vect::reset() {
    x = 0;
    y = 0;
    z = 0;
}

void vect::mult(const double d) {
    x *= d;
    y *= d;
    z *= d;
}

gravAcc::~gravAcc(){
}

gravAcc::gravAcc(){
    limit = 0;
    timer = 0;

    a.x = 0;
    a.y = 0;
    a.z = 0;
}

gravAcc::gravAcc(const gravAcc& w){
    this-> limit = w.limit;
    this-> timer = w.timer;

    this-> a.x = w.a.x;
    this-> a.y = w.a.y;
    this-> a.z = w.a.z;
}

body::~body() {
}


body::body() {
    nom = "";
    m = 0;

    r.x = 0;
    r.y = 0;
    r.z = 0;

    v.x = 0;
    v.y = 0;
    v.z = 0;

    a.x = 0;
    a.y = 0;
    a.z = 0;
}

body::body(Json::Value JsonBody) {
    this->nom = JsonBody["_name"].asString();
    this->m = JsonBody["m"].asDouble();

    this->r.x = JsonBody["rx"].asDouble();
    this->r.y = JsonBody["ry"].asDouble();
    this->r.z = JsonBody["rz"].asDouble();

    this->v.x = JsonBody["vx"].asDouble();
    this->v.y = JsonBody["vy"].asDouble();
    this->v.z = JsonBody["vz"].asDouble();
}

body::body(const body& b) {

    this-> nom	   = b.nom;    //name
    this-> m	   = b.m;      //mass

    this-> r	   = b.r;      //position
    this-> v	   = b.v;      //speed
    this-> a	   = b.a;	   //acceleration
}

Json::Value body::toJson() const {
    Json::Value root;
    root["_name"] = this->nom;
    root["m"] = this->m;
    root["rx"] = (double) this->r.x;
    root["ry"] = (double) this->r.y;
    root["rz"] = (double) this->r.z;
    root["vx"] = (double) this->v.x;
    root["vy"] = (double) this->v.y;
    root["vz"] = (double) this->v.z;

    return root;
}

Json::Value body::toJsonLight() const {
    Json::Value root;
    root["name"] = this->nom;
    root["rx"] = (double) this->r.x;
    root["ry"] = (double) this->r.y;
    root["rz"] = (double) this->r.z;

    return root;
}

void body::actualise() {

    const unsigned int count = memo.size();
    bool isUpToDate = true;
    timeStep += DT;

    for(unsigned int i = 0; i < count; i++){
        if( memo[i].limit == 0 ) continue;
        if( memo[i].timer++ != memo[i].limit) continue;
        isUpToDate = false;
        memo[i].timer = 0;
    }

    if( isUpToDate ) return;

    a.reset();

    for(unsigned int i = 0; i < count; i++){
        a.x += memo[i].a.x;
        a.y += memo[i].a.y;
        a.z += memo[i].a.z;
    }

    a.mult(1/m);

    const double t_2 = timeStep * timeStep * 0.5;
    // Actualize position
    r.x += a.x * t_2 + v.x * timeStep;
    r.y += a.y * t_2 + v.y * timeStep;
    r.z += a.z * t_2 + v.z * timeStep;
    // Actualize speed
    v.x += a.x * timeStep;
    v.y += a.y * timeStep;
    v.z += a.z * timeStep;
    // Actualize time
    timeStep = 0;
}
