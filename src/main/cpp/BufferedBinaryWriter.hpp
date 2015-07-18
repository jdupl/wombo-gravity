#include "FrameWriter.hpp"

#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

class BufferedBinaryWriter : public FrameWriter {

    public:
        BufferedBinaryWriter(string , int frameBufferSize = 10);
        virtual void append(Frame);
        virtual void flush();

    private:
        queue<Frame> frameBuffer;
        int frameBufferSize;
};
