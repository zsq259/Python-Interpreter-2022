#ifndef PYTHON_INTERPRETER_ANY_OP_H
#define PYTHON_INTERPRETER_ANY_OP_H
#include <Any.h>
#include "Python3BaseVisitor.h"
#include "Scope.h"
#include "utils.h"
#include "int2048.h"

antlrcpp::Any operator +(const antlrcpp::Any &a, const antlrcpp::Any &b);
antlrcpp::Any operator -(const antlrcpp::Any &a, const antlrcpp::Any &b);
antlrcpp::Any operator *(const antlrcpp::Any &a, const antlrcpp::Any &b);
antlrcpp::Any operator /(const antlrcpp::Any &a, const antlrcpp::Any &b);
antlrcpp::Any operator %(const antlrcpp::Any &a, const antlrcpp::Any &b);
antlrcpp::Any int_div(const antlrcpp::Any &a, const antlrcpp::Any &b);
bool operator <(const antlrcpp::Any &a, const antlrcpp::Any &b);
bool operator >(const antlrcpp::Any &a, const antlrcpp::Any &b);
bool operator ==(const antlrcpp::Any &a, const antlrcpp::Any &b);
bool operator >=(const antlrcpp::Any &a, const antlrcpp::Any &b);
bool operator <=(const antlrcpp::Any &a, const antlrcpp::Any &b);
bool operator !=(const antlrcpp::Any &a, const antlrcpp::Any &b);
std::ostream& operator<<(std::ostream &os, const antlrcpp::Any &a);
#endif //PYTHON_INTERPRETER_ANY_OP_H