
#ifndef func_H
#define func_H
#include <string>
#include <vector>
#include <variant>
#include <fstream>
#include <iostream>
#include <map>
#include "./inst.h"
using namespace std;
class scanner{

};
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
        string func_name;
        vector<inst> inst_queue;
        vector<variable_txt> variables;
        vector<pair<string,string>> param;
        vector<string> ptx_txt;
        map<string,int> jump_point;
    public:
        func();
        ~func();
        void get_func_txt(ifstream &f_stream);
        void show_txt();
        void show_variable();
        void add_line(const string &line);
        void get_func_name();
        void get_param();
        void get_variable();
        void get_inst();
};
class cu_thread{
    int *pc;
    int reg_num=64;
    func *func_ctx;
    map<string,ulong> mem;
    public:
        cu_thread(){};
        ~cu_thread(){};
        void malloc_variable();
        void malloc_param();
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
class symbol_table{
    
};
class context{

};


#endif