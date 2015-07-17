#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

#include "frame.hpp"

using namespace std;

class FrameWriter {

    public:
        virtual void append(Frame) = 0;
        virtual void flush() = 0;
};
