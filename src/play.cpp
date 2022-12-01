#include <iostream>
#include <cstdio>
#include <cstring>
#include "Any.h"
using namespace std;

int main(){
    antlrcpp::Any a = 1;
    cout<<a.is<vector<antlrcpp::Any> >()<<endl;
}