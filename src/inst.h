_Pragma("once")
#ifndef inst_H
#define inst_H
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <thread>
#include <cstring>
#include <iterator>
#include <algorithm>
//#include "software.h"
using namespace std;
typedef std::map<std::string,int> str_int_dict;
typedef std::map<std::string,std::string> str_str_dict;
static str_int_dict all_inst{
    {"bfi",1},
    {"rem",1},
    {"madc",1},
    {"add",2},
    {"sub",2},
    {"mul",2},
    {"div",2},
    {"mad",2},
    {"neg",2},
    {"abs",2},
    {"min",2},
    {"max",2},
    {"fma",3},
    {"sqrt",3},
    {"ex2",3},
    {"rcp",3},
    {"setp",4},
    {"selp",4},
    {"and",5},
    {"or",5},
    {"xor",5},
    {"not",5},
    {"shl",5},
    {"shr",5},
    {"bra",6},
    {"ret",6},
    {"mov",7},
    {"ld",7},
    {"st",7},
    {"cvta",7},
    {"cvt",7},
    {"bar",8},
    {"atom",8},
};
static str_str_dict data_type_lib{
    {"b32","uint"},
    {"b64","ulong"},
    {"u32","uint"},
    {"u64","ulong"},
    {"s32","int"},
    {"s64","long"},
    {"f32","float"},
    {"f64","double"},
    {"pred","bool"},
};
static str_int_dict data_type_lib_int{
    {"b32",0+1},
    {"b64",1+1},
    {"u32",2+1},
    {"u64",3+1},
    {"s32",4+1},
    {"s64",5+1},
    {"f32",6+1},
    {"f64",7+1},
    {"pred",8+1},
};
enum data_type{uint_g,ulong_g,int_g,long_g,float_g,double_g,bool_g};
static str_int_dict varible_lib{
    {"reg",1},
    {"sreg",2},
    {"const",3},
    {"global",4},
    {"local",5},
    {"param",6},
    {"shared",7},
};
static str_int_dict only_int_lib{
    {"bfi",1},
    {"rem",2},
    {"madc",3},
};
static str_int_dict int_float_lib{
    {"add",1},
    {"sub",2},
    {"mul",3},
    {"div",4},
    {"mad",5},
    {"neg",6},
    {"abs",7},
    {"min",8},
    {"max",9},
};
static str_int_dict only_float_lib{
    {"fma",1},
    {"sqrt",2},
    {"ex2",3},
    {"rcp",4},
};
static str_int_dict sel_and_com_lib{
    {"setp",1},
    {"selp",2},
};
static str_int_dict logic_and_shift_lib{
    {"and",1},
    {"or",2},
    {"xor",3},
    {"not",4},
    {"shl",5},
    {"shr",6},
};
static str_int_dict ctrl_lib{
    {"bra",1},
    {"ret",2},
};
static str_int_dict data_move_and_convert_lib{
    {"mov",1},
    {"ld",2},
    {"st",3},
    {"cvta",4},
    {"cvt",5},
};
static str_int_dict parallel_lib{
    {"bar",1},
    {"atom",2},

};
static std::map<int,str_int_dict> inst_category{
    {1,only_int_lib},
    {2,int_float_lib},
    {3,only_float_lib},
    {4,sel_and_com_lib},
    {5,logic_and_shift_lib},
    {6,ctrl_lib},
    {7,data_move_and_convert_lib},
    {8,parallel_lib}
};
//str_int_dict all_inst;
class CException
{
public:
    string msg;
    CException(string s) : msg(s) {}
};
class dim3{
    private:

    public:
        int x;
        dim3(){};
        ~dim3(){};
};
class symbol_table{
    private:
        int varible_num=0;
        int sp_offset=3;
        
    public:
        map<string,pair<data_type,uint>> varible_table;
        map<string,pair<data_type,uint>> param_table;
        map<string,uint> jump_table;
        symbol_table(){
            add_varible("%tid.x",(enum data_type)1,0);
        };
        ~symbol_table(){};
        void add_varible(string v_name,data_type v_type,uint offset);
        void add_param(string p_name,data_type p_type,uint offset);
        void add_jump_point(string jump_point,uint pc);
};
vector<string> split_multi(const string &str,string const delims);
enum operand_type {reg,imm,mem,pc};
struct operand{
    enum operand_type op_t;
    ulong value;
};
class inst{
    public:
        string pred;
        bool is_pred=false;
        bool is_float;
        uint data_width;
        pair<int,int> inst_type;
        string inst_opcode;
        vector<string> options;
        vector<string> inst_operands;
        vector<operand> real_operand;
        inst();
        ~inst();
        void get_inst_type();
        void trans(symbol_table s_t);
        //enum operand_type opt();
};
class arithmetic_inst:inst{
    
};


#endif