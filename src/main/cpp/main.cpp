#include <stdio.h>
#include <cstdio>
#include <math.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <ctime>

#include "../../../lib/json/json/json.h"

#include "FrameWriter.hpp"
#include "BufferedBinaryWriter.hpp"
#include "JsonWriter.hpp"

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
    printSimulationLog();
    int maxFrames = 365;
    int frameResolution = 60 * 60 * 24;
    bool useJson = false;
    string jsonFrameFile = "data/frames.json";
    string binFilename = "data/frames.bin";
    string outBinFile = "data/out.bin";

    FrameWriter *frameWriter = NULL ;
    BufferedBinaryWriter binWriter = BufferedBinaryWriter(outBinFile, 10);
    JsonWriter jsonWriter = JsonWriter(jsonFrameFile);

    vector <body> bodies = getBodies();

    if (useJson) {
        frameWriter = &jsonWriter;
    } else {
        frameWriter = &binWriter;
        remove(outBinFile.c_str());
    }

    int currentFrame = 0;

    while (currentFrame++ < maxFrames) {
        computeFrame(bodies, frameResolution);
        Frame frame = Frame(currentFrame, bodies);

        frameWriter->append(frame);
    }
    frameWriter->flush();

    Json::Value tmp = getSystemJson(bodies);
    printJson(tmp, "bodies", "data/out.json");

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

vector<body> getBodies() {
    vector <body> bodies;
    ifstream inFile("data/data.json");

    Json::Value root;
    Json::Reader reader;

    reader.parse(inFile, root);
    unsigned int count = root["bodies"].size();

    for (unsigned int i = 0; i < count; i++) {
        body b = body(root["bodies"][i]);
        b.id = int(i);
        bodies.push_back(b);
    }
    inFile.close();
    return bodies;
}

void accel(body& b1, body& b2) {
    double tmpX, tmpY, tmpZ, tmp;

    tmpX = b1.rx - b2.rx;
    tmpY = b1.ry - b2.ry;
    tmpZ = b1.rz - b2.rz;

    tmp = pow(tmpX, 2) + pow(tmpY, 2) + pow(tmpZ, 2);

    tmp = ((b1.m) * (b2.m)) / (tmp * sqrt(tmp));

    tmpX *= tmp;
    tmpY *= tmp;
    tmpZ *= tmp;

    b1.ax += tmpX;
    b1.ay += tmpY;
    b1.az += tmpZ;

    b2.ax -= tmpX;
    b2.ay -= tmpY;
    b2.az -= tmpZ;
}

void computeInterval(vector<body>& bodies) {
    int count = int(bodies.size());

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            accel(bodies[i], bodies[j]);
        }
    }

    for (int i = 0; i < count; i++) { // pour chaque astre
        bodies[i].ax *= G;
        bodies[i].ay *= G;
        bodies[i].az *= G;

        bodies[i].actualise();
        bodies[i].ax = 0;
        bodies[i].ay = 0;
        bodies[i].az = 0;
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
            << " x:" << bodies[i].rx
            << " y:" << bodies[i].ry
            << " z:" << bodies[i].rz
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
