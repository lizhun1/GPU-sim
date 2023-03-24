
#ifndef platform_H
#define platform_H
#include "hardware.h"
#include <any>
#include <stdarg.h>
#include <fstream>

class platform{
    private:
        int sm_number;
        
    public:
        GPGPU *gpu;
        context ctx;
        vector<func> funcs;
        platform();
        ~platform();
        void read_file_list();
        void init_hardware();
        void load_inst_cache(uint func_idx);
        void read_a_ptx(const char * ptx_path);
        void create_context(uint func_idx,dim3 cuda_dim,vector<any> real_param);
        void sim(string func_name,dim3 cuda_dim,uint param_num,...);

};


#endif