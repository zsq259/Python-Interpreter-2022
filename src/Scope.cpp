#include "Scope.h"
#include <unordered_map>
#include <string>
#include <vector>

Scope::Scope(){
    std::unordered_map<std::string, antlrcpp::Any> ret;
    varTable.push_back(ret);
}

void Scope::varRegister(const std::string& varName, antlrcpp::Any varData) {
    (varTable.back())[varName] = varData;
}    

std::pair<bool, antlrcpp::Any> Scope::varQuery(const std::string& varName) const {
    auto it = (varTable.back()).find(varName);
    if (it == varTable.back().end()) {
        it = varTable.front().find(varName);
        if (it == varTable.front().end()) return std::make_pair(false, 0);
    }
    return std::make_pair(true, it->second);
}