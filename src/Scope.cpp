#include "Scope.h"
#include <any_op.h>
#include <unordered_map>
#include <string>
#include <vector>

Scope::Scope() {
    std::unordered_map<std::string, antlrcpp::Any> ret;
    varTable.push_back(ret);
}

void Scope::varRegister(const std::string& varName, antlrcpp::Any varData) {
    if(varTable.back().find(varName) != varTable.back().end()) (varTable.back())[varName] = varData;
    else if(varTable.front().find(varName) != varTable.front().end()) (varTable.front())[varName] = varData;
    else (varTable.back())[varName] = varData;
}    

void Scope::newScope() {
    std::unordered_map<std::string, antlrcpp::Any> ret;
    varTable.push_back(ret);
}

void Scope::delScope() {
    if(!varTable.empty()) varTable.pop_back();
}

std::pair<bool, antlrcpp::Any> Scope::varQuery(const std::string& varName) const {
    auto it = (varTable.back()).find(varName);
    if (it == varTable.back().end()) {
        it = varTable.front().find(varName);
        if (it == varTable.front().end()) return std::make_pair(false, 0);
    }
    return std::make_pair(true, it->second);
}