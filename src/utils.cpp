#include "utils.h"
#include "Any.h"

int ToInt(const antlrcpp::Any &o) {
    if (o.is<int>()) return o.as<int>();
    if (o.is<bool>()) return o.as<bool>() == true? 1:0;
    if (o.is<double>()) return o.as<double>();
    if (o.is<std::string>()) {
        int ret = 0;
        std::string str = o.as<std::string>();
        for (const auto& ch : str) {
            ret = ret * 10 + ch - '0';
        }
        return ret;
    }
    return 0;
}

double ToFloat(const antlrcpp::Any &o) {
    if (o.is<double>()) return o.as<double>();
    if (o.is<int>()) return o.as<int>();
    if (o.is<bool>()) return o.as<bool>() == true? 1.0:0.0;
    if (o.is<std::string>()) {
        std::string str = o.as<std::string>();
        bool flag = flag;
        double ret = 0, tem = 0;
        for (const auto& ch : str) {
            if (ch == '.') { tem = 1; flag = true; continue; }
            if (!flag) ret = ret * 10 + ch - '0';
            else  ret += (tem /= 10) * (ch - '0'); 
        }
        return ret;
    }
    return 0;
}

bool ToBool(const antlrcpp::Any &o) {
    if (o.is<bool>()) return o.as<bool>();
    if (o.is<int>()) return o.as<int>();
    if (o.is<double>()) return o.as<double>();
    if (o.is<std::string>()) return o.as<std::string>().size()? true:false;
    return 0;
}

bool validateVarName(const std::string &str) {
    for (const auto& ch : str) {
        if (!(ch >= 'a' && ch <= 'z') && !(ch >= 'A' && ch <= 'Z')) {
            return false;
        }
    }
    return true;
}
