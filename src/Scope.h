#ifndef PYTHON_INTERPRETER_SCOPE_H
#define PYTHON_INTERPRETER_SCOPE_H

#include <Any.h>
#include <unordered_map>
#include <string>
#include <vector>

class Scope {

    private:
        //std::vector< std::unordered_map<std::string, antlrcpp::Any> > varTable;
        std::vector<std::unordered_map<std::string, antlrcpp::Any> > varTable;

    public:
        Scope();
        void newScope();
        void delScope();
        void varRegister(const std::string& varName, antlrcpp::Any varData);
        std::pair<bool, antlrcpp::Any> varQuery(const std::string& varName) const;
};

#endif //PYTHON_INTERPRETER_PIE_SCOPE_H