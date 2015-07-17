#include "frame.hpp"

#include <vector>

using namespace std;

Frame::Frame(unsigned int frameNumber, vector<body>& bodies) {
    this->frameNumber = frameNumber;
    this->bodies = bodies;
}
