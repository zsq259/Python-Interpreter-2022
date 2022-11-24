//refer to https://www.luogu.com.cn/blog/88403/solution-p5432
#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

#include "int2048.h"

namespace sjtu{
    const int P = 100;
    const int Mod=998244353;
    const int G=3,Gi=332748118;
    int2048::int2048() {}

    int2048::int2048(long long x) {
        sign = 0; num.clear();
        if(x > 0) sign = 1;
        if(x < 0) sign = -1, x = -x;
        while(x) num.push_back(x % P), x /= P;
    }

    void del_0(std::vector<int> &a) {
        int len = a.size() - 1;
        while (len >= 0 && !a[len]) a.pop_back(), --len;
    }

    int2048::int2048(const std::string &s) {
        sign = 0; num.clear();
        int ret = 0, len = s.size(), tmp = 1;
        bool o = 0;
        for (int i = len - 1; i >= 0; --i) {
            if (s[i] == '-') { sign = -1; continue; }
            ret += tmp * (s[i] - '0');
            o |= ret;
            tmp *= 10;
            if (tmp >= P) tmp = 1, num.push_back(ret), ret = 0;
        }
        if(ret) num.push_back(ret);
        del_0(num);
        if(!sign && o) sign = 1;
    }

    int2048::int2048(const int2048 &s) {
        sign = 0; num.clear();
        sign = s.sign;
        for(auto x:s.num) num.push_back(x);
    }

    int2048::int2048(int sign_, std::vector<int> num_) {
        num.clear();
        sign = sign_;
        for(auto x:num_) num.push_back(x);
    }
    
    void int2048::read(const std::string &s){
        sign = 0; num.clear();
        int ret = 0, len = s.size(), tmp = 1;
        bool o = 0;
        for (int i = len - 1; i >= 0; --i) {
            if (s[i] == '-') { sign = -1; continue; }
            ret += tmp * (s[i] - '0');
            o |= ret;
            tmp *= 10;
            if (tmp >= P) tmp = 1, num.push_back(ret), ret = 0;
        }
        if(ret) num.push_back(ret);
        del_0(num);
        if(!sign && o) sign = 1;
    }

    void my_print(std::ostream &os, const int &x) {
        int o = P/10;
        while(x < o) os<<'0', o /= 10;
        if(o) os<<x;
    }
    void int2048::print(){
        std::cout<<(*this);
    }
    int2048& int2048::add(const int2048 &b){
        if (!b.sign) return *this;
        if (!sign) return (*this) = int2048(b);
        if (sign != b.sign) {
            sign *= -1;
            (*this).minus(b);
            sign *= -1;
            return (*this);
        }
        int l1 = num.size(), l2 = b.num.size();
        int len = std::max(l1, l2);
        for (int i = len - l1; i > 0; --i) num.push_back(0);  
        for (int i = 0; i < l2; ++i) {
            num[i] += b.num[i];
        }
        int ret = 0;
        for (int i = 0; i < len; ++i) {
            num[i] += ret;
            ret = 0;
            if (num[i] >= P) num[i] -= P, ret++;
        }
        if (ret) num.push_back(ret);
        return *this;
    }

    int2048 add(int2048 a, const int2048 &b){
        return a.add(b);
    }

    int2048 minus(int2048 a, const int2048 &b);
    int2048& int2048::minus(const int2048 &b){
        if (!b.sign) return (*this);
        if (!sign) {
            (*this) = b;
            sign *= -1;
            return (*this);
        }
        if (sign != b.sign) {
            sign *= -1;
            (*this).add(b);
            sign *= -1;
            return (*this);
        }
        int o = sign > 0? (*this) < b : (*this) > b;
        int l1 = num.size(), l2 = b.num.size();
        int len = std::max(l1, l2);
        for (int i = len - l1; i > 0; --i) num.push_back(0);
        for (int i = 0; i < l2; ++i) {
            num[i] -= b.num[i];
        }
        int ret  = 0;
        if (o) {
            for (int i = 0; i < len; ++i) {
                num[i] += ret; ret = 0;
                if (num[i] > 0) num[i] -= P, ret++; 
            }
            sign *= -1;
            for (int i = 0; i < len; ++i) num[i] *= -1;
        }
        else {
            for (int  i = 0; i < len; ++i) {
                num[i] -= ret; ret = 0;
                if (num[i] < 0) num[i] += P, ret++;
            }
        }
        del_0(num);
        return (*this);
    }
    int2048 minus(int2048 a, const int2048 &b){
        return a.minus(b);
    }

