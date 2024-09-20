#ifndef MOUSEDICTIONARY_H
#define MOUSEDICTIONARY_H

#include <unordered_map> 
#include <string>
#include <cstring>

using std::unordered_map;
using std::string;

unordered_map<string, char> MouseDict = {
    {"LEFT", 0x01},
    {"RIGHT", 0x02},
    {"MIDDLE", 0x04},
    {"BACKWARD ", 0x08},
    {"FORWARD", 0x10},
    {"ALL", 0x1F}
};

#endif 