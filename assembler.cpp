/*
#### to check
slli shamt encoding(not necessary)

#### remaining 


done-
R-type
I-type
S-typeB-type
J-type
U-type
label handling

labels must be named first and then start from next line and must not be space separated
considering tha if size of components vector is 1 , then it will be a label

Exit label does not have any instruction so need to check for exit ;

for I type 
slli ,srli,srai will have shamt so max shift is upto 31
*/
#include<bits/stdc++.h>
using namespace std;

unordered_map<string,char> instruction_type;
unordered_map<string,int> register_numbering;
unordered_map<string,int> labels;

void initialise(){
    //instruction mapping
    //R-Type
    instruction_type["add"]='R';
    instruction_type["sub"]='R';
    instruction_type["slt"]='R';
    instruction_type["sltu"]='R';
    instruction_type["and"]='R';
    instruction_type["or"]='R';
    instruction_type["xor"]='R';
    instruction_type["sll"]='R';
    instruction_type["srl"]='R';
    instruction_type["sra"]='R';

    //I-Type
    instruction_type["addi"]='I';
    instruction_type["slti"]='I';
    instruction_type["sltiu"]='I';
    instruction_type["andi"]='I';
    instruction_type["ori"]='I';
    instruction_type["xori"]='I';
    instruction_type["slli"]='I';
    instruction_type["srli"]='I';
    instruction_type["srai"]='I';
    instruction_type["ld"]='I';
    instruction_type["lw"]='I';
    instruction_type["lb"]='I';
    instruction_type["jalr"]='I';

    //S-Type
    instruction_type["sd"]='S';
    instruction_type["sw"]='S';
    instruction_type["sb"]='S';

    //B-Type
    instruction_type["beq"]='B';
    instruction_type["bne"]='B';
    instruction_type["bge"]='B';
    instruction_type["bgeu"]='B';
    instruction_type["blt"]='B';
    instruction_type["bltu"]='B';

    //J-Type
    instruction_type["jal"]='J';

    //U-type
    instruction_type["lui"]='U';
    instruction_type["auipc"]='U';

    //register mapping
    //by register naming
    register_numbering["zero"]=0;
    register_numbering["ra"]=1;
    register_numbering["sp"]=2;
    register_numbering["gp"]=3;
    register_numbering["tp"]=4;
    register_numbering["t0"]=5;
    register_numbering["t1"]=6;
    register_numbering["t2"]=7;
    register_numbering["s0"]=8;
    register_numbering["s1"]=9;
    register_numbering["a0"]=10;
    register_numbering["a1"]=11;
    register_numbering["a2"]=12;
    register_numbering["a3"]=13;
    register_numbering["a4"]=14;
    register_numbering["a5"]=15;
    register_numbering["a6"]=16;
    register_numbering["a7"]=17;
    register_numbering["s2"]=18;
    register_numbering["s3"]=19;
    register_numbering["s4"]=20;
    register_numbering["s5"]=21;
    register_numbering["s6"]=22;
    register_numbering["s7"]=23;
    register_numbering["s8"]=24;
    register_numbering["s9"]=25;
    register_numbering["s10"]=26;
    register_numbering["s11"]=27;
    register_numbering["t3"]=28;
    register_numbering["t4"]=29;
    register_numbering["t5"]=30;
    register_numbering["t6"]=31;

    //by register directly

    register_numbering["x0"] = 0;
    register_numbering["x1"] = 1;
    register_numbering["x2"] = 2;
    register_numbering["x3"] = 3;
    register_numbering["x4"] = 4;
    register_numbering["x5"] = 5;
    register_numbering["x6"] = 6;
    register_numbering["x7"] = 7;
    register_numbering["x8"] = 8;
    register_numbering["x9"] = 9;
    register_numbering["x10"] = 10;
    register_numbering["x11"] = 11;
    register_numbering["x12"] = 12;
    register_numbering["x13"] = 13;
    register_numbering["x14"] = 14;
    register_numbering["x15"] = 15;
    register_numbering["x16"] = 16;
    register_numbering["x17"] = 17;
    register_numbering["x18"] = 18;
    register_numbering["x19"] = 19;
    register_numbering["x20"] = 20;
    register_numbering["x21"] = 21;
    register_numbering["x22"] = 22;
    register_numbering["x23"] = 23;
    register_numbering["x24"] = 24;
    register_numbering["x25"] = 25;
    register_numbering["x26"] = 26;
    register_numbering["x27"] = 27;
    register_numbering["x28"] = 28;
    register_numbering["x29"] = 29;
    register_numbering["x30"] = 30;
    register_numbering["x31"] = 31;


}

