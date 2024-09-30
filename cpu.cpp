#include <bits/stdc++.h>
using namespace std;

map<string, int> GPR;
map<string, int> MEMORY;
map<string, string> Operation = {
    {"0000", "AND"},
    {"0001", "OR"},
    {"0010", "ADD"},
    {"0110", "SUB"},
    {"1111", "MUL"},
};

string get_type(string &opcode)
{
    // string AluOp=opcode.substr(0,2);
    if (opcode == "0100011")
    {
        return "S";
    }
    else if (opcode == "1100011")
    {    
        return "B";
    }
    else if (opcode == "0110011")
    {
        return "R";
    }
    else if (opcode == "0010011")
    {
        return "I";
    }
    else if (opcode == "0000011")
    {
        return "Ld";
    }
    else if (opcode == "1101111")
    {
        return "U";
    }
}
void Read_GPR(){
    cout<<"HII i am reading --"<<endl;
    for(auto it:GPR){
        cout<<it.first<<"---"<<it.second<<endl;
    }
}
string Immediate_Generator(string &instruction,string &type)
{ 
    if(type=="I"||type=="Ld"){
     return instruction.substr(0,12);
    }else if(type=="S"){
     return instruction.substr(0,7)+instruction.substr(20, 5);
    }else if(type=="B"){
     string imm = instruction.substr(0, 1);      // imm[12]
        imm += instruction.substr(24, 1);             // imm[11]
        imm += instruction.substr(1, 5);            // imm[10:5]
        imm += instruction.substr(20, 4);
        imm+="0";          // imm[4:1]
         cout<<"B - type--"<<imm<<endl;
        return imm;
    }else if(type=="U"){
         string imm= instruction.substr(0, 1); // imm[20]
         imm = instruction.substr(12, 8);      // imm[19:12]
        imm += instruction.substr(11, 1);            // imm[11]
        imm += instruction.substr(1, 10);   
        imm+="0";        // imm[10:1]
        return imm;
    }

    // imm.insert(0, 32 - imm.size(), '0');
};

class Instruction_Memory
{
public:
    // string intruction;
    string funct7, RSL2, RSL1, funct3, Immediate, Rd, opcode;
    Instruction_Memory(string &intruction)
    {
        this->funct7 = intruction.substr(0, 7);
        this->RSL2 = intruction.substr(7, 5);
        this->RSL1 = intruction.substr(12, 5);
        this->funct3 = intruction.substr(17, 3);
       // decode it
        this->Rd = intruction.substr(20, 5);
        this->opcode = intruction.substr(25, 7);
        string type=get_type(this->opcode);
         this->Immediate = Immediate_Generator(intruction,type); 
    }
};

class Register_Memory
{
public:
    string intruction;
    int read_RS1(string &rsl1)
    {
        return GPR[rsl1];
    }
    int read_RS2(string &rsl2)
    {
        return GPR[rsl2];
    }
    // write at rd
    void write_RD(string &RD, int value)
    {
        GPR[RD] = value;
    }
};

// bit to num


string Alu_Control(string &Alu_op, string &func7, string &func3)
{
    if (Alu_op == "00")
    {
        return "0010";
    }
    if (Alu_op == "01")
    {
        return "0110";
    }

    if (func7 == "0000000" && func3 == "000")
    {
        return "0010";
    }
    if (func7 == "0100000" && func3 == "000")
    {
        return "0110";
    }
    if (func7 == "0000000" && func3 == "111")
    {
        return "0000";
    }
    if (func7 == "0000000" && func3 == "110")
    {
        return "0001";
    }
    if (func7 == "1000000" && func3 == "000")
    {
        return "1111";
    }
}

int Alu(string &Alu_Select, int &Input_Data1, int &Input_Data2)
{
    //    return output;
    string operation = Operation[Alu_Select];
    if (operation == "ADD")
    {
        return Input_Data1 + Input_Data2;
    }
    else if (operation == "AND")
    {
        return Input_Data1 & Input_Data2;
    }
    else if (operation == "OR")
    {
        return Input_Data1 | Input_Data2;
    }
    else if (operation == "SUB")
    {
        return Input_Data1 - Input_Data2;
    }
    else if (operation == "MUL")
    {
        return Input_Data1 * Input_Data2;
    }
    return 0;
}

class Data_Memory
{
public:
    int Mem_Read(string &EA)
    {
        return MEMORY[EA];
    }
    void Mem_Write(string &EA, int value)
    {
        MEMORY[EA] = value;
    }
};

class ControlUnit
{
public:
    int AluSrc, Mem2Reg, RegWrite, MemRead, MemWrite, Branch, AluOp1, AluOp0, jump;

    ControlUnit(const string &type)
    {
        AluSrc = (type == "S" || type == "I"||type=="Ld") ? 1 : 0;
        Mem2Reg = (type == "Ld") ? 1 : 0;
        RegWrite = (type == "R" || type == "I"||type=="Ld") ? 1 : 0;
        MemWrite = (type == "S") ? 1 : 0;
        Branch = (type == "B") ? 1 : 0;
        AluOp1 = (type == "R"||type=="I") ? 1 : 0;
        AluOp0 = (type == "B") ? 1 : 0;
        jump = (type == "U") ? 1 : 0;
    }
};



class Processor
{
public:
    string Intruction;
    string PC;
    string TPC;

