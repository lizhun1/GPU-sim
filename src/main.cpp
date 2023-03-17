#include "platform.h"
#include <iostream>
#include <variant>

int main(int argc, char *argv[]){
    platform pf;
    const char *path="./add.ptx";
    pf.read_a_ptx(path);
    dim3 d;
    d.x=1;
    cout<<"start sim"<<endl;
    int a[5],b[5];
    //ulong p=ulong(&a);
    pf.sim("init_array",d,2,ulong(a),ulong(b));
    
    return 0;
}