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

void appendJsonFrame(vector<body>& , Json::Value& , unsigned int);
vector<body> getBodies();
void accel(body&, body&);
void computeInterval(vector<body>&);
void computeFrame(vector<body>&, int&);
void printSystem(vector<body>&);
void printSimulationLog();
Json::Value getSystemJson(vector<body>&);
void printTime(double);
void printJson(Json::Value&, string, string);

int main(int argc, char** argv) {
    int maxFrames = 365;
    int frameResolution = 60 * 60 * 24;
    Json::Value jsonFrameBuffer;
    string jsonOutputFile = "data/out.json";
    string jsonFrameFile = "data/frames.json";

    printSimulationLog();
    vector <body> bodies = getBodies();

    int currentFrame = 0;

    while (currentFrame++ < maxFrames) {
        computeFrame(bodies, frameResolution);
        appendJsonFrame(bodies, jsonFrameBuffer, currentFrame);
    }

    remove(jsonOutputFile.c_str());
    printJson(jsonFrameBuffer, "frames", jsonFrameFile);

    Json::Value tmp = getSystemJson(bodies);
    printJson(tmp, "bodies", jsonOutputFile);

    return 0;
}

void printJson(Json::Value& json, string rootName, string filename) {
    Json::Value root;
    root[rootName] = json;
    ofstream jsonOut;

    jsonOut.open(filename.c_str());
    jsonOut << root;
    jsonOut.close();
}

void appendJsonFrame(vector<body>& bodies, Json::Value& json, unsigned int frameNumber) {
    Json::Value bodiesJson;

    for (unsigned int i = 0; i < bodies.size(); i++) {
        bodiesJson.append(bodies[i].toJsonLight());
    }

    Json::Value currentFrame;
    currentFrame["frameNumber"] = frameNumber;
    currentFrame["bodies"] = bodiesJson;
    json.append(currentFrame);
}

vector<body> getBodies() {
    vector <body> bodies;
    ifstream inFile("data/data.json");

    Json::Value root;
    Json::Reader reader;

    reader.parse( inFile, root);
    unsigned int count = root["bodies"].size();

    for(unsigned int i = 0; i < count; i++){
        bodies.push_back(body(root["bodies"][i]));
    }
    inFile.close();
    return bodies;
}

void accel(body& b1, body& b2) {
    double tmpX, tmpY, tmpZ, tmp;

    tmpX = b1.r.x - b2.r.x;
    tmpY = b1.r.y - b2.r.y;
    tmpZ = b1.r.z - b2.r.z;

    tmp = pow(tmpX, 2) + pow(tmpY, 2) + pow(tmpZ, 2);

    tmp = ((b1.m) * (b2.m)) / (tmp * sqrt(tmp));

    tmpX *= tmp;
    tmpY *= tmp;
    tmpZ *= tmp;

    b1.a.x += tmpX;
    b1.a.y += tmpY;
    b1.a.z += tmpZ;

    b2.a.x -= tmpX;
    b2.a.y -= tmpY;
    b2.a.z -= tmpZ;
}

void computeInterval(vector<body>& bodies) {
    int count = int(bodies.size());

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            accel(bodies[i], bodies[j]);
        }
    }

    for (int i = 0; i < count; i++) { // pour chaque astre
        bodies[i].a.mult(G);
        bodies[i].actualise();
        bodies[i].a.reset();
    }
}

void computeFrame(vector<body>& bodies, int& intervalCount) {
    clock_t begin = clock();
    int currentIteration = 0;

    while (currentIteration++ < intervalCount) {
        computeInterval(bodies);
    }

    cout << "frame took ";
    printTime(clock() - begin);
    cout << "\n";
}

void printSystem(vector<body>& bodies) {
    for (int i =0; i < int(bodies.size()); i++) {
        cout
            << bodies[i].nom
            << " x:" << bodies[i].r.x
            << " y:" << bodies[i].r.y
            << " z:" << bodies[i].r.z
            << endl;
    }
}

Json::Value getSystemJson(vector<body>& bodies) {
    Json::Value bodies_node;

    for (unsigned int i = 0; i < bodies.size(); i++) {
        bodies_node.append(bodies[i].toJson());
    }
    return bodies_node;
}

void printTime(double t) {
    cout << setw(6) << left << setfill('0') << t / 1000 << " ms" << flush;
}

