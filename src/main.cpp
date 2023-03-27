#include "platform.h"
#include <iostream>
#include <variant>

int main(int argc, char *argv[]){
    platform pf;
    const char *path="./add2.ptx";
    pf.read_a_ptx(path);
    
    dim3 d;
    d.x=32;
    cout<<"start sim"<<endl;
    float a[32],b[32];
    //ulong p=ulong(&a);
    // int op;
    // op=
    pf.sim("init_array_bra",d,2,ulong(a),ulong(b));
    //cout<<pf.gpu->global_mem_t.mem_read(0)<<hex<<endl;
    int i=0;
    for(auto &v:a){
        cout<<i<<": "<<v<<endl;
        i++;
    }
    cout<<"/////"<<endl;
    i=0;
    for(auto &v:b){
        cout<<i<<": "<<v<<endl;
        i++;
    }
    return 0;
}