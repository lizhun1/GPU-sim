#include "inst.h"
inst::inst(){};
inst::~inst(){};
vector<string> split_multi(const string &str,string const delims){
    size_t beg, pos = 0;
    vector<string> result;
    while ((beg = str.find_first_not_of(delims, pos)) != string::npos)
    {
        pos = str.find_first_of(delims, beg + 1);
        result.push_back(str.substr(beg, pos - beg));
    }
    return result;
};
void inst::get_inst_type(){
    if(only_int_lib.find(inst_opcode)!=only_int_lib.end()){
        inst_type.first=1;
        inst_type.second=only_int_lib[inst_opcode];
    }
    if(int_float_lib.find(inst_opcode)!=int_float_lib.end()){
        inst_type.first=2;
        inst_type.second=int_float_lib[inst_opcode];
    }
    if(only_float_lib.find(inst_opcode)!=only_float_lib.end()){
        inst_type.first=3;
        inst_type.second=only_float_lib[inst_opcode];
    }
    if(sel_and_com_lib.find(inst_opcode)!=sel_and_com_lib.end()){
        inst_type.first=4;
        inst_type.second=sel_and_com_lib[inst_opcode];
    }
    if(logic_and_shift_lib.find(inst_opcode)!=logic_and_shift_lib.end()){
        inst_type.first=5;
        inst_type.second=logic_and_shift_lib[inst_opcode];
    }
    if(ctrl_lib.find(inst_opcode)!=ctrl_lib.end()){
        inst_type.first=6;
        inst_type.second=ctrl_lib[inst_opcode];
    }
    if(data_move_and_convert_lib.find(inst_opcode)!=data_move_and_convert_lib.end()){
        inst_type.first=7;
        inst_type.second=data_move_and_convert_lib[inst_opcode];
    }
    if(parallel_lib.find(inst_opcode)!=parallel_lib.end()){
        inst_type.first=8;
        inst_type.second=parallel_lib[inst_opcode];
    }
}
void inst::trans(symbol_table s_t)
{
    //using namespace std;

    vector<string>::iterator it;
    it=find(this->options.begin(),this->options.end(),"f32");
    if(it!=this->options.end()) this->is_float=true;
    else this->is_float=false;
    it=find(this->options.begin(),this->options.end(),"f64");
    if(it!=this->options.end()) this->is_float=true;
    else this->is_float=false;


    for(const auto & op_str:this->inst_operands){
        operand op_tmp;
        auto op=op_str;
        //cout<<op<<endl;
        if(op.find("[")!=string::npos){
            op=split_multi(op_str,"[]").front();
        }
        //cout<<op<<endl;
        if(s_t.varible_table.find(op)!=s_t.varible_table.end())
        {
            op_tmp.op_t=reg;
            op_tmp.value=s_t.varible_table[op].second;
        }
        else
        {
            if(s_t.param_table.find(op)!=s_t.param_table.end())
            {
                op_tmp.op_t=mem;
                op_tmp.value=s_t.param_table[op].second;
            }
            else
            {
                if(s_t.jump_table.find(op)!=s_t.jump_table.end())
                {
                    op_tmp.op_t=pc;
                    op_tmp.value=s_t.jump_table[op];
                    //cout<<op<<endl;
                }
                else
                {
                    op_tmp.op_t=imm;
                    
                    if(op.front()=='0'&&op.find("f")!=string::npos){
                        op.erase(op.begin());
                        op.erase(op.begin());
                        
                        op_tmp.value=stoul(op,nullptr,16);
                        //cout<<op<<endl;
                    }
                    else{
                        op_tmp.value=stol(op);
                    }
                    
                }
            }
        }
        real_operand.push_back(op_tmp);
    }
}
// enum operand_type inst::opt(){
//     cout<<this->inst_opcode<<endl;
//     return reg;
// };
