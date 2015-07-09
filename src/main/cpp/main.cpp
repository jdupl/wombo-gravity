#include <stdio.h>
#include <cstdio>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>

#include "../../../lib/json/json/json.h"

#include "body.hpp"

using namespace std;

vector<body> getBodies();
void accel(body&, body&);
void computeInterval(vector<body>&);
void computeFrame(vector<body>&, int&);
void printSystem(vector<body>&);
void printSimulationLog();
void printSystemJson(vector<body>&, string);

int main(int argc, char** argv) {
    int maxFrames = 365;
    int frameResolution = 60 * 60 * 24;

    string jsonOutputFile = "data/out.json";

    printSimulationLog();
    vector <body> bodies = getBodies();

    int currentFrame = 0;

    while (currentFrame++ < maxFrames) {
        computeFrame(bodies, frameResolution);
    }

    remove(jsonOutputFile.c_str());
    printSystemJson(bodies, jsonOutputFile);
}

vector<body> getBodies() {
    vector <body> bodies;
    ifstream infile("data/data.csv");

    while (infile) {
        string s;
        if (!getline(infile, s)) break;

        istringstream ss(s);
        vector <string> record;

        while (ss) {
            if (!getline(ss, s, ',')) break;
            record.push_back(s);
        }

        bodies.push_back(body(record));
    }
    if (!infile.eof()) {
        cerr << "Error reading file!\n";
    }
    return bodies;
}

void accel(body& b1, body& b2) {
    double tempx, tempy, tempz, tempo;

    tempx = b1.r.x - b2.r.x;
    tempy = b1.r.y - b2.r.y;
    tempz = b1.r.z - b2.r.z;

    double tmp = pow(tempx, 2) + pow(tempy, 2) + pow(tempz, 2);

    tempo = (b2.m) / (tmp * sqrt(tmp));

    b1.a.x += (tempo) * (tempx);
    b1.a.y += (tempo) * (tempy);
    b1.a.z += (tempo) * (tempz);
}

void computeInterval(vector<body>& bodies) {
    int count = int(bodies.size());

    for (int i = 0; i < count; i++) { // pour chaque astre
        // Calcul de l'acceleration
        bodies[i].a.reset();

        for (int j = 0; j < count; j++) { // Pour l'interaction avec chaque autre astre
            if (i != j) accel(bodies[i], bodies[j]);
        }

        bodies[i].a.mult(G);
    }

    for (int i = 0; i < count; i++) { // pour chaque astre
        bodies[i].actualise();
    }
}

void computeFrame(vector<body>& bodies, int& intervalCount) {
    clock_t begin = clock();
    int currentIteration = 0;

    while (currentIteration++ < intervalCount) {
        computeInterval(bodies);
    }
    clock_t end = clock();

    cout << "frame took " << double(end - begin) / 1000 << " ms\n";
}

void printSystem(vector<body>& bodies) {
    for (int i =0; i < int(bodies.size()); i++) {
        cout << bodies[i].nom << " x:" << bodies[i].r.x << " y:" << bodies[i].r.y << " z:" << bodies[i].r.z<< "\n";
    }
}

void printSystemJson(vector<body>& bodies, string filename) {
    clock_t begin = clock();
    Json::Value json;
    Json::Value bodies_node;

    for (unsigned int i = 0; i < bodies.size(); i++) {
        bodies_node.append(bodies[i].toJson());
    }
    json["bodies"] = bodies_node;
    clock_t end = clock();
    cout << "json took " << double(end - begin) / 1000 << " ms\n";

    begin = clock();
    ofstream jsonOut;
    cout << filename.c_str();
    jsonOut.open(filename.c_str());
    jsonOut << json;
    jsonOut.close();

    end = clock();
    cout << "disk took " << double(end - begin) / 1000 << " ms\n";
}

