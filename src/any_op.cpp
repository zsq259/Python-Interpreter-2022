#include "Evalvisitor.h"
#include <Any.h>
#include "Python3BaseVisitor.h"
#include "Scope.h"
#include "utils.h"
#include "int2048.h"
using sjtu::int2048;

void transVar(antlrcpp::Any &a) {
    if (a.is<std::pair<std::string, antlrcpp::Any> >()) a = a.as<std::pair<std::string, antlrcpp::Any> >().second;
}

std::ostream& operator<<(std::ostream &os, const antlrcpp::Any &a) {
    if (a.is<bool>()) os<<(a.as<bool>()? "True":"False");
    if (a.is<int2048>()) os<<a.as<int2048>();
    if (a.is<double>()) os<<a.as<double>();
    if (a.is<std::string>()) os<<a.as<std::string>();
    return os;
}

antlrcpp::Any operator +(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if (a.is<std::string>() && b.is<std::string>()) return a.as<std::string>() + b.as<std::string>();
    if (a.is<double>() || b.is<double>()) return ToFloat(a) + ToFloat(b);
    return ToInt(a) + ToInt(b);
    return 0;
}

antlrcpp::Any operator -(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if(a.is<double>() || b.is<double>()) return ToFloat(a) - ToFloat(b);
    return ToInt(a) - ToInt(b);
    return 0;
}

antlrcpp::Any operator *(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if(a.is<std::string>() && b.is<int2048>()){
        std::string ret = "";
        int2048 tem = b.as<int2048>();
        while(tem > 0) ret += a.as<std::string>(), tem = tem - 1;
        return ret;
    }
    if(a.is<double>() || b.is<double>()) return ToFloat(a) * ToFloat(b);
    return ToInt(a) * ToInt(b);
    return 0;
}

antlrcpp::Any operator /(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    return ToFloat(a) / ToFloat(b);
    return 0;
}

antlrcpp::Any operator %(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if(a.is<int2048>() && b.is<int2048>()) return a.as<int2048>() % b.as<int2048>();
    return 0;
}

antlrcpp::Any int_div(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if(a.is<int2048>() && b.is<int2048>()) return a.as<int2048>() / b.as<int2048>();
    return 0;
}

bool operator ==(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if (a.is<int2048>() && b.is<int2048>()) return a.as<int2048>() == b.as<int2048>();
    if (a.is<double>() && b.is<double>()) return a.as<double>() == b.as<double>();
    if (a.is<std::string>() && b.is<std::string>()) return a.as<std::string>() == b.as<std::string>();
    return 0;
}

bool operator <(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if (a.is<int2048>() && b.is<int2048>()) return a.as<int2048>() < b.as<int2048>();
    if (a.is<double>() && b.is<double>()) return a.as<double>() < b.as<double>();
    if (a.is<std::string>() && b.is<std::string>()) return a.as<std::string>() < b.as<std::string>();
    return 0;
}

bool operator <=(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    return a < b || a == b;
}

bool operator >(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    return !(a <= b);
}

bool operator >=(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    return !(a < b);
}

bool operator !=(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    return !(a == b);
}
