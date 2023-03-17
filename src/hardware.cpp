#include "hardware.h"
void reg_file::reg_initial(ulong value){
    for(int i=0;i<64;i++){
        for(int j=0;j<64;j++){
            this->data[i][j]=value;
        }
    }
};
void single_core::attach_reg(reg_file *r){
    this->reg=r;

}
void stream_processer::attach_mem(global_mem *mem){
    this->global_mem_t=mem;
    for(int i=0;i<32;i++){
        this->ldst[i].attach_mem(mem);
        this->core[i].attach_reg(&(this->reg_t));
    }
};
void load_store_unit::attach_mem(global_mem* mem){
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
void global_mem::mem_write(uint addr,ulong value){
    this->mem[addr]=value;
};
ulong global_mem::mem_read(uint addr){
    return this->mem[addr];
}
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