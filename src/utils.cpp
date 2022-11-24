#include "utils.h"
#include "Any.h"
#include "int2048.h"
using sjtu::int2048;

int2048 ToInt(const antlrcpp::Any &o) {
    if (o.is<int2048>()) return o.as<int2048>();
    if (o.is<bool>()) return o.as<bool>() == true? 1:0;
    if (o.is<double>()) return o.as<double>();
    if (o.is<std::string>()) {
        int2048 ret = 0;
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
    if (o.is<int2048>()) {
        int2048 tem = o.as<int2048>();
        double ret = 0;
        for(int i = tem.num.size() - 1; i >= 0; --i) {
            ret = ret * 100 + tem.num[i];
        }
        ret *= tem.sign;
        return ret;
    }
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
    if (o.is<int2048>()) return o.as<int2048>().sign;
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
