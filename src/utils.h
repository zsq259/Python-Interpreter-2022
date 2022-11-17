#ifndef PYTHON_INTERPRETER_UTILS_H
#define PYTHON_INTERPRETER_UTILS_H

#include <string>
#include "Any.h"

int ToInt(const antlrcpp::Any &o);

double ToFloat(const antlrcpp::Any &o);

bool ToBool(const antlrcpp::Any &o);

bool validateVarName(const std::string& str);

#endif //PYTHON_INTERPRETER_UTILS_H