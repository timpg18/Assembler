/*
notes-
I type instruction can have different opcodes
so for each
arithmetic - IA;
load - IL
jump - IJ
both type of instructions of U type are having diff op
so directly mappng with name
*/
#include<bits/stdc++.h>
#include"alu.cpp"
#include"encoding.cpp"
using namespace std;

unordered_map<string,string> instruction_type_decoding;
vector<int> register_file(32,5);

int convert_signed_binary(string binary){
    int size=binary.size();
    int ans=0;
    int mul=1;
    if(binary[0]=='1')mul=-1;
    for(int i=size-1;i>=0;i--){
        if(binary[i]=='1'){
            ans+=pow(2,size-1-i);
        }
    }
    ans*=mul;
    return ans;

}

int convert_unsigned_binary_string_to_int(string binary){
    int size=binary.length();
    int ans=0;
    for(int i=size-1;i>=0;i--){
        if(binary[i]=='1'){
            ans+=pow(2,size-1-i);
        }
    }
    return ans;
}

int get_rs1(string line){
    string rs1;
    for(int i=12;i<=16;i++){
        rs1.push_back(line[i]);
    }
    int num=convert_unsigned_binary_string_to_int(rs1);
    return num;
}

int get_rs2(string line){
    string rs2;
    for(int i=7;i<=11;i++){
        rs2.push_back(line[i]);
    }
    int num=convert_unsigned_binary_string_to_int(rs2);
    return num;
}

int get_rd(string line){
    string rd;
    for(int i=20;i<=24;i++){
        rd.push_back(line[i]);
    }
    int num=convert_unsigned_binary_string_to_int(rd);
    return num;
}

class Instruction{
    public:
    string binary;
    string type;
    string opcode;
    string funct3;
    string funct7;
    string immediate;
    string rs1;
    string rs2;
};

class control_word{
    public:
    bool ALUSRC;
    bool MemtoReg;
    bool Regwrite;
    bool MemRead;
    bool MemWrite;
    bool branch;
    bool ALUOP1;
    bool ALUOP2;

    control_word(Instruction temp){
        if(temp.type=="R"){
            inR();
        }
        if(temp.type=="IA"){
            inIA();
        }
        if(temp.type=="IL"){
            inIL();
        }
        if(temp.type=="B"){
            inB();
        }
        if(temp.type=="S"){
            inS();
        }
    }

    void inR(){
        ALUSRC = 0;
        MemtoReg = 0;
        Regwrite = 1;
        MemRead = 0;
        MemWrite = 0;
        branch = 0;
        ALUOP1 = 1;
        ALUOP2 = 0;
    }

    void inIA(){
        ALUSRC = 1;
        MemtoReg = 0;
        Regwrite = 0;
        MemRead = 0;
        MemWrite = 0;
        branch = 0;
        ALUOP1 = 1;
        ALUOP2 = 1;
    }

    void inIL(){
        ALUSRC = 1;
        MemtoReg = 1;
        Regwrite = 1;
        MemRead = 1;
        MemWrite = 0;
        branch = 0;
        ALUOP1 = 0;
        ALUOP2 = 0;
    }

    void inB(){
        ALUSRC = 0;
        MemtoReg = 0;
        Regwrite = 0;
        MemRead = 0;
        MemWrite = 0;
        branch = 1;
        ALUOP1 = 0;
        ALUOP2 = 1;
    }

    void inS(){
        ALUSRC = 1;
        MemtoReg = 0;
        Regwrite = 0;
        MemRead = 0;
        MemWrite = 0;
        branch = 1;
        ALUOP1 = 0;
        ALUOP2 = 1;
    }

    void print(){
        cout<<ALUSRC;
        cout<<MemtoReg;
        cout<<Regwrite;
        cout<<MemRead;
        cout<<MemWrite;
        cout<<branch;
        cout<<ALUOP1;
        cout<<ALUOP2;
        cout<<endl;
    }

};

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

string get_immediate(string binary){
    string imm;
    for(int i=0;i<=11;i++){
        imm.push_back(binary[i]);
    }
    return imm;
}

string get_immediate_S(string binary){
    string imm;
    for(int i=0;i<=6;i++){
        imm.push_back(binary[i]);
    }
    for(int i=20;i<=24;i++){
        imm.push_back(binary[i]);
    }
    return imm;
}

