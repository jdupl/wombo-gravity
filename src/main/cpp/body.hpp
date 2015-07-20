#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

#ifndef _BODY_
#define _BODY_

#include "../../../lib/json/json/json.h"

#define G (-6.6738480e-11)

const unsigned int interval = 1;
const unsigned int interval_p2 = interval * interval;

using namespace std;

class body {
    friend istream& operator >> (istream&, const body&);
    friend ostream& operator << (ostream&, const body&);

    public:
        int id;
        //masse
        double m;

        // position
        double rx;
        double ry;
        double rz;

        //vitesse
        double vx;
        double vy;
        double vz;

        //acceleration
        double ax;
        double ay;
        double az;

        string nom; //nom

        ~body();
        body();
        body(Json::Value);
        // body(const body&);
        body(vector<string>);

        void actualise();

        Json::Value toJson();
        Json::Value toJsonLight();
};

#endif