    int2048 &int2048::operator=(const int2048 &s){
        sign = s.sign;num.clear();
        for(auto x:s.num) num.push_back(x);
        return *this;
    }

    int2048 &int2048::operator+=(const int2048 &b) {
        (*this).add(b);
        return *this;
    }
    int2048 operator+(int2048 a, const int2048 &b) {
        return a.add(b);
    }

    int2048 &int2048::operator-=(const int2048 &b) {
        (*this).minus(b);
        return *this;
    }
    int2048 operator-(int2048 a, const int2048 &b){
        return a.minus(b);
    }

    long long fpow(long long a, int b) {
        long long ret = 1;
        while (b) { if(b & 1) ret = ret * a % Mod; a = a * a %Mod; b >>= 1; }
        return ret;
    }

    void ntt(int *a, const int &op, int m, int *r) {
        for (int i = 0; i < m; ++i) if (i < r[i]) std::swap(a[i], a[r[i]]);
        for (int l = 2; l <= m; l <<= 1) {
            int mid = l >> 1;
            long long Omg = fpow(op? G:Gi, (Mod - 1)/l);
            for (int i = 0; i < m; i += l) {
                long long omg = 1;
                for (int j = 0; j < mid; ++j, omg = omg * Omg % Mod) {
                    long long a1 = a[i+j], a2 = 1ll * a[i+j+mid] * omg % Mod;
                    a[i+j] = (a1 + a2) % Mod;
                    a[i+j+mid] = (a1 - a2 + Mod) % Mod;
                }
            }
        }
    }

    int2048 &int2048::operator*=(const int2048 &b){
        if (!sign) return (*this);
        if (!b.sign) {
            sign = 0;
            num.clear();
            return (*this);
        }
        sign *= b.sign;
        int l1 = num.size(), l2 = b.num.size();
        int n = 1, o = 0;
        while (n < (l1 + l2)) n <<= 1, ++o;
        int *r = new int[n+1];
        int *f = new int[n+1];
        int *g = new int[n+1];
        for (int i = 0; i <= n; ++i) r[i] = f[i] = g[i] = 0;
        for (int i = 0; i < l1; ++i) f[i] = num[i];
        for (int i = 0; i < l2; ++i) g[i] = b.num[i];
        for (int i = 0; i < n; ++i) r[i] = (r[i>>1] >> 1) | ((i&1) << (o-1));  
        ntt(f, 1, n, r);ntt(g, 1, n, r);
        for (int i = 0; i < n; ++i) f[i] = 1ll * f[i] * g[i] % Mod;
        ntt(f, 0, n, r);
        long long inv = fpow(n, Mod - 2);
        for (int i = 0; i < n; ++i) f[i] = (1ll * f[i] * inv % Mod + Mod) % Mod;
        for (int i = 0; i < n; ++i) {
            f[i+1] += f[i]/P;
            f[i] %= P;     
        }
        num.clear();
        for (int i = 0; i <= n; ++i) num.push_back(f[i]);
        del_0(num);
        delete []r;
        delete []f;
        delete []g;
        return (*this);
    }
    int2048 operator*(int2048 a, const int2048 &b){
        return a *= b;
    }

    int2048 &int2048::operator<<=(int o) {
        if (!o) return (*this);
        int len = num.size();
        for (int i = 0; i < o; ++i) num.push_back(0);
        for (int i = len - 1; i >= 0; --i) num[i+o] = num[i];
        for (int i = 0; i < o; ++i) num[i] = 0;
        return (*this);
    }
    int2048 &int2048::operator>>=(int o) {
        if (!o) return (*this);
        int len = num.size();
        if (o >= len) { num.clear();return (*this); }
        for (int i = o; i < len; ++i) num[i-o] = num[i];
        for (int i = 0; i < o; ++i) num.pop_back();
        return (*this);
    }

    int2048 div(const int2048 &a, const int2048 &b) {
        if (b == int2048(1)) return a;
        std::vector<int2048> sta;
        std::vector<int2048> s;
        int2048 ret = b, tem = int2048(1);
        while (ret <= a) {
            sta.push_back(ret);
            s.push_back(tem);
            ret += ret;
            tem += tem;
        }
        ret = a;tem = int2048(0);
        int len = sta.size() - 1;
        while(len >= int2048(0)) {
            int2048 tmp = sta[len];
            if (tmp <= ret) ret -= tmp, tem += s[len];
            len--;
        }
        return tem;
    }

