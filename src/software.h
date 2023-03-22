
#ifndef func_H
#define func_H
#include <string>
#include <vector>
#include <variant>
#include <fstream>
#include <iostream>
#include <map>
#include <tuple>
#include "./inst.h"
using namespace std;
class variable{
    string variable_name;
    string data_type;
};
class param{
    public:
        

};
struct variable_txt
{
    string variable_name;
    string variable_data_t;
    int vector_len;
    string state_space;
    string value;
};

class func{
    private:
        int kernel_para_num;
        vector<string> ptx_txt;
        map<string,uint> jump_point;
    public:
        vector<inst> inst_queue;
        string func_name;
        vector<variable_txt> variables;
        vector<pair<string,string>> param;
        func();
        ~func();
        void get_func_txt(ifstream &f_stream);
        void show_txt();
        void show_variable();
        void del_space();
        void add_line(const string &line);
        void get_func_name();
        void get_param();
        void get_variable();
        void get_inst();
};
class cu_thread{
    int *pc;
    int reg_num=64;
    int tid;
    func *func_ctx;
    map<string,ulong> mem;
    public:
        cu_thread(){};
        ~cu_thread(){};
        void malloc_variable();
        void malloc_param();
        void creat_context();
        void init_tid(int t);
        void run();
        

};
class warp{
    public:
        static const int warp_size=32;
        int pc=0;
        cu_thread  threads[warp_size];
        func *func_ctx;
        warp();
        ~warp();
        void run();
};

class context{
    public:
        symbol_table s_t;
        //warp


};


#endif