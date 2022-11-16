#include "utils.h"

int stringToInt(const std::string& str) {
    int ret = 0;
    for (const auto& ch : str) {
        ret = ret * 10 + ch - '0';
    }
    return ret;
}

double stringToFloat(const std::string& str) {
    int o = 0;
    double ret = 0, tem = 0;
    for (const auto& ch : str) {
        if (ch == '.') { o = tem = 1;continue; }
        if (!o) ret = ret * 10 + ch - '0';
        else  ret += (tem /= 10) * (ch - '0'); 
    }
    return ret;
}

bool stringToBool(const std::string& str) {
    if (str == "True") return true;
    else return false;
}

bool validateVarName(const std::string& str) {
    for (const auto& ch : str) {
        if (!(ch >= 'a' && ch <= 'z') && !(ch >= 'A' && ch <= 'Z')) {
            return false;
        }
    }
    return true;
}
