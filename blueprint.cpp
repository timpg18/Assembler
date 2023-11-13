#include<bits/stdc++.h>
using namespace std;

//to remove confusion using r in the start of naming for register data
//  IF/ID register set
string r_curr_instruction;
string r_curr_opcode;

//  ID/EX register set
int r_rdl;
int r_rs1;
int r_rs2;
int r_imm;
int r_store_data;
int r_ALUSelect;

    // Jump and Branch Address
int JA;
int BA;

    // Control Word elements
bool CW_Branch;
bool CW_Jump;
bool CW_Regwrite;
bool CW_Mem2reg;
bool CW_Memread;
bool CW_Memwrite;
bool CW_ALUdo;
bool CW_Memdo;
bool CW_Writebackdo;
bool CW_End;

//  EX/MEM register set
int R_ALUOutputReg;
int R_MEMoutdata;
int R_writeBackData;
int R_memWriteData1;
int R_regDst1;
int R_regDst2;

//  MEM/WB register set

bool zeroFlagGenerated;
bool R_memDo1;
bool R_writeBackDo1;
bool R_regWrite1;
bool R_memRead1;   
bool R_memToReg1;
bool R_memWrite1;
bool R_end1;
bool R_writeBackDo2;
bool R_regWrite2;
bool R_end2;

// Extra Flags
bool ZeroFlag;
bool BranchPossible;
bool JumpFlag;
bool EndFlag;
//lets create to flag that indicates whether the program has ended or not
bool Program_ended;
int Clock;
bool finito;

unordered_map<string,char> opcode_type;
int register_file[32];
int main_memory[1024];

void initialize_opcode(){
    opcode_type["0110011"]='R';
    opcode_type["0010011"]='I';
    opcode_type["0100011"]='S';
    opcode_type["1100011"]='B';
    opcode_type["1101111"]='J';
    opcode_type["0000011"]='L';
}

// string get_opcode(string binary){
//     return binary.substr(25,7);
// }

// since only using add, sub instructions so the funct bits of R type is only useful. 
/*      funct3    funct7
add -    000      0000000
sub -    000      0100000
*/
// only addi will be used in the program.


//Each part of the pipeline
/*
In a pipeline, in all the stages of a pipeline, all the components will take some values from the previous registers and send the values to the next register

So each unit will have a function to take register data. do the computation and give the values to the next set of registers
*/

//functions to convert signed and unsigned binary
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

class Fetch{
    private:
        int PC=0;
        string Instruction;
        vector<string> Machine_Code;

    public:
        Fetch(vector<string> Instr){
            Machine_code=Instr;
        }

        int get_PC(){
            return PC;
        }

        void set_PC(int k){
            PC=k;
        }

};

class Jump_Branch_EA{
    public:
        void jump_address(string opcode,string instr){
            if(opcode_type[opcode]=='J'){
                //calculate jump address
            }
        }
        void branch_address(string opcode,string instr){
            if(opcode_type[opcode]=='B'){
                //calculate branch address
            }
        }
};

class Control_Unit{
    private:
        string CU_opcode;//current opcode
        string CU_instruction;//current instruction
        bool Work_condition;//whether control unit should compute
        Jump_Branch_EA EA_calculator;

    public:
        Control_Unit(){
            Work_condition=0;
        }

        void take_previous_register_data(){
            if(!r_curr_opcode.empty()){
                Work_condition=1;
                CU_opcode=r_curr_opcode;
                CU_instruction=r_curr_instruction;
                r_curr_opcode.clear();
                r_curr_instruction.clear();
                //clearing value from register 
                EA_calculator.jump_address(CU_opcode);
                EA_calculator.branch_address(CU_opcode);
            }
        }
        //making function to calculate each control word bit
        int destination_reg(){
            if(opcode_type[CU_opcode]=='L'||opcode_type[CU_opcode]=='R'||opcode_type[CU_opcode]=='I'){
                string rdl=CU_instruction.substr(20,5);
                int register_num=convert_unsigned_binary(rdl);
                return register_num;
            }
            else return -1;
        }

        bool Branch(){
            if(opcode_type[CU_opcode]=='B') return 1;
            else return 0;
        }

        bool MemRead(){
            if(opcode_type[CU_opcode]=='L') return 1;
            else return 0;
        }

        bool Mem2Reg(){
            if(opcode_type[CU_opcode]=='L') return 1;
            else return 0;
        }

        bool MemWrite(){
            if(opcode_type[CU_opcode]=='S') return 1;
            else return 0;
        }

        bool Jump(){
            if(opcode_type[CU_opcode]=='J') return 1;
            else return 0;
        }

        //functions for ALUInput1 and 2 needs to be created

        bool RegWrite(){
            if(opcode_type[CU_opcode]=='L'||opcode_type[CU_opcode]=='R'||opcode_type[CU_opcode]=='I'){
                return 1;
            }
            else return 0;
        }

        int store_data(){
            string rsl2=CU_instruction.substr(7,5);
            int register_num=convert_unsigned_binary(rsl2);
            return register_file[rsl2];
        }

        int ALUcontrol(){
            // the ALU control word decides which operation to perform
            /*
            0: add
            1: sub
            2: branch
            3: jump / end
            */
           if(opcode_type[CU_opcode]=='B')return 2;
           else if(opcode_type[CU_opcode]=='L'||opcode_type[CU_opcode]=='S') return 0;
           else if(opcode_type[CU_opcode]=='R'){
                if(CU_instruction.substr(17,3)=="000" && CU_instruction[1]=='0')return 0;
                else if(CU_instruction.substr(17,3)=="000" && CU_instruction[1]=='1')return 1;
           }
           else if(opcode_type[CU_opcode]=='I'){
                return 0;
           }
           else return 3;

        }

