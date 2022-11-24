#ifndef PYTHON_INTERPRETER_UTILS_H
#define PYTHON_INTERPRETER_UTILS_H

#include <string>
#include "Any.h"
#include "int2048.h"
using sjtu::int2048;

int2048 ToInt(const antlrcpp::Any &o);

double ToFloat(const antlrcpp::Any &o);

std::string ToString(const antlrcpp::Any &o);

bool ToBool(const antlrcpp::Any &o);

bool validateVarName(const std::string& str);

#endif //PYTHON_INTERPRETER_UTILS_H