void printSimulationLog() {
	cout
    << "            . . . . .   .   . . . .         . .   . . .   . . . .   .   . . . . \n"
    << "   . .         . . .     . .   .  .7vq   .  :Uii .   . .   . .   .              \n"
    << ".         .     . .   . .   . .Y@O@8@2r     @EG8@ZX     .   .     .       . . . \n"
    << " . .     .         .     .   . iOY .       .OG . @Mr . . @5.       . . . . .   .\n"
    << ".         . . . . . iP@M@O@     @E.,U2M .  i@F. 7M@   .  :8Z@Nv . .     . . . . \n"
    << " . .       . . .   @8G . :B@   .E@O@N87. . @ZOM@Gr . .    r@k5G@   .     .   .  \n"
    << ".     .   .   . . . @M.   @B.   7Zr .   . .E@ .v@   .    :@X  .   .       .     \n"
    << " . . .   .   uB@ . . @E..@ML   . @E. :i5  :@2   M@ . .  ,@G  . . .     . . . . .\n"
    << "        . .   @Z@5  ..@M@L. . .  B@Z@M@M: iFi   kEu : . @G. .     . Z8@j  .     \n"
    << "   . .   qui .:@ @BU .i@i  . .   .     . .     . .  B@G@G. .     qM@qMB@   .   .\n"
    << ". .   . .M@G@F:8N vO@  v@v    .         .   . . .   . uN@G5   5O@8L  M@   . .   \n"
    << "   .     .v@Frk@8r  q@X LL . . .             . .       . v:rE@M@B7 .O@   . .    \n"
    << "  .  O2 . ..@q  i   ..q .           .   .     .       . . . 7.  LZ@E@r. .   18i \n"
    << "   rG@   . . @Oi     .     .     . . .     .   .     . .     . .  O@X.   .U@B@: \n"
    << "  MO@MO . .   jO@ .   .   . .         . .     .     . .       . . @M    BZ@L.   \n"
    << " iMr iO@Mi     :F: .   . . .   .     .     . .     .       . .   .7: :O@8J   . .\n"
    << "  . . ..@B@..Z@   .           . . . .   .             . . . . .   . MM@ . . .   \n"
    << " .     . .v@E@     .     .     .     . .     .       . . .         . @O.       .\n"
    << ".       . LM@ . .       .         .     .   .     .   .   .   .   .   FZ8 .   . \n"
    << " .   .   .55     . .     . .       . .     . .   . . .   .   .   .   . r8,     .\n"
    << "    . . . . . .         .   .         .     .   . .       . . .   . . .   .   . \n"
    << "   .       . . .           .       . .       .       .         .         . . .  \n"
    << "      . .       .  i@   uJ         7@UOJ,   ZO@1  :GBFO:  @i. . @:  . .   . .   \n"
    << " .         . .   . @G@  0@     . . @G  @G. @Gi MO :@:::@Z r@;  @8,   . .   .   .\n"
    << "  .     .         .q@1@ qB  . . .  O@,vOO FO  . @YiB  . @. :@ @O  . .   . . . . \n"
    << "     . . .         MB BEJ@  S@Z@L. @Mr:GG j@ .  Bk:@    Mr  :@M. .   .   . .    \n"
    << ".   . . .     .    Z@  M@G  .   .  G@ ..@, Mq  v@ LM, :G@   .O1 . .   . . .     \n"
    << " .   . . .   .     SP.  GO . . .   PMMqB:. :E@qB...@ZBGF   . @,    . .   .   .  \n"
    << ".     . . .     . .     .     .   . . . . .     .     . . . . . . . . .         \n"
    << " . . . rX@O@.. @0@P@, U@   @; O7 . @: .@ . , , .OB   qEOOBMM  :@E@i  vBMEq   . .\n"
    << "      iMX ...   iB5 . @Eq @M@ @u. .G0 7M:   .  O@8X   :.@2, .X@:..@G.2@ .M@     \n"
    << " . . . @MX:  .   @:.  8LB@P2O.B2   @r.:@      ,@ rB     M7 . @i.  i@ uMr:MF    .\n"
    << ".   . .  :BE1    Mi   @ rO.:@ @:  .BX iM. .   @E, @k. . @7.  Mu . u8 v@:@B    . \n"
    << " . . . , . @B  . @L  ,O:   u8 E@ .,@: :@.  . 0OPuGq@.  .M1   @O, iGO 0B. @M     \n"
    << ". . . vZ@M@F. ,E@8@Ei @ .  .@ .q@8@i  :E@Z@P qF .  O0   @r  . PO@MF .i@   @i. . \n"
    << " . . . .       . . .     . .       . . . .     . . . .   . . . .   .   .        \n"
    << ". . .   . .   . . . .     .   .       . . . .       .   .         .   . . .     \n"
    << " . . .   .     . .   . .     . . .   . .         . .         .   . .   . .     .\n"
    << ". .   . .     .           .         .   . .     . .     .   .     . .     . . . \n"
    << "   .   . . . . .           .   . .   . .       . .   .   .     . . .     .      \n"
    << "              . . . . . . . .       .   .   .   . .     . .     . .     . .   . \n"
    << " .   . . . .   .       . .     .   .     .   . .   .       .     . . .     . .  \n"
    << "  .   .     .       .     . .     .       .     . . .       .   .   . .     .   \n"
    << " .   .   . . .   .     .:@B@i  . . . . .   . . . iu@7.     .           .   .    \n"
    << "  . .       . . .     .  :;i@E.     . . .       @MBj@ZB .   .     .     . . .   \n"
    << "         .       . .   . .  ,@7 7@M@X:   OG@Gv BB. ..@G@         . .     . . .  \n"
    << ".             .   .   .   .r@E.M@r, ME:.@Br UBO1@ . @u.B@   .   . . . .         \n"
    << "       .   . .   .   :r8Z@O@i F@    .@M@M.  .@OFZ@ @M  :B0 . . .     . .   .   .\n"
    << "    .   . .     . .u@M@7: .   @N:7@G@Z@M@  G@,@B.M@M  . @S  .   . .     .   .   \n"
    << " . .   .     . . . ..XZ@J.   ;M@8Mi  @:OEEO@ .L@  E@MSk@G. .                    \n"
    << "    .   .   . . .     .,@GO   @Z. . @B. @M.   @E. . EE0:    . .                 \n"
    << " . .     .   .                i@M25@E:  :@ZUr@8L . . .     . .     .       . .  \n"
    << "  .   .   .     .   . .   .   . :rv     . i75,.   . .   .       .       .   . . \n"
    << endl;
}
