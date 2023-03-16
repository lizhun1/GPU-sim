#include "platform.h"
#include <iostream>
#include <variant>

int main(int argc, char *argv[]){
    platform pf;
    const char *path="./add.ptx";
    pf.read_a_ptx(path);
    dim3 d;
    cout<<"start sim"<<endl;
    pf.sim("init_array",d,2,1,1);
    // for(int i=0;i<pf.funcs.size();i++){
    //     pf.funcs[i].show_txt();
    // }
    return 0;
}