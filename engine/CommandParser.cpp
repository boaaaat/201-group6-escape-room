#include "CommandParser.h"
#include <sstream>
#include <iomanip>

std::vector<std::string> CommandParser::tokenize(const std::string& line) 
{
    std::vector<std::string> out;
    std::istringstream iss(line);
    std::string word;
    while (iss >> std::quoted(word) || (iss.clear(), iss >> word)) 
    {
        if (!word.empty())
            out.push_back(word);
    }
    return out;
}
