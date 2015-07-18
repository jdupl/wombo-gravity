#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>

#ifndef _BODY_AND_VECT_
#define _BODY_AND_VECT_

#include "../../../lib/json/json/json.h"

#define G (-6.6738480e-11)

const unsigned int interval = 1;
const unsigned int interval_p2 = interval * interval;

using namespace std;

class vect {

    public:
        double x;
        double y;
        double z;

        vect& operator = (const vect&);

        ~vect();
        vect(double xi=0, double yi=0, double zi=0);
        vect(const vect&);

        void reset();
        void mult(double);
};

class body {
    friend istream& operator >> (istream&, const body&);
    friend ostream& operator << (ostream&, const body&);

    public:
        int id;
        double m;	//masse
        vect r;		//position
        vect v;		//vitesse
        vect a;		//acceleration
        string nom; //nom

        ~body();
        body();
        body(Json::Value);
        body(std::vector<string>);
        body(const body&);

        void actualise();

        Json::Value toJson();
        Json::Value toJsonLight();
};

#endif
