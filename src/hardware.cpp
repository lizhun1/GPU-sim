#include "hardware.h"
void reg_file::reg_initial(ulong value){
    for(int i=0;i<64;i++){
        for(int j=0;j<64;j++){
            this->data[i][j]=value;
        }
    }
};
void stream_processer::attach_mem(global_mem *mem){
    this->global_mem_t=mem;
};
GPGPU::GPGPU(){
    for(int i=0;i<SM_num;i++){
        stream_processer SM_t;
        SM_t.attach_mem(&(this->global_mem_t));
        this->SM.push_back(SM_t);
    };
};
GPGPU::~GPGPU(){

};
reg_file::reg_file(){};
reg_file::~reg_file(){};
single_core::single_core(){};
single_core::~single_core(){};
load_store_unit::load_store_unit(){};
load_store_unit::~load_store_unit(){};
inst_cache::inst_cache(){};
inst_cache::~inst_cache(){};
stream_processer::stream_processer(){};
stream_processer::~stream_processer(){};
global_mem::global_mem(){};
global_mem::~global_mem(){};