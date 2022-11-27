#include "utils.h"
#include "Any.h"
#include "int2048.h"
#include "any_op.h"
using sjtu::int2048;

int2048 ToInt(const antlrcpp::Any &o) {
    if (o.is<int2048>()) return o.as<int2048>();
    if (o.is<bool>()) return o.as<bool>() == true? 1:0;
    if (o.is<double>()) return o.as<double>();
    if (o.is<std::string>()) {
        return std::move(int2048(o.as<std::string>()));
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
        return std::move(ret);
    }
    if (o.is<bool>()) return o.as<bool>() == true? 1.0:0.0;
    if (o.is<std::string>()) {
        std::string str = o.as<std::string>();
        bool flag = false;
        double ret = 0, tem = 0;
        int f = 1;
        for (const auto& ch : str) {
            if (ch == '-') { f = -1; continue; }
            if (ch == '.') { tem = 1; flag = true; continue; }
            if (!flag) ret = ret * 10 + ch - '0';
            else  ret += (tem /= 10) * (ch - '0');
        }
        return std::move(f * ret);
    }
    return 0;
}

std::string ToString(const antlrcpp::Any &o) {
    if (o.is<std::string>()) return o.as<std::string>();
    if (o.is<int2048>()) {
        int2048 ret = o.as<int2048>();
        if (!ret.sign) return "0";
        std::string str = "", f = "";
        if (ret.sign < 0) f = "-";
        for (int i = 0; i < ret.num.size(); ++i) {
            int tem = ret.num[i];
            if (i == ret.num.size() - 1) {
                while(tem) str += '0'+ (tem % 10), tem /= 10;
            }
            else {
                for (int j = 0; j < 2; ++j) str += '0'+ (tem % 10), tem /= 10;
            }
        }
        for (int i = 0; i < (str.size() / 2); ++i) {
            std::swap(str[i], str[str.size() - 1 - i]);
        }
        return f + str;
    }
    if (o.is<double>()) {
        return std::to_string(o.as<double>());
    }
    if (o.is<bool>()) {
        return o.as<bool>()? "True" : "False";
    }
    if (o.isNull()) return "None";
    return "";
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
