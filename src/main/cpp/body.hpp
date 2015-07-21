#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <vector>
#include <limits>

#ifndef _BODY_AND_VECT_
#define _BODY_AND_VECT_

#include "../../../lib/json/json/json.h"

#define G (-6.6738480e-11)
#define G_INV (-1.49838594e10)
#define SECOND_PER_DAY (3600 * 24)
#define SECOND_PER_YEAR (31556926)
#define DAYS_PER_YEAR (365.242190517)
#define YEARS_PER_DAY (0.002737909)
#define DT (1)

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
        void mult(const double);
};

class gravAcc{
    public:
        vect a;
        unsigned int limit, timer;

        ~gravAcc();
        gravAcc();
        gravAcc(const gravAcc&);
};

class body {
    friend istream& operator >> (istream&, const body&);
    friend ostream& operator << (ostream&, const body&);

    public:
        string nom; //nom
        double m;	//masse*G
        vect r;		//position
        vect v;		//vitesse
        vect a;		//acceleration

        //for variable time steps
        vector<gravAcc> memo;
        double timeStep;

        ~body();
        body();
        body(Json::Value);
        body(const body&);

        void actualise();

        Json::Value toJson() const;
        Json::Value toJsonLight() const;
};

#endif
