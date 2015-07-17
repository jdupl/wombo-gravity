#include <vector>
#include "body.hpp"

using namespace std;

class Frame {

    public:
        Frame(unsigned int, vector<body>&);
        vector<body> bodies;
        unsigned int frameNumber;
};
