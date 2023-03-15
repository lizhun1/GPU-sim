#include "platform.h"
#include <iostream>
#include <variant>

int main(int argc, char *argv[]){
    platform pf;
    const char *path="./add.ptx";
    pf.read_a_ptx(path);
    //pf.funcs[0].show_txt();
    return 0;
}