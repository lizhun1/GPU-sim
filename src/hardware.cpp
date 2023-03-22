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
        // stream_processer SM_t;
        // SM_t.sid=i;
        // SM_t.attach_mem(&(this->global_mem_t));
        // this->SM.push_back(SM_t);
        SM[i].attach_mem(&(this->global_mem_t));
    };
};
GPGPU::~GPGPU(){

};
void GPGPU::prepare_tid(dim3 cuda_dim){
    for(uint i=0;i<SM_num;i++){
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
    //cout<<me->sid<<endl;
    thread c_thread;
    // thread l_thread;
    // thread w_thread;
    thread i_thread;
    cout<<"start inst_dispatch thread"<<endl;
    i_thread =thread(&(me->inst_dispatch),me);
    cout<<"start core thread"<<endl;
    c_thread= thread(&(me->core_run),me);
    // l_thread= thread(&(me->ldst_run),me);
    // w_thread =thread(&(me->warp_sch),me);
    
    // l_thread.join();
    // w_thread.join();
    i_thread.join();
    cout<<"inst exit"<<endl;
    c_thread.join();
};
void stream_processer::core_run(stream_processer *me){
    while(1){
        if(me->core_mailbox_lock.try_lock())
        {
            //cout<<"1"<<endl;
            if(me->core_mailbox.size()==1){
                //cout<<me->core_mailbox.front().inst_opcode<<endl;
                for(uint i=0;i<32;i++){
                    // core exec
                }
                if(me->core_mailbox.front().inst_opcode=="ret"){
                    me->core_mailbox.erase(me->core_mailbox.begin());
                    me->core_mailbox_lock.unlock();
                    break;
                } 
                //cout<<me->core_mailbox.size()<<endl;
                me->core_mailbox.erase(me->core_mailbox.begin());
                  
            }
            me->core_mailbox_lock.unlock();  
        }
        //else cout<<"core lock"<<endl;
        
    }
};
void stream_processer::ldst_run(stream_processer *me){
    
};
void stream_processer::warp_sch(stream_processer *me){
    
};
void stream_processer::inst_dispatch(stream_processer *me){
    while(1){
        if(me->core_mailbox_lock.try_lock()){
            //cout<<me->core_mailbox.size()<<endl;
            if(me->core_mailbox.size()==0){
                me->core_mailbox.push_back(me->inst_cache_t.read_inst(me->main_PC)); 
                
                if(me->inst_cache_t.read_inst(me->main_PC).inst_opcode=="ret") 
                {   
                    //cout<<"here ret"<<endl;
                    me->core_mailbox_lock.unlock();
                    break;
                }
                me->main_PC++;
            }
            me->core_mailbox_lock.unlock();  
        }
        //else cout<<"inst lock"<<endl;
    }
    
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
    // switch(ari_inst.inst_type.first)
    // {
    // case 1:
    //     switch (ari_inst.inst_type.second)
    //     {
    //     case 1:
    //         /* code */
    //         break;
        
    //     default:
    //         break;
    //     }
    //     break;
    // case 2:
    //     switch (ari_inst.inst_type.second)
    //     {
    //     case 1:
    //         add_s(ari_inst);
    //         break;
    //     case 2:
    //         sub_s(ari_inst);
    //         break;
    //     case 3:
    //         mul_s(ari_inst);
    //         break;
    //     case 4:
    //         div_s(ari_inst);
    //         break;
    //     case 5:
    //         mad_s(ari_inst);
    //         break;
    //     case 6:
    //         neg_s(ari_inst);
    //         break;
    //     case 7:
    //         abs_s(ari_inst);
    //         break;
    //     case 8:
    //         min_s(ari_inst);
    //         break;
    //     case 9:
    //         max_s(ari_inst);
    //         break;
    //     default:
    //         break;
    //     }
    //     break;
    
    // default:
    //     break;
    // }
};
void single_core::add_s(inst ari_inst){

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
