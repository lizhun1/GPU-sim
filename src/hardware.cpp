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
    for(uint i=0;i<32;i++){
        this->ldst[i].attach_mem(mem);
        this->core[i].attach_reg(&(this->reg_t));
        this->core[i].global_mem_t=mem;
        this->core[i].core_id=i;
    }
};
void load_store_unit::attach_mem(global_mem* mem){
    this->global_mem_t=mem;
};
GPGPU::GPGPU(){
    mask m_a;
    for(uint i=0;i<32;i++){
        m_a.m[i]=bool(1);
    }
    for(uint i=0;i<SM_num;i++){
        SM[i].attach_mem(&(this->global_mem_t));
        SM[i].active_mask=m_a;
        //SM[i].simt_stack.push_back(pair<uint,mask>(0,m_a));
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
    cout<<"core exit"<<endl;
    me->show_reg(me);
};
void stream_processer::core_run(stream_processer *me){
    while(1){
        if(me->core_mailbox_lock.try_lock())
        {
            if(me->core_mailbox.size()==1){
                //handle bra
                cout<<me->core_mailbox.front().inst_opcode<<endl;
                cout<<me->core_mailbox.front().is_pred<<endl;
                if(me->core_mailbox.front().inst_opcode=="bra"&&me->core_mailbox.front().is_pred==false)
                {
                    cout<<"///////////uni bra to"<<me->core_mailbox.front().real_operand.front().value<<"/////////////"<<endl;
                    me->main_PC=me->core_mailbox.front().real_operand.front().value;
                }
                else{
                    if(me->core_mailbox.front().is_pred==true&&me->core_mailbox.front().inst_opcode=="bra")
                    {
                        cout<<"pred bra//////////////////////////////"<<endl;
                
                        mask m_b;
                        mask m_b_rev;
                        for(uint i=0;i<32;i++){
                            m_b.m[i]=me->core[i].p;
                            m_b_rev.m[i]=!me->core[i].p;
                        }   
                    //gen mask
                        if(me->core_mailbox.front().real_operand.front().value<me->main_PC){
                            pair<uint,mask> simt_stack_element;
                            me->active_mask=m_b;
                            simt_stack_element.first=me->main_PC;
                            simt_stack_element.second=m_b_rev;
                            me->main_PC=me->core_mailbox.front().real_operand.front().value;
                            me->simt_stack.push_back(simt_stack_element);
                        }
                        else
                        {
                            pair<uint,mask> simt_stack_element;
                            simt_stack_element.first=me->core_mailbox.front().real_operand.front().value;
                            me->active_mask=m_b_rev;
                            simt_stack_element.second=m_b;
                            me->simt_stack.push_back(simt_stack_element);
                        }
                    }
                    //branch  convergence
                    if(me->simt_stack.size()>0&&me->main_PC==me->simt_stack.back().first){
                        cout<<"get to convergence"<<endl;
                        me->active_mask=me->simt_stack.back().second;
                        me->simt_stack.pop_back();

                    }
                    //cout<<"error"<<endl;
                    ////////////core
                    streambuf *ori=cout.rdbuf();
                    //cout.rdbuf(NULL);
                    for(uint i=0;i<32;i++)
                    {
                        if(me->active_mask.m[i]){
                            if(me->core_mailbox.front().is_pred){
                                if(me->core[i].p){
                                    me->core[i].exec(me->core_mailbox.front());
                                }
                                
                            }
                            else{
                                me->core[i].exec(me->core_mailbox.front());
                            }
                            
                        }
                    }
                    cout.rdbuf(ori);
                    //////////////

                }
                cout<<"pc is "<<me->main_PC<<endl;
                if(me->core_mailbox.front().inst_opcode=="ret"){
                    if(me->simt_stack.size()==0){
                        me->core_mailbox.erase(me->core_mailbox.begin());
                        me->core_mailbox_lock.unlock();
                        break;
                    }
                    else{
                        cout<<"here"<<endl;
                        me->main_PC=me->simt_stack.back().first;
                        me->active_mask=me->simt_stack.back().second;
                        me->simt_stack.pop_back();
                        //me->core_mailbox.erase(me->core_mailbox.begin());
                        //me->core_mailbox_lock.unlock();
                        //break;
                    } 
                }
                
                me->core_mailbox.erase(me->core_mailbox.begin());
                  
            }
            me->core_mailbox_lock.unlock();  
        }
        
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
                
                if(me->inst_cache_t.read_inst(me->main_PC).inst_opcode=="ret"&&me->simt_stack.size()==0) 
                {   
                    //cout<<"here ret"<<endl;
                    me->core_mailbox_lock.unlock();
                    break;
                }
                else{
                    if(me->inst_cache_t.read_inst(me->main_PC).inst_opcode=="ret"&&me->simt_stack.size()>0)
                    {
                        //dont change pc
                    }
                    else{
                        me->main_PC++;
                    }
                    
                }
                
            }
            me->core_mailbox_lock.unlock();  
        }
        //else cout<<"inst lock"<<endl;
    }
    
};
void stream_processer::show_reg(stream_processer *me){
    for(auto &i:me->reg_t.data[1]){
        cout<<i<<endl;
    }
}
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
            add_s(ari_inst);
            break;
        case 2:
            sub_s(ari_inst);
            break;
        case 3:
            mul_s(ari_inst);
            break;
        case 4:
            div_s(ari_inst);
            break;
        case 5:
            mad_s(ari_inst);
            break;
        case 6:
            neg_s(ari_inst);
            break;
        case 7:
            abs_s(ari_inst);
            break;
        case 8:
            min_s(ari_inst);
            break;
        case 9:
            max_s(ari_inst);
            break;
        default:
            break;
        }
        break;
    case 4:
        switch (ari_inst.inst_type.second)
        {
        case 1:
            setp_s(ari_inst);
            break;
        case 2:
            selp_s(ari_inst);
            break;
        default:
            break;
        }
        break;
    case 7:
        switch (ari_inst.inst_type.second)
        {
        case 1:
            mov_s(ari_inst);
            break;
        case 2:
            ld_s(ari_inst);
            break;
        case 3:
            st_s(ari_inst);
            break;
        case 4:
            cvta_s(ari_inst);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
};
ulong reg_file::reg_read(uint x,uint y){
    return this->data[x][y];
}
void reg_file::reg_write(uint x,uint y,ulong value){
    this->data[x][y]=value;
}
void single_core::add_s(inst ari_inst){
    if(ari_inst.is_float){
        auto s_reg1_offset=ari_inst.real_operand[2].value;
        auto s_reg1=double(this->reg->reg_read(core_id,s_reg1_offset));
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=double(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=double(s_reg1+s_reg2);
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    else{
        auto s_reg1_offset=ari_inst.real_operand[2].value;
        auto s_reg1=long(this->reg->reg_read(core_id,s_reg1_offset));
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=long(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=long(s_reg1+s_reg2);
        //cout<<d_reg<<endl;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    cout<<"add"<<endl;
}
void single_core::mul_s(inst ari_inst){
    if(ari_inst.is_float){
        double s_reg1;
        if(ari_inst.real_operand[2].op_t==0){
            auto s_reg1_offset=ari_inst.real_operand[2].value;
            s_reg1=double(this->reg->reg_read(core_id,s_reg1_offset));
        }
        else{
            s_reg1=double(ari_inst.real_operand[2].value);
        }
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=double(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=double(s_reg1*s_reg2);
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    else{
        long s_reg1;
        if(ari_inst.real_operand[2].op_t==0){
            auto s_reg1_offset=ari_inst.real_operand[2].value;
            s_reg1=long(this->reg->reg_read(core_id,s_reg1_offset));
        }
        else{
            s_reg1=long(ari_inst.real_operand[2].value);
        }
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=long(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=long(s_reg1*s_reg2);
        cout<<"mul"<<endl;
        //cout<<d_reg<<endl;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
}
void single_core::sub_s(inst ari_inst){
    if(ari_inst.is_float){
        auto s_reg1_offset=ari_inst.real_operand[2].value;
        auto s_reg1=double(this->reg->reg_read(core_id,s_reg1_offset));
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=double(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=double(s_reg2-s_reg1);
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    else{
        auto s_reg1_offset=ari_inst.real_operand[2].value;
        auto s_reg1=long(this->reg->reg_read(core_id,s_reg1_offset));
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=long(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=long(s_reg2-s_reg1);
        //cout<<d_reg<<endl;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
}
void single_core::div_s(inst ari_inst){
    if(ari_inst.is_float){
        auto s_reg1_offset=ari_inst.real_operand[2].value;
        auto s_reg1=double(this->reg->reg_read(core_id,s_reg1_offset));
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=double(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=double(s_reg2/s_reg1);
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    else{
        auto s_reg1_offset=ari_inst.real_operand[2].value;
        auto s_reg1=long(this->reg->reg_read(core_id,s_reg1_offset));
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=long(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=long(s_reg2/s_reg1);
        //cout<<d_reg<<endl;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
}
void single_core::abs_s(inst ari_inst){
    if(ari_inst.is_float){
        auto s_reg1_offset=ari_inst.real_operand[1].value;
        auto s_reg1=double(this->reg->reg_read(core_id,s_reg1_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=abs(s_reg1);
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    else{
        auto s_reg1_offset=ari_inst.real_operand[1].value;
        auto s_reg1=long(this->reg->reg_read(core_id,s_reg1_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=abs(s_reg1);
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
}
void single_core::max_s(inst ari_inst){
    if(ari_inst.is_float){
        auto s_reg1_offset=ari_inst.real_operand[2].value;
        auto s_reg1=double(this->reg->reg_read(core_id,s_reg1_offset));
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=double(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=(s_reg1>s_reg2)?s_reg1:s_reg2;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    else{
        auto s_reg1_offset=ari_inst.real_operand[2].value;
        auto s_reg1=long(this->reg->reg_read(core_id,s_reg1_offset));
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=long(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=(s_reg1>s_reg2)?s_reg1:s_reg2;
        //cout<<d_reg<<endl;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
}
void single_core::min_s(inst ari_inst){
    if(ari_inst.is_float){
        auto s_reg1_offset=ari_inst.real_operand[2].value;
        auto s_reg1=double(this->reg->reg_read(core_id,s_reg1_offset));
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=double(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=(s_reg1<s_reg2)?s_reg1:s_reg2;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    else{
        auto s_reg1_offset=ari_inst.real_operand[2].value;
        auto s_reg1=long(this->reg->reg_read(core_id,s_reg1_offset));
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=long(this->reg->reg_read(core_id,s_reg2_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=(s_reg1<s_reg2)?s_reg1:s_reg2;
        //cout<<d_reg<<endl;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
}
void single_core::neg_s(inst ari_inst){
    if(ari_inst.is_float){
        auto s_reg1_offset=ari_inst.real_operand[1].value;
        auto s_reg1=double(this->reg->reg_read(core_id,s_reg1_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=0.0f-s_reg1;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    else{
        auto s_reg1_offset=ari_inst.real_operand[1].value;
        auto s_reg1=long(this->reg->reg_read(core_id,s_reg1_offset));
        auto d_reg_offset=ari_inst.real_operand[0].value;
        auto d_reg=0-s_reg1;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
}
void single_core::mad_s(inst ari_inst){
    //if(ari_inst.options)
}
void single_core::mov_s(inst ari_inst){
    //if(ari_inst.options)
    auto d_reg_offset=ari_inst.real_operand[0].value;
    if(ari_inst.real_operand[1].op_t==0){//reg
        auto s_reg1_offset=ari_inst.real_operand[1].value;
        auto s_reg1=ulong(this->reg->reg_read(core_id,s_reg1_offset));
        auto d_reg=s_reg1;
        cout<<"mov "<<ari_inst.inst_operands[1]<<" to "<<ari_inst.inst_operands[0]<<" "<<s_reg1<<" to "<<d_reg_offset<<endl;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    if(ari_inst.real_operand[1].op_t==1){//imm
        //ari_inst.real_operand[1].value;
        auto s_reg1=ulong( ari_inst.real_operand[1].value);
        cout<<"imm "<<s_reg1<<" to "<<ari_inst.inst_operands[0]<<endl;
        auto d_reg=s_reg1;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }

}

void single_core::ld_s(inst ari_inst){
    //vector<string>
    auto d_reg_offset=ari_inst.real_operand[0].value;
    ulong value;
    if(ari_inst.real_operand[1].op_t==0){
        auto s_reg1_offset=ari_inst.real_operand[1].value;
        auto s_reg1=(ulong*)(this->reg->reg_read(core_id,s_reg1_offset));
        value=*s_reg1;
    }
    else{
        
        value=this->global_mem_t->mem_read(ari_inst.real_operand[1].value);
        
    } 
    this->reg->reg_write(core_id,d_reg_offset,ulong(value));
    cout<<"core "<<core_id<<" load "<<value<<" to "<<ari_inst.inst_operands[0]<<endl;
    // cout<<value<<endl;
    // cout<<ari_inst.inst_operands[1]<<endl;
}
void single_core::st_s(inst ari_inst){
    auto s_reg1_offset=ari_inst.real_operand[1].value;
    auto s_reg1=ulong(this->reg->reg_read(core_id,s_reg1_offset));
    auto d_reg_offset=ari_inst.real_operand[0].value;
    auto d_reg=(ulong*)(this->reg->reg_read(core_id,d_reg_offset));
    *d_reg=s_reg1;
    cout<<"st "<<ari_inst.inst_operands[1]<<" to "<<ari_inst.inst_operands[0]<<endl;
    cout<<d_reg<<endl;
    
}
void single_core::cvta_s(inst ari_inst){
    auto d_reg_offset=ari_inst.real_operand[0].value;
    auto s_reg1_offset=ari_inst.real_operand[1].value;
    auto s_reg1=ulong(this->reg->reg_read(core_id,s_reg1_offset));
    auto d_reg=s_reg1;
    cout<<"cvta "<<ari_inst.inst_operands[1]<<" to "<<ari_inst.inst_operands[0]<<" "<<s_reg1<<" to "<<d_reg_offset<<endl;
    this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
};
void single_core::setp_s(inst ari_inst){
    auto options=ari_inst.options;
    if(ari_inst.is_float){
        double s_reg1;
        if(ari_inst.real_operand[2].op_t==0){
            auto s_reg1_offset=ari_inst.real_operand[2].value;
            s_reg1=double(this->reg->reg_read(core_id,s_reg1_offset));
        }
        else{
            s_reg1=double(ari_inst.real_operand[2].value);
        }
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=double(this->reg->reg_read(core_id,s_reg2_offset));
        if(find(options.begin(),options.end(),"eq")!=options.end()){
            this->p=(s_reg2==s_reg1)?1:0;
        }
        if(find(options.begin(),options.end(),"gt")!=options.end()){
            this->p=(s_reg2>s_reg1)?1:0;
        }
        if(find(options.begin(),options.end(),"lt")!=options.end()){
            this->p=(s_reg2<s_reg1)?1:0;
        }
        cout<<"setp "<<s_reg2<<" "<<s_reg1<<endl;
        //auto d_reg=double(s_reg1*s_reg2);
        //this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    else{
        long s_reg1;
        if(ari_inst.real_operand[2].op_t==0){
            auto s_reg1_offset=ari_inst.real_operand[2].value;
            s_reg1=long(this->reg->reg_read(core_id,s_reg1_offset));
        }
        else{
            s_reg1=long(ari_inst.real_operand[2].value);
        }
        auto s_reg2_offset=ari_inst.real_operand[1].value;
        auto s_reg2=long(this->reg->reg_read(core_id,s_reg2_offset));
        if(find(options.begin(),options.end(),"eq")!=options.end()){
            this->p=(s_reg2==s_reg1)?1:0;
        }
        if(find(options.begin(),options.end(),"gt")!=options.end()){
            this->p=(s_reg2>s_reg1)?1:0;
        }
        if(find(options.begin(),options.end(),"lt")!=options.end()){
            this->p=(s_reg2<s_reg1)?1:0;
        }
        cout<<"setp "<<s_reg2<<" "<<s_reg1<<endl;
    }
}
void single_core::selp_s(inst ari_inst){
    auto options=ari_inst.options;
    auto d_reg_offset=ari_inst.real_operand[0].value;
    if(ari_inst.is_float){
        double s_reg1;
        double s_reg2;
        if(ari_inst.real_operand[2].op_t==0){
            auto s_reg1_offset=ari_inst.real_operand[2].value;
            s_reg1=double(this->reg->reg_read(core_id,s_reg1_offset));
        }
        else{
            s_reg1=double(ari_inst.real_operand[2].value);
        }
        if(ari_inst.real_operand[1].op_t==0){
            auto s_reg2_offset=ari_inst.real_operand[1].value;
            s_reg2=double(this->reg->reg_read(core_id,s_reg2_offset));
        }
        else{
            s_reg2=double(ari_inst.real_operand[1].value);
        }
        auto d_reg=this->p?s_reg2:s_reg1;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
    else{
        long s_reg1;
        long s_reg2;
        if(ari_inst.real_operand[2].op_t==0){
            auto s_reg1_offset=ari_inst.real_operand[2].value;
            s_reg1=long(this->reg->reg_read(core_id,s_reg1_offset));
        }
        else{
            s_reg1=long(ari_inst.real_operand[2].value);
        }
        if(ari_inst.real_operand[1].op_t==0){
            auto s_reg2_offset=ari_inst.real_operand[1].value;
            s_reg2=long(this->reg->reg_read(core_id,s_reg2_offset));
        }
        else{
            s_reg2=long(ari_inst.real_operand[1].value);
        }
        auto d_reg=this->p?s_reg2:s_reg1;
        this->reg->reg_write(core_id,d_reg_offset,ulong(d_reg));
    }
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
