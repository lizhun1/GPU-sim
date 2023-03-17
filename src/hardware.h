
#ifndef HW_H
#define HW_H
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include "software.h"
using namespace std;
class reg_file{
    public:
        ulong data[64][64+3];
        reg_file();
        ~reg_file();
        void reg_initial(ulong value);
        ulong read(int x,int y);
        void write(int x,int y);
};
class single_core{
    private:
        int *PC;
        reg_file *reg;
    public:
    single_core();
    ~single_core();
    void attach_reg(reg_file *r);
    void exec(arithmetic_inst ari_inst);

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
        inst read_inst(int pc);
};

class stream_processer{
    private:
        int PC=0;
        reg_file reg_t;
        int max_warp=2;
        inst_cache inst_cache_t;
        global_mem *global_mem_t;
        single_core core[32];
        load_store_unit ldst[32];
    public:
        stream_processer();
        ~stream_processer();
        void attach_mem(global_mem *mem);
};
class GPGPU{
    private:
        int SM_num=1;
        
        //inst_cache *inst_cache_t;
        vector<stream_processer> SM;
    public:
        global_mem global_mem_t;
        GPGPU();
        ~GPGPU();
        void gpu_load();
        void gpu_run();
};


#endif