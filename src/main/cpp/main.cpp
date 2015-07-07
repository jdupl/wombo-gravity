#include <stdio.h>
#include <cstdio>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <ctime>

#include "body.hpp"

using namespace std;

void accel(body& b1, body& b2);

//int main(int argc, char** argv) {
//    return 0;
//}

void accel(body& b1, body& b2) {

    double tempx, tempy, tempz, tempo;

    tempx = b1.r.x - b2.r.x;
    tempy = b1.r.y - b2.r.y;
    tempz = b1.r.z - b2.r.z;

    tempo = (b2.m)/(pow(
                        pow((tempx), 2) +
                        pow((tempy), 2) +
                        pow((tempz), 2),
                    1.50));

    b1.a.x += (tempo) * (tempx);
    b1.a.y += (tempo) * (tempy);
    b1.a.z += (tempo) * (tempz);
}
