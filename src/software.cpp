#include "software.h"
func::func(){};
func::~func(){};
warp::warp(){};
warp::~warp(){};
void func::get_func_txt(ifstream &f_stream){
    string line;
    while(getline(f_stream,line)){
        if(line.find("ret;")==string::npos){
            //del all space line
            auto tokens=split_multi(line," \t");
            if(tokens.size()!=0){
                this->ptx_txt.push_back(line);
            }   
        }   
        else{
            this->ptx_txt.push_back(line);
            break;}     
    }
};
void func::show_variable(){
    for(uint i=0;i<this->variables.size();i++){
        std::cout<<variables[i].variable_name<<variables[i].variable_data_t<<std::endl;
    }
}
void func::show_txt(){
    for(uint i=0;i<this->ptx_txt.size();i++){
        std::cout<<this->ptx_txt[i]<<std::endl;
    }
}
void func::add_line(const string &line){
    //std::cout<<line<<std::endl;
    this->ptx_txt.push_back(line);
};
void func::get_func_name(){
    this->func_name=split_multi(this->ptx_txt.front()," (").back();
    this->ptx_txt.erase(ptx_txt.begin());
    //std::cout<<func_name<<std::endl;
};
void func::get_param(){
    pair<string,string> param_t;
    while(1){
        if(ptx_txt.front().find(")")!=string::npos)
        { 
            ptx_txt.erase(ptx_txt.begin());
            break;
        }
        else
        {
            auto tokens=split_multi(ptx_txt.front()," \t,.");
            param_t.first=tokens.back();//param name
            tokens.pop_back();
            param_t.second=tokens.back();//param type
            ptx_txt.erase(ptx_txt.begin());
            param.push_back(param_t);
            //cout<<param_t.second<<" "<<param_t.first<<endl;


        }
           
        
    }
    // ptx_txt.erase(ptx_txt.begin());
    // cout<<*ptx_txt.begin()<<"nihao"<<endl;
};
bool is_variable_statement(string line){
    auto tokens=split_multi(line,"\t ,;");
    if(tokens.size()==0) return false;
    if(tokens.front().front()=='.'){
        return true;
    }
    else return false;
}
bool is_inst(string line){
    auto tokens=split_multi(line,"\t. ;");
    if(tokens.size()==0) return false;
    if(all_inst.find(tokens[0])!=all_inst.end()||tokens[0].find('@')!=std::string::npos||tokens[0].find(':')!=std::string::npos){
        return true;
    }
    else return false;
};
void func::get_variable(){
    variable_txt variable_tmp;
    while (1)
    {   
        //cout<<ptx_txt.front()<<endl;
        if(ptx_txt.front().find("{")!=string::npos){
            ptx_txt.erase(ptx_txt.begin());
        }
        else
        {
            if(is_variable_statement(ptx_txt.front()))
            {   
                auto tokens=split_multi(ptx_txt.front(),"\t ,.;");
                variable_tmp.state_space=tokens.front();
                variable_tmp.variable_name=tokens.back();
                tokens.pop_back();
                variable_tmp.variable_data_t=tokens.back();
                variables.push_back(variable_tmp);
                ptx_txt.erase(ptx_txt.begin());
            }
            else 
            {
                break;
            }
        }

    }
    

};
void func::del_space(){

}
void func::get_inst(){
    
    for(uint i=0;i<ptx_txt.size();i++){
        inst inst_tmp;
        if(is_inst(ptx_txt[i]))
        {
            auto tokens=split_multi(ptx_txt[i],"\t ;,");
            //cout<<tokens.front()<<endl;
            //jump point
            if(tokens.front().find(":")!=string::npos){
                //cout<<"jump"<<endl;
                tokens.front().pop_back();
                this->jump_point[tokens.front()]=i;
                continue;

            }
            //pred var
            if(tokens.front().find("@")!=string::npos){
                //cout<<ptx_txt[i]<<endl;
                tokens.front().erase(tokens.front().begin());
                inst_tmp.is_pred=true;
                inst_tmp.pred=tokens.front();
                tokens.erase(tokens.begin());
            }
            //opcode
            auto opcode=tokens.front();
            auto tmp=split_multi(opcode,".");
            inst_tmp.inst_opcode=tmp.front();
            tmp.erase(tmp.begin());
            tokens.erase(tokens.begin());
            inst_tmp.options=tmp;
            inst_tmp.inst_operands=tokens;
            inst_tmp.get_inst_type();
            this->inst_queue.push_back(inst_tmp);
            //cout<<inst_tmp.options[0]<<endl;


        }
    }
};
void cu_thread::malloc_param(){
    //this->func_ctx;
};
void warp::run(){
    for(;;pc++){
        for(uint i=0;i<32;i++)
            this->threads[i].run();
    }
};
void cu_thread::init_tid(int t){
    tid=t;
};
void cu_thread::run(){
    try{

    }
    catch(int){

    }
};
void symbol_table::add_varible(string v_name,data_type v_type,uint offset){
    //offset=offset+sp_offset;
    pair<data_type,uint> pair_tmp;
    pair_tmp.first=v_type;
    pair_tmp.second=offset;
    this->varible_table[v_name]=pair_tmp;
};
void symbol_table::add_param(string p_name,data_type p_type,uint offset){
    pair<data_type,int> pair_tmp;
    pair_tmp.first=p_type;
    pair_tmp.second=offset;
    this->param_table[p_name]=pair_tmp;
};
void symbol_table::add_jump_point(string jump_point,uint pc){
    this->jump_table[jump_point]=pc;
};