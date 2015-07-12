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

//#define G (-6.6738480 * pow(10, -11))
#define G (-6.6738480e-11)
#define SECOND_PER_DAY (3600 * 24)
#define SECOND_PER_YEAR (31556926)
#define DAYS_PER_YEAR (365.242190517)
#define YEARS_PER_DAY (0.002737909)

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