    int2048 inverse(const int2048 &a) {
        int m = a.num.size();
        if (m <= 10) {
            return div(int2048(1) <<= (m << 1), a); 
        }
        int k = (m + 5) >> 1;
        
        //used to wrongly moved by base^(k+1)
        int2048 b = a;
        b >>= (m - k); //get b prime
        int2048 c = inverse(b); //get c prime 
        int2048 c1 = c;
        c1 += c;
        c1 <<= (m - k);
        b = a * c * c;
        b >>= (k << 1);
        int2048 d = c1 - b; //get c star
        d -= int2048(1); //quotient
        c = (int2048(1) <<= (m << 1)) - d * a; //remain
        //while (c >= a) d += int2048(1), c -= a;
        //while (c < int2048(0)) d -= int2048(1), c += a;
        if (c >= a) d += int2048(1);
        //if (c < 0) d -= int2048(1);
        return d;
    }

    int2048 &int2048::operator/=(int2048 b){
        if (num.size() < b.num.size()) {
            (*this) = int2048(0);
            return (*this);
        }
        int o = sign * b.sign;
        sign = sign > 0? sign : -sign;
        b.sign = b.sign > 0? b.sign : -b.sign;
        
        int2048 x = (*this), y = b;
        int l1 = num.size(), l2 = b.num.size();
        if (l1 > (l2 << 1)) {
            int tmp = l1 - (l2 << 1) ;
            x <<= tmp;
            y <<= tmp;
            l2 = l1 - l2;
            l1 = l2 << 1;
        }
        int2048 z = inverse(y);
        y = x * z; //quotient
        y >>= (l2 << 1);
        x = b * y; //the quotient times b
        int2048 ret = (*this) - x;
        //while (ret >= b) y += int2048(1), ret -= b;
        //while (ret < int2048(0)) y -= int2048(1), ret += b;
        if (ret >= b) y += int2048(1);
        //if (ret < 0) y -= int2048(1);
        (*this) = y;
        sign = o;
        return (*this);
    }
    int2048 operator/(int2048 a, const int2048 &b){
        return a /= b;
    }

    std::istream &operator>>(std::istream &is, int2048 &a){
        std::string str = "";
        is >> str;
        a = int2048(str);
        return is;
    }
    std::ostream &operator<<(std::ostream &os, const int2048 &a){
        if (a.num.empty()) {
            os<<"0";
        }
        else{
            auto it = a.num.end(); it--;
            int flag = it != a.num.begin();
            if(a.sign == -1) os<<'-';
            os<<(*it);
            while (flag) {
                it--;
                my_print(os, (*it));
                if (it == a.num.begin()) flag = 0;
            }
        }
        return os;
    }

    bool operator ==(const int2048 &a, const int2048 &b){
        if (a.sign != b.sign) return false;
        if (a.num.size() != b.num.size()) return false;
        int len = a.num.size();
        for (int i = 0; i < len; ++i) {
            if (a.num[i] != b.num[i]) return false;
        }
        return true;
    }
    bool operator!=(const int2048 &a, const int2048 &b){
        return !(a == b);
    }
    bool operator<(const int2048 &a, const int2048 &b){
        if (a.sign != b.sign) return a.sign < b.sign;
        if (!a.sign) return false;
        int len = a.num.size();
        if (a.sign > 0) {
            if (len != b.num.size()) return len < b.num.size();
            for (int i = len - 1; i >= 0; --i) {
                if (a.num[i] != b.num[i]) return a.num[i] < b.num[i];
            } 
        }
        else {
            if (len != b.num.size()) return len > b.num.size();
            for (int i = len - 1; i >= 0; --i) {
                if (a.num[i] != b.num[i]) return a.num[i] > b.num[i];
            }
        }
        return false;
    }
    bool operator>(const int2048 &a, const int2048 &b){
        return !(a <= b);
    }
    bool operator<=(const int2048 &a, const int2048 &b){
        return a < b || a == b;
    }
    bool operator>=(const int2048 &a, const int2048 &b){
        return !(a < b); 
    }
}
