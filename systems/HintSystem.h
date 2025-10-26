#pragma once
#include <string>
#include <map>
#include <vector>
#include "Inventory.h"

// The hint system can:
// - provide context hints about current area / room
// - optionally adapt if you already found certain key items
//
// Implementation note:
//  You can decide how dynamic you want it.
//  For now we'll assume each Area and each Room can set a "hint code",
//  and HintSystem returns a line of text for that code.
class HintSystem {
public:
    // register a hint line for a context id
    void registerHint(const std::string& contextId,
                      const std::string& hintLine);

    // getHint:
    //  - look up contextId
    //  - maybe factor in if inv has certain items (advanced later)
    //  - print the hint line
    void getHint(const std::string& contextId,
                 const Inventory& inv) const;

private:
    std::map<std::string, std::vector<std::string>> hintLines;
};