vector<string> processing(string &input){
    vector<string> components;
    string temp;
    int n=input.size();
    for(int i=0;i<n;i++){
        if(input[i]!=' ' && input[i]!=',' && input[i]!='(' &&input[i]!=')' && input[i]!=':'){
            temp.push_back(input[i]);
        }
        else{
            int t=temp.size();
            if(t>0){
                components.push_back(temp);
            }
            temp.clear();
        }
        if(i==n-1){
            int t=temp.size();
            if(t>0){
                components.push_back(temp);
            }
            temp.clear();
        }

        if(input[i]=='#')break;
    }

    return components;
}

void print(vector<string> &components){
    for(auto i:components){
        cout<<i<<endl;
    }
}

string Encode_U_Type(string selected,vector<string> &components){
    string operation;
    string imm20;
    string rd;
    string opcode;

    string encoded;
    ifstream fin;
    fin.open("U-type.txt");
    while(fin>>operation>>imm20>>rd>>opcode){
        if(operation==selected){
            std::bitset<5> b(register_numbering[components[1]]);
            std::bitset<20> c(stoi(components[2]));
            rd=b.to_string();
            imm20=c.to_string();
            
            encoded = imm20 + rd + opcode;
            break;
        }
    }

    fin.close();
    return encoded;

}

string Encode_J_Type(string selected,vector<string> &components){
    string operation;
    string imm;
    string rd;
    string opcode;

    string encoded;
    ifstream fin;
    fin.open("J-type.txt");
    while(fin>>operation>>imm>>rd>>opcode){
        if(operation==selected){
            std::bitset<5> b(register_numbering[components[1]]);
            std::bitset<21> c(stoi(components[2]));
            rd=b.to_string();
            string immt=c.to_string();
            imm.clear();
            imm.push_back(immt[0]);
            for(int i=10;i<=19;i++){
                imm.push_back(immt[i]);
            }
            imm.push_back(immt[9]);
            for(int i=1;i<=8;i++){
                imm.push_back(immt[i]);
            }
            encoded=imm + rd + opcode;
            break;
        }
    }

    fin.close();
    return encoded;
}

string Encode_B_Type(string selected,vector<string> &components){
    string operation;
    string imm12;
    string rs2;
    string rs1;
    string imm105;
    string imm41;
    string imm11;
    string opcode;
    string funct3;
    
    string encoded;

    ifstream fin;
    fin.open("B-type.txt");
    while(fin>>operation>>imm12>>imm105>>rs2>>rs1>>funct3>>imm41>>imm11>>opcode){
        if(operation==selected){
            std::bitset<5> b(register_numbering[components[1]]);
            std::bitset<13> c(stoi(components[3]));
            std::bitset<5> d(register_numbering[components[2]]);
            rs2=d.to_string();
            rs1=b.to_string();
            string imm=c.to_string();
            imm12=imm[0];
            imm11=imm[1];
            imm105.clear();
            imm41.clear();
            for(int i=2;i<=7;i++){
                imm105.push_back(imm[i]);
            }
            for(int i=8;i<=11;i++){
                imm41.push_back(imm[i]);
            }
            encoded=imm12 +imm105+ rs2 + rs1 + funct3 + imm41 + imm11 + opcode;
            break;
        }
    }

    fin.close();
    return encoded;

}

string Encode_S_Type(string selected,vector<string> &components){
    string operation;
    string immediate1;
    string rs2;
    string rs1;
    string funct3;
    string immediate2;
    string opcode;

    string encoded;

    ifstream fin;
    fin.open("S-type.txt");
    while(fin>>operation>>immediate1>>rs2>>rs1>>funct3>>immediate2>>opcode){
        if(operation==selected){
            std::bitset<5> b(register_numbering[components[1]]);
            std::bitset<12> c(stoi(components[2]));
            std::bitset<5> d(register_numbering[components[3]]);
            rs2=b.to_string();
            rs1=d.to_string();
            string imm=c.to_string();
            immediate1.clear();
            immediate2.clear();
            for(int i=0;i<=6;i++){
                immediate1.push_back(imm[i]);
            }
            for(int i=7;i<=11;i++){
                immediate2.push_back(imm[i]);
            }
            encoded=immediate1 + rs2 + rs1 + funct3 + immediate2 + opcode;
            break;
        }
    }

    fin.close();
    return encoded;
}

