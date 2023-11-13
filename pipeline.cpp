/*
valid /invalid bit se decide hoga ki voh cycle execute hoga ki nhi
cycles par hi loop chalayenge
and ulta cycle execute karenge
*/

#include<bits/stdc++.h>
using namespace std;
int PC=0;

unordered_map<string,string> instruction_type_decoding;
vector<int> register_file(32,0);
vector<int> main_memory(1024,0);

class Instruction{
    public:
    string binary;
    string opcode;
    string funct3;
    string funct7;
    int rsl1;
    int rsl2;
    int rdl;
    int immediate;
};

void initialize(){
    //opcode to type of instruction
    instruction_type_decoding["0110011"]="R";
    instruction_type_decoding["0010011"]="IA";
    instruction_type_decoding["0000011"]="IL";
    instruction_type_decoding["1100111"]="IJ";
    instruction_type_decoding["1100011"]="B";
    instruction_type_decoding["1101111"]="J";
    instruction_type_decoding["0100011"]="S";
    instruction_type_decoding["0110111"]="LUI";
    instruction_type_decoding["1101111"]="AUIPC";
}

int convert_signed_binary(string binary){
    int size=binary.size();
    int ans=0;
    int mul=1;
    if(binary[0]=='1')mul=-1;
    for(int i=size-1;i>=1;i--){
        if(binary[i]=='1'){
            ans+=pow(2,size-1-i);
        }
    }
    ans*=mul;
    return ans;

}

int convert_unsigned_binary(string binary){
    int size=binary.length();
    int ans=0;
    for(int i=size-1;i>=0;i--){
        if(binary[i]=='1'){
            ans+=pow(2,size-1-i);
        }
    }
    return ans;
}

//decoding
int get_rsl1(string line){
    string rs1;
    for(int i=12;i<=16;i++){
        rs1.push_back(line[i]);
    }
    int num=convert_unsigned_binary_string_to_int(rs1);
    return num;
}

int get_rsl2(string line){
    string rs2;
    for(int i=7;i<=11;i++){
        rs2.push_back(line[i]);
    }
    int num=convert_unsigned_binary_string_to_int(rs2);
    return num;
}

int get_rdl(string line){
    string rd;
    for(int i=20;i<=24;i++){
        rd.push_back(line[i]);
    }
    int num=convert_unsigned_binary_string_to_int(rd);
    return num;
}

string get_funct3(string binary){
    string funct3;
    for(int i=17;i<=19;i++){
        funct3.push_back(binary[i]);
    }
    return funct3;
}

string get_funct7(string binary){
    string funct7;
    for(int i=0;i<=6;i++){
        funct7.push_back(binary[i]);
    }
    return funct7;
}

int get_immediate_I(Instruction current){
    string imm;
    int num=0;
    if(current.funct3=="001" || current.funct3=="101"){
        for(int i=7;i<=11;i++){
            imm.push_back(binary[i]);
        }
        num=convert_unsigned_binary(imm);
    }
    else{
        for(int i=0;i<=11;i++){
            imm.push_back(binary[i]);
        }
        num=convert_signed_binary(imm);
    }
    
    return imm;
}

int get_immediate_S(string binary){
    string imm;
    for(int i=0;i<=6;i++){
        imm.push_back(binary[i]);
    }
    for(int i=20;i<=24;i++){
        imm.push_back(binary[i]);
    }
    return convert_signed_binary(imm);
}

int get_immediate_B(string binary){
    string imm;
    imm.push_back(binary[0]);
    imm.push_back(binary[24]);
    for(int i=1;i<=6;i++){
        imm.push_back(binary[i]);
    }
    for(int i=20;i<=23;i++){
        imm.push_back(binary[i]);
    }
    return convert_signed_binary(imm);
}

string get_opcode(string line){
    string opcode;
    for(int i=25;i<=31;i++){
        opcode.push_back(line[i]);
    }
    return opcode;
}

//tocomplete
int get_imm(string binary){
    if()
}

void decoder(Instruction &current){
    current.opcode=get_opcode(current.binary);
    current.rsl1=get_rsl1(current.binary);
    current.rsl2=get_rsl2(current.binary);
    current.rdl=get_rdl(current.binary);
    current.funct3=get_funct3;
    current.funct7=get_funct7;
    current.immediate=get_imm(current.binary);
}

int main(){
    initialize();
    vector<Instruction> instruction_memory;
    ifstream fin;
    fin.open("encoded_instruction.txt");
    string s;
    while(getline(fin,s)){
        Instruction* a=new Instruction();
        a->binary=s;
        instruction_memory.push_back((*a));
    }
    fin.close();
    for(auto i:instruction_memory){
        cout<<i.binary<<endl;
    }
    return 0;
}