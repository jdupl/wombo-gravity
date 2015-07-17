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

#include "../../../lib/json/json/json.h"

using namespace std;

class JsonWriter : public FrameWriter {

    public:
        JsonWriter(string filename);
        virtual void append(Frame) override;
        virtual void flush() override;

    private:
        string filename;
        Json::Value framesJson;
};
