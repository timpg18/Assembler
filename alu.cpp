#include<iostream>
using namespace std;

const int ALU_OP_AND =0;//0
const int ALU_OP_OR =1;//1
const int ALU_OP_XOR =3;//3
const int ALU_OP_ADD =2;//2
const int ALU_OP_SUB =6;//6
const int ALU_OP_SLT =7;//7
const int ALU_OP_SLTU =11;//11
const int ALU_OP_SLL =8;//8
const int ALU_OP_SRL =4;//4
const int ALU_OP_SRA =5;//5

int alu(int rs1,int rs2,int op){
    int out=0;
    switch(op){
        case ALU_OP_AND:
            out= rs1&rs2;
        break;
        case ALU_OP_OR:
            out= rs1|rs2;
        break;
        case ALU_OP_XOR:
            out= rs1^rs2;
        break;
        case ALU_OP_ADD:
            out= rs1+rs2;
        break;
        case ALU_OP_SUB:
            out= rs1-rs2;
        break;
        case ALU_OP_SLT:
            out= (rs1<rs2? 1:0);
        break;
        case ALU_OP_SLTU:
            out= rs1&rs2;//to write
        break;
        case ALU_OP_SLL:
            out= rs1<<rs2;
        break;
        case ALU_OP_SRL:
            out= rs1>>rs2;
        break;
        case ALU_OP_SRA:
            out= rs1&rs2;//to write
        break;
    }

    return out;
}