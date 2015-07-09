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

#define G (-6.6738480 * pow(10, -11))
#define interval (1)
#define interval_p2 (interval*interval)
#define JOURNEE (3600 * 24)
#define ANNEE (3600 * 24 * 365.242190517)
#define SAVE_RES (JOURNEE * 1)

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
        body(string str="-", double masse=0, vect position=0, vect vitesse=0, vect acc=0);
        body(std::vector<string> csv);
        body(const body&);

        void actualise();

        Json::Value toJson();
};

#endif
