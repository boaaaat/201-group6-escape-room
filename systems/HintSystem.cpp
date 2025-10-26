#include "HintSystem.h"
#include <iostream>

// registerHint:
//  - push hintLine into hintLines[contextId]
void HintSystem::registerHint(const std::string& contextId,
                              const std::string& hintLine) {
    // TODO
}

// getHint:
//  - find vector for contextId
//  - if empty, print generic "no hint"
//  - else print either the first line or rotate / pick based on inventory, etc.
void HintSystem::getHint(const std::string& contextId,
                         const Inventory& inv) const {
    // TODO
    (void)inv; // avoid unused warning until you use it for dynamic hints
}
