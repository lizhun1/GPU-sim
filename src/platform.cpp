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
            func_tmp->get_inst();
            //func_tmp->show_variable();
            this->funcs.push_back(*func_tmp);
            delete func_tmp;
            
        }
        

    }
};
void platform::create_context(vector<any> real_param){

};
void platform::sim(string func_name,dim3 cuda_dim,int param_num,... ){
    int func_idx=0;
    
    for(int i=0;i<funcs.size();i++){
        if(funcs[i].func_name.find(func_name)!=string::npos)
        {
            func_idx=i;
            break;
        }

    }
    //unknown param

}