void printSimulationLog() {
	cout
	<< "  .                   .     .     .   .     . . .       . . .   .   . .         .   .   .       .   \n"
	<< "   . . . .   .   . .         . . .     .     .r. . .   :i:   .   .   .   .               .   . .    \n"
	<< ". . . .   . .           .     .     .  Z@@@B@W@W  .   .M@M@B@M2   . .   .   .           .     . . . \n"
	<< "             .   .   . . .           . @B@22::     . . @W7 i2@8@         .         . .     .   . . .\n"
	<< "  .                   .      r2:. .    2@B.           XB@   .7@B.      M@ZS   . .       .           \n"
	<< "     .       .     .   . 7B@M@M@MB   . .M@;:r88@a    .M@@2.:7@@@       .2@B@M@.          .          \n"
	<< ".   . . .   . .        2@8@02. i@M@     @8@B@M@8@ .   @B@Z@B@Z2 .        B@M@W@M.             .     \n"
	<< " .   . . . . .   .   .  M@8      @B:    X@M.       . 7B@. 7@W. . .     .B@8. .::         .   .   .  \n"
	<< ".       .        :M . .  B@M    MM@   . 7@@   .      X@8  .S@@  .      M@Mi       . .   . . .       \n"
	<< " .   . . . .    2@W@2    .8@S7M@M@     . @MS.2S@B@   @MB .  B@7       X@87     .   . . :     . .   .\n"
	<< ".   . . . .      W@M@M7   .Z@M@Z7   .   .8@8@8@M@8i  0@7    @W@  .   2@Z2 . . .     7M@M@2  . . . . \n"
	<< "         . .S;    M@:@M@ . :M@7.   . .    r7   .     . .   . r2. @M@X@82       . i8@M@X@8@          \n"
	<< ".   .     :W@8@B7 7B2 M@@S  78@7.     . .   .         .   . .     88@B@W7     :X@M@M7 MM@     . . . \n"
	<< " . .   .   7W@M@8@M@8: :W@M2 0@@..             .       .             ;0@8B :M@M@B7   2M@:. . . .    \n"
	<< "  .   .   .  B@B:i@M@M  .2@W@ r:    . . .         . . .   .     .   .   . @M@M0M@Wi .M@2            \n"
	<< "      B@.  .  7@M7 .ii .   B2  .   . .         .         .     .     .     7..  7@M@Z@0.       8M@  \n"
	<< "  . 7B@2.       @M@             .   .     . . .           . .   .                 7M@M. . . .0@8@M. \n"
	<< "   MM@W2     .   B8@2        . .         .           .   .       .   . .         .2@W7   ..@M@MX .  \n"
	<< ". @M@S@M@W. .   . .M@Z          .       .   .           .   .   .           . .   @MX   aM@M@7    . \n"
	<< " .M@   SM@@@.    2 .r. . .     .     . .               . . . .     .               7  S@M@0r . .    \n"
	<< "  .   . ..@M@M2 @@@ .     . . .   .           .           . . . .       . . . .      8@B2         . \n"
	<< "         . .S@M@M@   . .   .       .   .   . .   .   .   . .     .               .   .M@0    . .   .\n"
	<< "          .  7@MZ .           .     .       . .   .       .   .   .           .        X@M. . .     \n"
	<< " .       .  8@87     .               .           . . .   .     .   .       .   .       .7@@7        \n"
	<< "        .   :2.     .       .           . .           .     .     . . . . . .   . .      .M..   . . \n"
	<< " .     .     . . .         .               .                   . . .       .   .   .   .         .  \n"
	<< ". .       .         .     .     .               . .                       .     . .       .     .   \n"
	<< "     .         .   .   .:2   ..X           .i22Xr  . . .7M2    77X22 .  2X   .  S7 . .   .          \n"
	<< "  . .   .         .     @M@   @Mr     .   . @M@8@M@   @8@B@M@  8@W@M@Zi MM@ .  Z@X        .     .   \n"
	<< "   . . .     .     .   .B@M@ .M@:        .  M@   @M2 @MW . MM8 @W.   @M: WZ@  M@S.           . .    \n"
	<< "  .               .     @87M@ M8i .     . . @Z0a@MM .M@ . . @8 M@ .  7@8  B80a@X  .       .   .     \n"
	<< "               .        8@ Z872@:  @8@M@0   M@S@M@M: @B     8@ @8    7BM   88@2  . .         .   . .\n"
	<< "    .       .       .   @8  @Z@Zr ..27S2r . @W   .@B B@.  .7@8.M@    Z@7. . @M          .       . . \n"
	<< "           .     .   .  8@   @W@i. . .     .B@XWZ@82 :8@MM8@8. @M@M@B@2     M@   .   .   . . . .    \n"
	<< "            . .   .     02.   W2.           S28W87     2@MM:   2XZMX7     . Z2    .       . .       \n"
	<< "   .   .   .   .       .     . .       .   . .   . . .     .         .     . . .       .           .\n"
	<< "    .    i@M@B@.  BB@W@W@i r@Z. .2@2 X@   .r@2  @B.   :.i  :@@r . MM@M@B@Z@   WM@WM   ;8@8@W7       \n"
	<< "     .  M@B228Zr  r8S@M0Xr M8@2. @B@ @82   X8@  @@.        @M@M.  :Z2BB@282. @8B2WM@i 7@W22@MB      \n"
	<< "    .   @M0 .     .  M@    0@M@ @M@W.W@r   7@0. @8:   .   XM72@7.     @8. . @@i .  M@ 2B2 ..@M.     \n"
	<< "       . MM@8@.      @W  . @W B@8.W@ BB7   S8M .@@.. .    B@  8@     :M@   .W@     @8 :@M@W@2r .    \n"
	<< "  .         SM@M.   .M@    M@ 2MW @M M@.   :@Z  @B.      8@Z2r@BM   .:@M. . @W  . .M@ 7MB:@Ba .     \n"
	<< "     . .     .8@..   @M. . @M.   .B@ 8BX . @Ma .8@   . . @8@B@B@Mi   iB@ .  W@X  .M@0.r@2  @80   .  \n"
	<< "      . @M@M@B@2  @M@M@B@7.M@ .   @82 @Z@W@M@ . @M@Z@M@ @W2   .7@8   i@8  .  B@B@B@0  2@@   @B2 .   \n"
	<< " .       77X2i     2r::27:  .      : . .207. .   ii772r  :   .   7 .  .i   .   277 . . :..   r.    .\n"
	<< ".     .   .   . .         . . .   . .   . . .   .   .     . .   . .   .   .           . .         . \n"
	<< "     .         .     . . . . .   .   . . .           .       .   .     . .     .   .       .       .\n"
	<< "        .     .     .       . .     .         .   . .   .     . . . . . .         .               . \n"
	<< " .     .       .           .                       . . . .   .       .     .     . .       . .      \n"
	<< "        .                       .       . . .   .             .   . . .     .       .     . .       \n"
	<< "     .   .   .   .       .     . . .     . . . . .     . .       .               . . .     . . .   .\n"
	<< "    .   .     . .                 .             . .   . . .               .       .           .     \n"
	<< "             .   .   . . .     . .   . .         .       .   . .   . .   .                 .        \n"
	<< ". .           .       . . .   .   .     .               .                   .   .   .     . .   .   \n"
	<< "         . .       . .       . W2a       .     . .   . .     . .       . .     .       . .       .  \n"
	<< ".       . .     .   .         XB@M@MX     . . . .   .   .   iM@@@W@..   .                         . \n"
	<< "   .     .   .       .       . .  .@Z@ . .             .   Z8@0a7MB@B      .         . . .   .      \n"
	<< "        .   .   .                  i@8  XM@M@W7    X@B@8X  M@:.  i@8@M          .       . .     . . \n"
	<< "           . .     .             .7@M0 @M@00a@B@ 2M@ZWX@M@:@87   @MrW@B    . . .   . .   .     .    \n"
	<< "  .     .                   . 2X@M@BS @WM     @M7M@7  . @W@S@B  @Za  B@M  .   .                 .   \n"
	<< " . .   . .   . .   .   .2@8@B@M@B@7  XM@   7S@M@@@@. .i@8@BXM@S@M@   78@ . .             .   . .    \n"
	<< ".   . .     .       .   @8@BM   .   .M@2XM@@@S@WMZ@  M@M77@B 8@B@ .  X@8            . . .     .     \n"
	<< " .   .         .   .     r8@8@X:     @@@M@Sr  8@:@8ZM@0   B@  Z@@@Z@8@M7 .     .   .                \n"
	<< "      .   .       . .   .   7@@M@.  .B@2     8@0 X@B@.  ..@8i   @M@B@2  . .   .   .   .   . .   . . \n"
	<< "       . .             .     . XZ:   MM@r. iM@@i .M@8.  .@M@   .           .     .         . . .    \n"
	<< "  .       .   . .       .           . 0M@B@Z@8.    M@@@W@MZ .       .           .           .   .   \n"
	<< "                       . .         .   . .:. . .     :i2   .           .   . . .                    \n"
	<< endl;
}
