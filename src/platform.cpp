#include "platform.h"
#include <stdlib.h>
platform::platform(){

};
platform::~platform(){

};

void platform::read_a_ptx(const char* ptx_path){
    std::ifstream ptx_f;
    ptx_f.open(ptx_path);
    string line;
    while(getline(ptx_f,line)){
        
        func *func_tmp;
        if(line.find(".entry")!=string::npos)
        {
            //std::cout<<line<<std::endl;
            func_tmp=new func();
            func_tmp->add_line(line);
            func_tmp->get_func_txt(ptx_f);
            func_tmp->get_func_name();
            func_tmp->get_param();
            func_tmp->get_variable();
            //func_tmp->get_inst();
            //func_tmp->show_variable();
            this->funcs.push_back(*func_tmp);
            delete func_tmp;
            
        }
        

    }
};