string get_immediate_B(string binary){
    string imm;
    imm.push_back(binary[0]);
    imm.push_back(binary[24]);
    for(int i=1;i<=6;i++){
        imm.push_back(binary[i]);
    }
    for(int i=20;i<=23;i++){
        imm.push_back(binary[i]);
    }
    return imm;
}

vector<string> decode(Instruction &temp){
    vector<string> funct;
    if(temp.type=="R"){
        temp.funct3=get_funct3(temp.binary);
        temp.funct7=get_funct7(temp.binary);
        funct.push_back(temp.funct3);
        funct.push_back(temp.funct7);
    }
    if(temp.type=="IA"){
        temp.immediate=get_immediate(temp.binary);
        temp.funct3=get_funct3(temp.binary);
        funct.push_back(temp.funct3);
    }
    if(temp.type=="IL"){
        temp.immediate=get_immediate(temp.binary);
        temp.funct3=get_funct3(temp.binary);
        funct.push_back(temp.funct3);
    }
    if(temp.type=="B"){
        temp.funct3=get_funct3(temp.binary);
        temp.immediate=get_immediate_B(temp.binary);
        funct.push_back(temp.funct3);
    }
    if(temp.type=="S"){
        temp.funct3=get_funct3(temp.binary);
        temp.immediate=get_immediate_S(temp.binary);
        funct.push_back(temp.funct3);
    }
        return funct;
}

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

string get_instruction_type(string opcode){
    return instruction_type_decoding[opcode];   
}

string get_opcode(string line){
    string opcode;
    for(int i=25;i<=31;i++){
        opcode.push_back(line[i]);
    }
    return opcode;
}

int ALU_Select(bool aluop1,bool aluop2,vector<string> &funct){
    int selected=0;
    if(aluop1==0 && aluop2==0){
        selected=2;
    }
    if(aluop1==1 && aluop2==0){
        string funct3=funct[0];
        string funct7=funct[1];
        if(funct3=="000" &&funct7[1]=='0'){
            selected=2;
        }
        if(funct3=="000" &&funct7[1]=='1'){
            selected=6;
        }
        if(funct3=="010"){
            selected=7;
        }
        if(funct3=="011"){
            selected=11;
        }
        if(funct3=="111"){
            selected=0;
        }
        if(funct3=="110"){
            selected=1;
        }
        if(funct3=="100"){
            selected=3;
        }
        if(funct3=="001"){
            selected=8;
        }
        if(funct3=="101" && funct7[1]=='0'){
            selected=4;
        }
        if(funct3=="101" &&funct7[1]=='1'){
            selected=5;
        }

    }
    if(aluop1==1 &&aluop2==1){
        string funct3=funct[0];
        if(funct3=="000"){
            selected=2;
        }
        if(funct3=="010"){
            selected=7;
        }
        if(funct3=="011"){
            selected=11;
        }
        if(funct3=="111"){
            selected=0;
        }
        if(funct3=="110"){
            selected=1;
        }
        if(funct3=="100"){
            selected=3;
        }
        if(funct3=="001"){
            selected=8;
        }
        if(funct3=="101"){
            selected=4;
        }
        if(funct3=="101"){
            selected=4;
        }
    }
    return selected;
}

int main(){
    initialize();
    encoding();
    ifstream fin;
    fin.open("encoded_instruction.txt");
    string line;
    register_file[6]=20;
    while(getline(fin,line)){
        string opcode=get_opcode(line);
        string instruction_type=get_instruction_type(opcode);
        Instruction temp;
        temp.binary=line;
        temp.opcode=opcode;
        temp.type=instruction_type;
        vector<string> funct=decode(temp);
        control_word curr(temp);
        int operation=ALU_Select(curr.ALUOP1,curr.ALUOP2,funct);
        int rd=get_rd(line);
        int rs1=get_rs1(line);
        int rs2=0;
        int output=0;
        if(instruction_type=="S" || instruction_type=="R") rs2=get_rs2(line);
        else if(instruction_type=="IA"){
            rs2=convert_signed_binary(temp.immediate);
        } 
        if(instruction_type=="S" || instruction_type=="R")
        output=alu(register_file[rs1],register_file[rs2],operation);
        if(instruction_type=="IA")output=alu(register_file[rs1],rs2,operation);
        register_file[rd]=output;
        cout<<register_file[rd]<<endl;


    }
    fin.close();

    //to write in the register file
    // ofstream fout;
    // fout.open("register_file.txt");


    return 0;
}