#ifndef PYTHON_INTERPRETER_UTILS_H
#define PYTHON_INTERPRETER_UTILS_H

#include <string>


int stringToInt(const std::string& str);

double stringToFloat(const std::string& str);

bool stringToBool(const std::string& str);

bool validateVarName(const std::string& str);

#endif //PYTHON_INTERPRETER_UTILS_H