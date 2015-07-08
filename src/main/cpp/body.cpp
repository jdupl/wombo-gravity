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

vect& vect::operator =	(const vect& w){
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

void vect::mult(double d) {
    x *= d;
    y *= d;
    z *= d;
}

body::~body() {
}

body::body(string str, double masse, vect position, vect vitesse, vect acc) {
    nom = str;
    m = masse;
    r = position;
    v = vitesse;
    a = acc;
}

body::body(vector<string> csv) {
    string::size_type sz;
    this->nom = csv[0];
    this->m = stod(csv[1]);
    this->r = vect(stod(csv[2]), stod(csv[3]), stod(csv[4]));
    this->v = vect(stod(csv[5]), stod(csv[6]), stod(csv[7]));
}

body::body(const body& b) {
    this-> m	= b.m;	//masse
    this-> r	= b.r;	//position
    this-> v	= b.v;		//vitesse
    this-> a	= b.a;	//acceleration
    this->nom	= b.nom;	//nom
}

void body::actualise() {
    // Calcul de la nouvelle position
    r.x += 0.5 * a.x * interval_p2 + v.x * interval;
    r.y += 0.5 * a.y * interval_p2 + v.y * interval;
    r.z += 0.5 * a.z * interval_p2 + v.z * interval;
    // Calcul de la nouvelle vitesse
    v.x += a.x * interval;
    v.y += a.y * interval;
    v.z += a.z * interval;
}