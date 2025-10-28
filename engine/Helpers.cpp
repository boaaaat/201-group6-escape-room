#include <iostream>
#include <algorithm>
#include <Helpers.h>

std::string toLower(const std::string& s) 
{
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                    [](unsigned char c) { return std::tolower(c); });
    return out;
}