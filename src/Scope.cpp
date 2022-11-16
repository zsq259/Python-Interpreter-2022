#include "Scope.h"
#include <unordered_map>
#include <string>
#include <vector>


void Scope::varRegister(const std::string& varName, antlrcpp::Any varData) {
    varTable[varName] = varData;
}    

std::pair<bool, antlrcpp::Any> Scope::varQuery(const std::string& varName) const {
    auto it = varTable.find(varName);
    if (it == varTable.end()) return std::make_pair(false, 0);
    return std::make_pair(true, it->second);
}