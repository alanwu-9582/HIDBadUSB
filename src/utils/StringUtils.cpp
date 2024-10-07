#include "StringUtils.h"

using std::string;
using std::vector;

vector<string> splitString(string str, string delimiter) {
    vector<string> result;
    int startIndex = 0;
    int endIndex = str.find(delimiter);
    while (endIndex != -1) {
        result.push_back(str.substr(startIndex, endIndex - startIndex));
        startIndex = endIndex + delimiter.size();
        endIndex = str.find(delimiter, startIndex);
    }
    result.push_back(str.substr(startIndex, endIndex - startIndex));
    return result;
}