        //the following bools checks if the particular unit has to work

        bool ALUdo(){
            if(opcode_type[CU_opcode]=='J'||CU_opcode=="1111111") return 0;
            else return 1;
        }

        bool Memdo(){
            if(opcode_type[CU_opcode]=='L'||opcode_type[CU_opcode]=='S') return 1;
            else return 0;
        }

        bool Writebackdo(){
            if(opcode_type[CU_opcode]=='R'||opcode_type[CU_opcode]=='I'||opcode_type[CU_opcode]=='L') return 1;
            else return 0;
        }

        //end is when the instruction consists of all 1
        bool END(){
            if(CU_instruction=="00000000000000000000000000000000") return 1;
            else return 0;
        }

        int get_rsl1(){
            string rs1=CU_instruction.substr(12,5);
            int num=convert_unsigned_binary(rs1);
            return num;
        }

        int get_rsl2(){
            string rs2=CU_instruction.substr(7,5);
            int num=convert_unsigned_binary(rs2);
            return num;
        }

        void forward_data_to_register(){
            if(Work_condition==1){
                // giving the ID/EX, register the data 
                r_rdl=destination_reg();
                r_rs1=register_file[get_rsl1()];
                r_rs2=register_file[get_rsl2()];
                r_imm=;//calculate imm
                r_store_data=store_data();
                r_ALUSelect=ALUcontrol();
                CW_Branch=Branch();
                CW_Jump=Jump();
                CW_Regwrite=RegWrite();
                CW_Mem2reg=Mem2Reg();
                CW_Memread=MemRead();
                CW_Memwrite=MemWrite();
                CW_ALUdo=ALUdo();
                CW_Memdo=Memdo();
                CW_Writebackdo=Writebackdo();
                CW_End=END();
                Work_condition=0;
            }
        }


};

class Execute{
    private:
        int ALUInput1;
        int ALUInput2;
        bool ALUdo;
        bool ALUBranch;
        bool ALUMemRead;
        bool ALUMemToReg; 
        bool ALUMemWrite;
        bool ALURegWrite;
        bool ALUMemDo;
        bool ALUWriteBackDo;
        int passDataForSt;
        int regAddress;
        int ALUControl;
        bool end;

    public:
        void take_previous_register_data(){
            ALUInput1=;//to be calculated
            ALUInput2=;//to be calculated
            ALUdo=CW_ALUdo;
            ALUBranch=CW_Branch;
            ALUMemRead=CW_Memread;
            ALUMemToReg=CW_Mem2reg; 
            ALUMemWrite=CW_Memwrite;
            ALURegWrite=CW_Regwrite;
            ALUMemDo=CW_Memdo;
            ALUWriteBackDo=CW_Writebackdo;
            passDataForSt=r_store_data;
            regAddress=r_rdl;
            ALUControl=r_ALUSelect;
            end=CW_End;
            //flag generation
            if(ALUdo){
                //branch flag generation
                // if(ALUInput1-ALUInput2==0 && ALUBranch){
                //     ZeroFlag = true;
                // }
                // if(ALUBranch){
                //     zeroFlagGenerated = true;
                // }
                CW_ALUdo = false;
            }
        }

        void forward_data_to_register(){
            if(ALUdo){
                if(ALUControl==0){
                    R_ALUOutputReg= ALUInput1+ALUInput2;//add
                }
                else if(ALUControl==1){
                    R_ALUOutputReg= ALUInput1-ALUInput2;//sub
                }
            }

            R_memRead1 = ALUMemRead; 
            R_writeBackDo1 = ALUWriteBackDo;
            R_memDo1 = ALUMemDo;
            R_regWrite1 = ALURegWrite;
            R_memWrite1 = ALUMemWrite;
            R_memToReg1 = ALUMemToReg;
            R_memWriteData1 = passDataForSt;
            R_regDst1 = regAddress;
            R_end1 = end;
        }
        
};

class Memory{
    private:
        bool memRead;
        bool memWrite;
        bool memDo;
        bool regWrite; 
        bool writeBackDo;
        int ALUOutput;
        int dataToBeStored;
        int readData;
        int regDst;
        bool end;
    public:
        void takeFromRegister(){ 
            //takes the necessary data from EX/MEM registers
            memWrite = R_memWrite1;
            memDo = R_memDo1;
            ALUOutput = R_ALUOutputReg;
            dataToBeStored = R_memWriteData1;
            memRead = R_memRead1;
            writeBackDo = R_writeBackDo1;
            regWrite = R_regWrite1;
            regDst = R_regDst1;
            end=R_end1;
            if(memWrite==1 && memDo==1) { 
                MEM[ALUOutput] = dataToBeStored;
                R_MemDo=false;  
            }
        }

        void giveToRegister(){
            if(memRead==1 && memDo==1){
                readData = MEM[ALUOutput];
            }
            else if(memRead==0 && memDo==0){//by-pass
                readData = ALUOutput;
            }
            R_writeBackData = readData;//data to write
            R_writeBackDo2 = writeBackDo;//wb on/off state
            R_regDst2 = regDst;//where to write
            R_end2=end;
        }
};

class Writeback{
  private:
    int dataToBeWritten;
    int regDst;
    bool writeDo;
    bool end;
  public:
    void takeFromRegister(){
        //taking the necessary data from MEM/WB registers
        writeDo = R_writeBackDo2;
        regDst = R_regDst2;
        if(writeDo){
            dataToBeWritten = R_writeBackData;
            CW_Writebackdo = false;
        }
        end=R_end2;
    }

    void writeBackToRegister(){
        if(writeDo==1){
            R[regDst] = dataToBeWritten;
        }
        finito=end;
    }
};