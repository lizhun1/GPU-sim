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
    for(uint i=0;i<SM_num;i++){
        stream_processer SM_t;
        SM_t.sid=i;
        SM_t.attach_mem(&(this->global_mem_t));
        this->SM.push_back(SM_t);
    };
};
GPGPU::~GPGPU(){

};
void GPGPU::prepare_tid(dim3 cuda_dim){
    for(uint i=0;i<SM.size();i++){
        for(uint j=0;j<SM[i].warp_size;j++){
            SM[i].reg_t.data[j][0]=i*32+j;
        }
    }
};
void GPGPU::gpu_load(vector<inst> inst_q){
    for(auto &sm: this->SM){
        sm.inst_cache_t.load_inst(inst_q);
    }
    cout<<"inst load success"<<endl;
};
void global_mem::mem_write(uint addr,ulong value){
    this->mem[addr]=value;
};
ulong global_mem::mem_read(uint addr){
    return this->mem[addr];
}
void inst_cache::load_inst(vector<inst> inst_q){
    this->inst_queue=vector<inst>(inst_q);
};
void stream_processer::run(stream_processer *me){
    cout<<me->sid<<endl;
    thread c_thread;
    thread l_thread;
    thread w_thread;
    thread i_thread;
    c_thread= thread(&(me->core_run),me);
    l_thread= thread(&(me->ldst_run),me);
    c_thread.join();
    l_thread.join();
};
void stream_processer::core_run(stream_processer *me){
    
};
void stream_processer::ldst_run(stream_processer *me){
    
};
void stream_processer::warp_sch(stream_processer *me){
    
};
void GPGPU::gpu_run(){
    vector<thread> run_thread;
    for( auto &i:SM){
        run_thread.push_back(thread(i.run,&i));
        //cout<<"0"<<endl;
    }
    for(auto &i:run_thread ){
        i.join();
    }
    cout<<"all thread exit"<<endl;
    // cout<<thread::hardware_concurrency<<endl;
};
void single_core::exec(inst ari_inst){
    switch(ari_inst.inst_type.first)
    {
    case 1:
        switch (ari_inst.inst_type.second)
        {
        case 1:
            /* code */
            break;
        
        default:
            break;
        }
        break;
    case 2:
        switch (ari_inst.inst_type.second)
        {
        case 1:
            /* code */
            break;
        
        default:
            break;
        }
        break;
    
    default:
        break;
    }
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
