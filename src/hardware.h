
#ifndef HW_H
#define HW_H
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include "software.h"
using namespace std;
class global_mem;
class reg_file{
    public:
        ulong data[64][64+3];
        reg_file();
        ~reg_file();
        void reg_initial(ulong value);
        ulong reg_read(uint x,uint y);
        void reg_write(uint x,uint y,ulong value);
};
class single_core{
    private:
        int *PC;
        reg_file *reg;
    public:
    global_mem *global_mem_t;
    uint core_id;
    bool p;
    bool q;
    single_core();
    ~single_core();
    void attach_reg(reg_file *r);
    void exec(inst ari_inst);
    void add_s(inst ari_inst);
    void sub_s(inst ari_inst);
    void mul_s(inst ari_inst);
    void div_s(inst ari_inst);
    void mad_s(inst ari_inst);
    void neg_s(inst ari_inst);
    void abs_s(inst ari_inst);
    void min_s(inst ari_inst);
    void max_s(inst ari_inst);
    void mov_s(inst ari_inst);
    void ld_s(inst ari_inst);
    void st_s(inst ari_inst);
    void cvta_s(inst ari_inst);
    void setp_s(inst ari_inst);
    void selp_s(inst ari_inst);

};
class global_mem{
    private:
        ulong mem[1024];
        int latency=20;
    public:
        global_mem();
        ~global_mem();
        ulong mem_read(uint addr);
        void mem_write(uint addr,ulong value);
        // void mem_show(){
        //     for(int i=0;i<10;)
        // }
};

class load_store_unit{
    private:
        global_mem *global_mem_t;
    public:
        load_store_unit();
        ~load_store_unit();
        void attach_mem(global_mem* mem);

};
class inst_cache{
    private:
        vector<inst> inst_queue;
    public:
        inst_cache();
        ~inst_cache();
        void load_inst(vector<inst> inst_q);
        inst read_inst(uint pc){return inst_queue[pc];};
};
struct mask
{
    bool m[32];
};

class stream_processer{
    public:
        uint main_PC=0;
        uint vice_PC=0;
        //bool p[32];
        global_mem *global_mem_t;
        single_core core[32];
        load_store_unit ldst[32];
        vector<inst> core_mailbox;
        vector<inst> ldst_mailbox;
        uint switch_warp=0;
        mutex core_mailbox_lock;
        vector<pair<uint,mask>> simt_stack;
        mask active_mask;
        //thread core_t;
        inst_cache inst_cache_t;
        reg_file reg_t;
        uint max_warp=2;
        uint warp_size=32;
        uint sid;
        stream_processer();
        ~stream_processer();
        void attach_mem(global_mem *mem);
        static void show_reg(stream_processer *me);
        static void inst_dispatch(stream_processer *me);
        static void run(stream_processer *me);
        static void core_run(stream_processer *me);
        static void ldst_run(stream_processer *me);
        static void warp_sch(stream_processer *me);
};
class GPGPU{
    private:
        static const int SM_num=1;
        //inst_cache *inst_cache_t;
        stream_processer SM[SM_num];
    public:
        mutex global_mem_lock;
        global_mem global_mem_t;
        GPGPU();
        ~GPGPU();
        void gpu_load(vector<inst> inst_q);
        void gpu_run();
        void prepare_tid(dim3 cuda_dim);
};


#endif