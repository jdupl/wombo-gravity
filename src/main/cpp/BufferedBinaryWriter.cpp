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
    FILE* binFile = fopen("data/bin", "ab");

    while (!frameBuffer.empty()) {
        Frame frame = frameBuffer.front();
        this->frameBuffer.pop();
        os.write(reinterpret_cast<char const *>(std::addressof(obj)), sizeof(Type));
        fwrite(frame, 1, sizeof(Frame), binFile);
    }
    fclose(binFile);
}
