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
    if (a.is<std::pair<std::string, antlrcpp::Any> >()) os << a.as<std::pair<std::string, antlrcpp::Any> >().second;
    else if (a.is<bool>()) os<<(a.as<bool>()? "True":"False");
    else if (a.is<int2048>()) os<<a.as<int2048>();
    else if (a.is<double>()) os<<a.as<double>();
    else if (a.is<std::string>()) os<<a.as<std::string>();
    else if (a.isNull()) os<<"None";
    return os;
}

antlrcpp::Any operator +(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if (a.is<std::string>() && b.is<std::string>()) return a.as<std::string>() + b.as<std::string>();
    if (a.is<double>() || b.is<double>()) return std::move(ToFloat(a) + ToFloat(b));
    return std::move(ToInt(a) + ToInt(b));
}

antlrcpp::Any operator -(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if(a.is<double>() || b.is<double>()) return std::move(ToFloat(a) - ToFloat(b));
    return std::move(ToInt(a) - ToInt(b));
}

antlrcpp::Any operator *(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if(a.is<std::string>()){
        std::string ret = "";
        int2048 tem = ToInt(b);
        while(tem > 0) ret += a.as<std::string>(), tem = tem - 1;
        return std::move(ret);
    }
    if(b.is<std::string>()){
        std::string ret = "";
        int2048 tem = ToInt(a);
        while(tem > 0) ret += b.as<std::string>(), tem = tem - 1;
        return std::move(ret);
    }
    if(a.is<double>() || b.is<double>()) return std::move(ToFloat(a) * ToFloat(b));
    return std::move(ToInt(a) * ToInt(b));
}

antlrcpp::Any operator /(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    return std::move(ToFloat(a) / ToFloat(b));
}

antlrcpp::Any operator %(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    return std::move(ToInt(a) % ToInt(b));
}

antlrcpp::Any int_div(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    return std::move(ToInt(a) / ToInt(b));
}

bool operator ==(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if (a.isNull() || b.isNull()) {
        if (a.isNull() && b.isNull()) return true;
        else return false;
    }

    /*
    if (a.is<bool>() || b.is<bool>()) return ToBool(a) == ToBool(b);
    */
    
    if (a.is<std::string>() && b.is<std::string>()) return a.as<std::string>() == b.as<std::string>();
    else if (a.is<std::string>() || b.is<std::string>()) return false;
    if (a.is<bool>() && b.is<bool>()) return ToBool(a) == ToBool(b);
    
    if (a.is<double>() || b.is<double>()) return ToFloat(a) == ToFloat(b);
    return ToInt(a) == ToInt(b);
}

bool operator <(const antlrcpp::Any &a, const antlrcpp::Any &b) {
    if (a.is<std::string>() && b.is<std::string>()) return a.as<std::string>() < b.as<std::string>();
    if (a.is<double>() || b.is<double>()) return ToFloat(a) < ToFloat(b);
    return ToInt(a) < ToInt(b);
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
    if (a.isNull() || b.isNull()) {
        if (a.isNull() && b.isNull()) return false;
        else return true;
    }
    return !(a == b);
}
