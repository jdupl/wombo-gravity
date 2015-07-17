#include "BufferedBinaryWriter.hpp"

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

BufferedBinaryWriter::BufferedBinaryWriter() {
}

void BufferedBinaryWriter::append(Frame frame) {
    frameBuffer.push(frame);

    if (frameBuffer.size() > 4) {
        this->flush();
    }
}

void BufferedBinaryWriter::flush() {
    while (!frameBuffer.empty()) {
        cout << frameBuffer.front().frameNumber << "\n";
        this->frameBuffer.pop();
    }
}
