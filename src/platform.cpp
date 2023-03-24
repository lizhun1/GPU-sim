#include "platform.h"
#include <stdlib.h>
#include <cassert>
platform::platform(){
    this->init_hardware();
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
            //func_tmp->show_txt();
            func_tmp->get_func_name();
            func_tmp->get_param();
            func_tmp->get_variable();
            //func_tmp->show_txt();
            func_tmp->get_inst();
            //func_tmp->show_variable();
            this->funcs.push_back(*func_tmp);
            delete func_tmp;
            
        }
        

    }
};
void platform::create_context(uint func_idx,dim3 cuda_dim,vector<any> real_param){
    func *sim_func;
    sim_func=&(funcs[func_idx]);
    //sim_func->show_txt();
    uint offset=3;
    //ctx.s_t.add_varible(array_name+to_string(j+1),(enum data_type) data_type_lib_int[var.variable_data_t],0);
    for(uint i=0;i<sim_func->variables.size();i++){
        
        auto var=sim_func->variables[i];
        //enum data_type d_t=1;
        if(var.variable_name.find("<")!=string::npos)
        {
            uint array_num=stoi(split_multi(var.variable_name,"<>").back());
            auto array_name=split_multi(var.variable_name,"<>").front();
            for(uint j=0;j<array_num;j++){
                cout<<array_name+to_string(j+1)+" is on "+to_string(j+offset)<<endl;
                ctx.s_t.add_varible(array_name+to_string(j+1),(enum data_type) data_type_lib_int[var.variable_data_t],offset+j);
            }
            offset=offset+array_num;
        }
        //cout<<var.variable_name<<endl;
        else 
            {
                ctx.s_t.add_varible(var.variable_name,(enum data_type) data_type_lib_int[var.variable_data_t],offset);
                cout<<var.variable_name+" is on "+to_string(offset)<<endl;
                offset++;
            }
    }
    for(uint i=0;i<sim_func->param.size();i++){
        auto par=sim_func->param[i];
        //enum data_type d_t=1;
        ctx.s_t.add_param(par.first,(enum data_type) data_type_lib_int[par.second],i);
        gpu->global_mem_t.mem_write(i,any_cast<ulong>(real_param[i]));
        cout<<"global mem addr "+to_string(i)+" is "<<hex<<gpu->global_mem_t.mem_read(i)<<endl;
    }
    //
    //translate the operands
     for(uint i=0;i<sim_func->inst_queue.size();i++){
         sim_func->inst_queue[i].trans(ctx.s_t);
     }
     //prepare tid.x
     gpu->prepare_tid(cuda_dim);
    
};
void platform::init_hardware(){
    this->gpu=new GPGPU();
}
void platform::load_inst_cache(uint func_idx){
    gpu->gpu_load(funcs[func_idx].inst_queue);
};
void platform::sim(string func_name,dim3 cuda_dim,uint param_num,... ){
    
    uint func_idx=0;
    //cout<<"hi"<<endl;
    for(uint i=0;i<funcs.size();i++){
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
    
    for(uint i=0;i<param_num;i++){
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
            r_param.push_back(va_arg(arg_ptr,double));
            break;
        case 8:
            r_param.push_back(va_arg(arg_ptr,double));
            break; 
        case 9:
            r_param.push_back(va_arg(arg_ptr,int));
            break;   
        default:
            cout<<"unknown  kernel param type"<<endl;
            exit(EXIT_FAILURE);
            break;
        }

    }
    cout<<"create context"<<endl;
    this->create_context(func_idx,cuda_dim,r_param);
    //cout<<this->gpu->global_mem_t.mem_read(0)<<"test "<<endl;
    this->load_inst_cache(func_idx);
    this->gpu->gpu_run();
    // this->del_context();
}

