#include "platform.h"
#include <stdlib.h>
#include <cassert>
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
void platform::create_context(int func_idx,dim3 cuda_dim,vector<any> real_param){
    func *sim_func;
    sim_func=&(funcs[func_idx]);
    //sim_func->show_txt();
    //next day to build symbol table
};
void platform::sim(string func_name,dim3 cuda_dim,int param_num,... ){
    
    int func_idx=0;
    //cout<<"hi"<<endl;
    for(int i=0;i<funcs.size();i++){
        if(funcs[i].func_name.find(func_name)!=string::npos)
        {
            func_idx=i;
            break;
        }

    }
    //
    //cout<<funcs[func_idx].param.size()<<endl;
    assert(param_num==funcs[func_idx].param.size());
    //unknown param
    
    va_list arg_ptr;
    vector<any> r_param;
    va_start(arg_ptr,param_num);
    
    for(int i=0;i<param_num;i++){
        auto param_txt=funcs[func_idx].param[i];
        
        //cout<<param_txt.first<<endl;
        //judge param type
        switch (data_type_lib_int[param_txt.second])
        {
        case 1:
            r_param.push_back(va_arg(arg_ptr,uint));
            break;
        case 2:
            r_param.push_back(va_arg(arg_ptr,ulong));
            break;
        case 3:
            r_param.push_back(va_arg(arg_ptr,uint));
            break;
        case 4:
            r_param.push_back(va_arg(arg_ptr,ulong));
            break;
        case 5:
            r_param.push_back(va_arg(arg_ptr,int));
            break;
        case 6:
            r_param.push_back(va_arg(arg_ptr,long));
            break;
        case 7:
            r_param.push_back(va_arg(arg_ptr,float));
            break;
        case 8:
            r_param.push_back(va_arg(arg_ptr,double));
            break; 
        case 9:
            r_param.push_back(va_arg(arg_ptr,bool));
            break;   
        default:
            cout<<"unknown  kernel param type"<<endl;
            exit(EXIT_FAILURE);
            break;
        }

    }
    this->create_context(func_idx,cuda_dim,r_param);
}
