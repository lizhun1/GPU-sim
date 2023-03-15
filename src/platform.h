
#ifndef platform_H
#define platform_H
#include "hardware.h"
#include <fstream>

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
        void create_context();
        void sim();

};


#endif