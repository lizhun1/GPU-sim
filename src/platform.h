
#ifndef platform_H
#define platform_H
#include "hardware.h"
#include <any>
#include <stdarg.h>
#include <fstream>
class dim3{
    private:

    public:
        dim3(){};
        ~dim3(){};
};
class platform{
    private:
        int sm_number;
        scanner scanner_t;
        GPGPU gpu;
        context ctx;
    public:
        vector<func> funcs;
        platform();
        ~platform();
        void read_file_list();
        void read_a_ptx(const char * ptx_path);
        void scan_func();
        void create_context(int func_idx,dim3 cuda_dim,vector<any> real_param);
        void sim(string func_name,dim3 cuda_dim,int param_num,...);

};


#endif