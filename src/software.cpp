#include "software.h"
func::func(){};
func::~func(){};
warp::warp(){};
warp::~warp(){};
void func::get_func_txt(ifstream &f_stream){
    string line;
    while(getline(f_stream,line)){
        if(line.find("ret;")==string::npos)
            this->ptx_txt.push_back(line);
        else{
            this->ptx_txt.push_back(line);
            break;}     
    }
};
void func::show_variable(){
    for(int i=0;i<this->variables.size();i++){
        std::cout<<variables[i].variable_name<<variables[i].variable_data_t<<std::endl;
    }
}
void func::show_txt(){
    for(int i=0;i<this->ptx_txt.size();i++){
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
            param_t.first=tokens.back();
            param_t.second=tokens[1];
            ptx_txt.erase(ptx_txt.begin());
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
    if(all_inst.find(tokens[0])!=all_inst.end()||tokens[0].find('@')!=std::string::npos){
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
void func::get_inst(){
    inst inst_tmp;
    for(int i=0;i<ptx_txt.size();i++){
        if(is_inst(ptx_txt[i]))
        {
            auto tokens=split_multi(ptx_txt[i],"\t ;,");
            //jump point
            if(tokens.front().find(":")!=string::npos){
                tokens.front().pop_back();
                this->jump_point[tokens.front()]=i;

            }
            //pred var
            if(tokens.front().find("@")!=string::npos){
                tokens.front().erase(tokens.front().begin());
                inst_tmp.pred=true;
                inst_tmp.pred=tokens.front();
                tokens.erase(tokens.begin());
            }
            //opcode



        }
    }
};
void cu_thead::malloc_param(){
    this->func_ctx;
};
void warp::run(){
    for(;;pc++){
        for(int i=0;i<32;i++)
            this->threads[i].run();
    }
};
void cu_thead::run(){};