    Processor(string &Instruction, string &PC)
    {
        this->Intruction = Instruction;
        this->PC = PC;
    }
    void Run()
    {
        // Instruction fetch
        cout<<"Running"<<endl;
        Instruction_Memory Im(Intruction);
        string type = get_type(Im.opcode);
        cout << "Type of Instruction--> " << type << endl;
        // CU initalize
        ControlUnit CU(type);
        // Register memory
        Register_Memory Rg;
        int Rs2, Rs1; // for ALU input
        Rs1 = Rg.read_RS1(Im.RSL1);
        if (CU.AluSrc)
        {
            Rs2 = stoll(Im.Immediate, nullptr, 2);
        }
        else
        {
            Rs2 = Rg.read_RS2(Im.RSL2);
        }
       
       if(type=="Ld"||type=="S"){ // register no
        Rs1=stoll(Im.RSL1,nullptr,2);
       }
        cout << "Rs2  " << Rs2 << "Rs1  " << Rs1 << endl;
        string Alu_op = to_string(CU.AluOp1) + to_string(CU.AluOp0);
        string Alu_Select = Alu_Control(Alu_op, Im.funct7, Im.funct3);

        // Alu Caculation
        cout<<"Alu_Select"<<Alu_Select<<"-- "<<Operation[Alu_Select]<<endl;;
        int Alu_Output = Alu(Alu_Select, Rs2, Rs1);

        cout << "Alu Output  " << Alu_Output << endl;

        int flag = (CU.Branch && Alu_Output == 0) ? 1 : 0;

        Data_Memory Dm;
        int LD_result;
        if (CU.MemRead)
        {
            bitset<32> bitset(Alu_Output);
            string EA = bitset.to_string();
            LD_result = Dm.Mem_Read(EA);
            cout<<"LOAD Result--"<<LD_result<<endl;
        }

        if (CU.Mem2Reg)
        {
            Rg.write_RD(Im.Rd, LD_result);
        }
        else
        {   cout<<"Oh i am writing "<<endl;
            if (CU.RegWrite)
                Rg.write_RD(Im.Rd, Alu_Output);
        }

        if (CU.MemWrite)
        {
            bitset<32> bitset(Alu_Output);
            string EA = bitset.to_string();
            Rs2 = Rg.read_RS2(Im.RSL2);
            cout<<"Writing--"<<Rs2<<endl;
            Dm.Mem_Write(EA, Rs2);
        }

        // Instruction decode
        // Rs2=stoll(Im.Immediate,nullptr,2);
        int NPC_INT = stoll(PC, nullptr, 2) + 4;
        bitset<32> bitsetNPC(NPC_INT);
        string NPC = bitsetNPC.to_string();
        if(type=="R"){
            cout<<"Update--RK"<<endl;
            cout<<NPC<<endl;
            TPC=NPC;
        }else{
        string imm = Im.Immediate;
        int JPC_INT = stoll(imm, nullptr, 2);
        bitset<32> bitsetJPC(JPC_INT);
        string JPC = bitsetJPC.to_string();
        
        int imme = stoll(Im.Immediate, nullptr, 2);
        cout<<"IMMEd--"<<imme<<endl;
        int pc = stoll(PC, nullptr, 2);
        int BPC_INT = imme + pc;
        cout<<"BPC-- "<<BPC_INT<<endl;
        bitset<32> bitsetBPC(BPC_INT);
        string BPC = bitsetBPC.to_string();

        if (CU.jump)
        {   cout<<"hey jump Instruction--"<<JPC<<endl;
            TPC = JPC;
        }
        else if (CU.Branch && Alu_Output == 0)
        {
            TPC = BPC;
        }
        else
        {
            TPC = NPC;
        }
        }
       
    }
};

string binary(int a)
{
    bitset<32> bitset(a);
    return bitset.to_string();
}

void Test_Run(vector<string> &machine_Code)
{
    map<string, string> instruction_Set;
    int n = machine_Code.size();
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        string pc = binary(count);
        instruction_Set[pc] = machine_Code[i];
        count += 4;
    }

    string pc = binary(0);
    cout<<"Program Counter--"<<pc<<endl;
    while (true)
    {
        Processor pr(instruction_Set[pc], pc);
        pr.Run();
        string nextPc = pr.TPC;
        cout<<"NextPC--"<<nextPc<<endl;
        if (instruction_Set.find(nextPc) == instruction_Set.end())
        {
            break;
        }
        else
        {
            pc = nextPc;
        }
    }
}

void init(){
    // GPR["01001"]=4;
    // GPR["00100"]=6;
    // GPR["01000"]=5;
    // GPR["00110"]=10;
}
void DM_init(){
    MEMORY[binary(5)]=1;
    MEMORY[binary(6)]=6;
}

int main()
{
    vector<string> machine_Code = {
       "00000000000000101011001000000011",  // x4 =1
        "00000000000000101011000100000011", // x2=1
        "00000000000000110011000110000011", // x3=5
        "00000000001100010000100001100011", // beq x2 x3 16
        "10000000001000100000001000110011", // mul x4 x4 x2 
         "00000000000100010000000100010011",// x2 =x2+ 1
         "00000000110000000000000101101111",// jump to lopp
         "00000000010000000011001000100011"// sd value 
                 // "00000000100100100000000110110011", // Add more machine code instructions here
        // "00000000100101000110001100010011" 
        // "00000000010001001000010001100011"
        // "00000000000000000000000101101111"
        // "10000000100100100000000110110011"
        // "00000000010000100011001010000011"
        //  00000000100101000110001100010011 
    };
     cout<<"Intially GPR"<<endl;
     init();
     DM_init();
     Read_GPR();
    Test_Run(machine_Code);
    cout<<"Finally GPR"<<endl;
     Read_GPR();
    return 0;
}