string Encode_I_Type(string selected,vector<string> &components){
    string operation;
    string immediate;
    string rs1;
    string funct3;
    string rd;
    string opcode;

    string encoded;
    ifstream fin;
    fin.open("I-type.txt");
 
    while(fin>>operation>>immediate>>rs1>>funct3>>rd>>opcode){
        
        if(operation==selected){
            if(selected=="ld" || selected=="lw" ||selected=="lb" || selected=="jalr" ){
                std::bitset<5> b(register_numbering[components[1]]);
                rd=b.to_string();
                std::bitset<5> c(register_numbering[components[3]]);
                rs1=c.to_string();
                std::bitset<12> d(stoi(components[2]));
                immediate=d.to_string();
            }
            else if(selected=="srai"){
                std::bitset<5> b(register_numbering[components[1]]);
                rd=b.to_string();
                std::bitset<5> c(register_numbering[components[2]]);
                rs1=c.to_string();
                std::bitset<12> d(stoi(components[3]));
                immediate=d.to_string();
                immediate[1]='1';
            }
            else{
                std::bitset<5> b(register_numbering[components[1]]);
                rd=b.to_string();
                std::bitset<5> c(register_numbering[components[2]]);
                rs1=c.to_string();
                std::bitset<12> d(stoi(components[3]));
                immediate=d.to_string();
            }
            encoded= immediate + rs1 + funct3 + rd + opcode;
            break;
        }
    }
    
 
    // Close the file
    fin.close();
    return encoded;
    
}

string Encode_R_Type(string selected,vector<string> &components){
    string operation;
    string opcode;
    string funct7;
    string funct3;
    string rs1;
    string rs2;
    string rd;


    string encoded;
    ifstream fin;
    fin.open("R-type.txt");
 
    while(fin>>operation>>funct7>>rs2>>rs1>>funct3>>rd>>opcode){
        
        if(operation==selected){
            std::bitset<5> b(register_numbering[components[1]]);
            std::bitset<5> c(register_numbering[components[2]]);
            std::bitset<5> d(register_numbering[components[3]]);
            rd=b.to_string();
            rs1=c.to_string();
            rs2=d.to_string();
            encoded= funct7 + rs2 + rs1 + funct3 + rd + opcode;
            break;
        }
    }
    
 
    // Close the file
    fin.close();
    return encoded;
}

string label_processing(string label,int curr_inst){
    unordered_map<string,int> :: iterator it= labels.find(label);
    if(it!=labels.end()){
        int jump=labels[label];
        int eff=(jump-curr_inst)*4;
        label=to_string(eff);
        return label;
    }
    else return label;
}

int main(){
    
    initialise();
    string line,line2;
    ifstream fin;
    ofstream fout;

    // by default open mode = ios::in mode
    int instruction_number=1;
    fin.open("instruction.txt");
 
    // Execute a loop until EOF (End of File)
    while(getline(fin,line)){
        //cout<<line<<endl;
        vector<string> components=processing(line);
        //print(components);
        int size=components.size();
        if(size>1){
            instruction_number++;    
        }
        if(size==1){
            labels.insert({components[0],instruction_number});
        }
    }
    fin.close();

    fout.open("encoded_instruction.txt");

    // by default open mode = ios::in mode
    fin.open("instruction.txt");
    instruction_number=1;
    // Execute a loop until EOF (End of File)
    while(getline(fin,line)){
        //cout<<line<<endl;
        vector<string> components=processing(line);
        //print(components);
        int size=components.size();
        if(size>1){
            string operation=components[0];
            string label;
            vector<string> :: iterator it=components.end();
            it--;
            string encoded;
            transform(operation.begin(),operation.end(),operation.begin(),::tolower);
            char type_selection=instruction_type[operation];
            switch(type_selection){
                case 'R':
                    encoded=Encode_R_Type(operation,components);
                    break;

                case 'I':
                    encoded=Encode_I_Type(operation,components);
                    break;
                
                case 'S':
                    encoded=Encode_S_Type(operation,components);
                    break;
                
                case 'B':
                    label=(*it);
                    (*it)=label_processing(label,instruction_number);
                    cout<<(*it)<<endl;
                    encoded=Encode_B_Type(operation,components);
                    break;

                case 'J':
                    label=(*it);
                    (*it)=label_processing(label,instruction_number);
                    cout<<(*it)<<endl;
                    encoded=Encode_J_Type(operation,components);
                    break;

                case 'U':
                    encoded=Encode_U_Type(operation,components);
                    break;
            }
            
            //cout<<encoded<<endl;
            
            //writing in file
            fout<<encoded<<endl;
            instruction_number++;
            
        }
    }
    fout<<"00000000000000000000000000000000"<<endl;
    fout.close();
    // Close the file
    fin.close();


}