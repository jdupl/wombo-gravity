#include "BufferedBinaryWriter.hpp"

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <memory>

using namespace std;

/**
 * Provides a fast and low-memory implementation of persistance for the frames.
 */
BufferedBinaryWriter::BufferedBinaryWriter(string filename, int frameBufferSize) {
    this->filename = filename;
    this->frameBufferSize = frameBufferSize;
}

void BufferedBinaryWriter::append(Frame frame) {
    frameBuffer.push(frame);

    if (frameBuffer.size() >= frameBufferSize) {
        this->flush();
    }
}

void BufferedBinaryWriter::flush() {
    // Header of the frame
    struct FrameData {
        int frameNumber;
        int bodyCount;
    };

    struct BodyData {
        int id;
        double rx;
        double ry;
        double rz;
    };

    FILE* binFile = fopen(filename.c_str(), "ab");

    while (!frameBuffer.empty()) {
        Frame frame = frameBuffer.front();
        this->frameBuffer.pop();

        FrameData header;
        header.frameNumber = frame.frameNumber;
        header.bodyCount = frame.bodies.size();

        fwrite(&header, sizeof(FrameData), 1, binFile);

        for (unsigned int i = 0; i < frame.bodies.size(); i++) {
            BodyData bodyData;
            bodyData.id = frame.bodies[i].id;
            bodyData.rx = frame.bodies[i].rx;
            bodyData.ry = frame.bodies[i].ry;
            bodyData.rz = frame.bodies[i].rz;
            fwrite(&bodyData, sizeof(BodyData), 1, binFile);
        }
    }
    fclose(binFile);
}
