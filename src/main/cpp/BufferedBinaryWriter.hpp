#ifndef FRAME_WRITER_H
#define FRAME_WRITER_H
#include "FrameWriter.hpp"
#endif

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

class BufferedBinaryWriter : public FrameWriter {

    public:
        BufferedBinaryWriter();
        virtual void append(Frame);
        virtual void flush();

    private:
        queue<Frame> frameBuffer;
};
