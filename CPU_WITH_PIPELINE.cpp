#include <bits/stdc++.h>
using namespace std;

map<string, int> GPR;
map<string, int> MEMORY;
string binary(int a)
{
    bitset<32> bitset(a);
    return bitset.to_string();
}
void init()
{
    GPR["01001"] = 4;
    // GPR["00011"] = 6;
    // GPR["00010"] = 5;
    // GPR["00100"] = 6;
    GPR["01000"] = 5;
    GPR["00110"] = 10;
    GPR["00101"] = 7;

    // GPR["00110"]=11;
}
void DM_init()
{
    MEMORY[binary(5)] = 0; // change to 1 for factorial 
    MEMORY[binary(6)] = 7; // n + 1 for facotrial on n or sum of n natural n 
}

vector<string> machine_Code = {
    // "00000000100100100000001110110011", // add x7,x4 = 6,x9 = 4 output should be 10 at at x7
    // "00000000010100111000000100110011"  // hazard test add x2 ,x7 ,x5

    //   "00000000100100011000000100010011" // addi x2 , x3 = 6, 9 output should be 15 at x2
    //  "00000000000000101011001000000011"    // ld x4,0(x5)
    // "00000000011000111111000000100011" // sd x6 ,0(x7)

    // "00000000100100100000000110110011",
    "00000000000000101011001000000011", // x4 =1
    "00000000000000101011000100000011", // x2=1
    "00000000000000110011000110000011", // x3=6
    "00000000001100010000100001100011", // beq x2 x3 16
    // "10000000001000100000001000110011", // mul x4 x4 x2
    "00000000001000100000001000110011" ,// add x4 x4 x2
    "00000000000100010000000100010011", // x2 =x2+ 1
    "00000000110000000000000101101111", // jump to lopp5
    "00000000010000000011001000100011",
    // "00000000000000000000000000000000"  // sd value
    // "00000000100100100000000110110011", // Add more machine code instructions here
    // "00000000100101000110001100010011"
    // "00000000010001001000010001100011"
    // "00000000000000000000000101101111"
    // "10000000100100100000000110110011"
    // "00000000010000100011001010000011"
    //  00000000100101000110001100010011
};
map<string, string> instruction_Set;

void init_instruction_set()
{
    int n = machine_Code.size();
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        string pc = binary(count);
        instruction_Set[pc] = machine_Code[i];
        count += 4;
    }
}

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
void Read_GPR()
{
    cout << "HII i am reading --" << endl;
    for (auto it : GPR)
    {
        cout << it.first << "---" << it.second << endl;
    }
}
string Immediate_Generator(string &instruction, string &type)
{
    if (type == "I" || type == "Ld")
    {
        return instruction.substr(0, 12);
    }
    else if (type == "S")
    {
        return instruction.substr(0, 7) + instruction.substr(20, 5);
    }
    else if (type == "B")
    {
        string imm = instruction.substr(0, 1); // imm[12]
        imm += instruction.substr(24, 1);      // imm[11]
        imm += instruction.substr(1, 5);       // imm[10:5]
        imm += instruction.substr(20, 4);
        imm += "0"; // imm[4:1]
        cout << "B - type--" << imm << endl;
        return imm;
    }
    else if (type == "U")
    {
        string imm = instruction.substr(0, 1); // imm[20]
        imm = instruction.substr(12, 8);       // imm[19:12]
        imm += instruction.substr(11, 1);      // imm[11]
        imm += instruction.substr(1, 10);
        imm += "0"; // imm[10:1]
        return imm;
    }
    return "111";
    // imm.insert(0, 32 - imm.size(), '0');
};

// Pipeline registers

class Instruction_Memory
{
public:
    // string intruction;
    string funct7, RSL2, RSL1, funct3, Immediate, Rd, opcode;
    Instruction_Memory(string &intruction)
    {
        cout << "Ins" << intruction << endl;
        this->funct7 = intruction.substr(0, 7);
        this->RSL2 = intruction.substr(7, 5);
        this->RSL1 = intruction.substr(12, 5);
        this->funct3 = intruction.substr(17, 3);
        // decode it
        this->Rd = intruction.substr(20, 5);
        this->opcode = intruction.substr(25, 7);
        string type = get_type(this->opcode);
        cout << "type" << type << endl;
        this->Immediate = Immediate_Generator(intruction, type);
        cout << "Here" << Immediate << endl;
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
    return -1;
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
        cout << "hey writing" << endl;
        cout << "EA " << EA << endl;
        cout << "value " << value << endl;
        MEMORY[EA] = value;
    }
};

class ControlUnit
{
public:
    int AluSrc, Mem2Reg, RegWrite, MemRead, MemWrite, Branch, AluOp1, AluOp0, jump;
    ControlUnit()
    {
    }
    ControlUnit(const string &type)
    {
        AluSrc = (type == "S" || type == "I" || type == "Ld") ? 1 : 0;
        Mem2Reg = (type == "Ld") ? 1 : 0;
        RegWrite = (type == "R" || type == "I" || type == "Ld") ? 1 : 0;
        MemWrite = (type == "S") ? 1 : 0;
        Branch = (type == "B") ? 1 : 0;
        AluOp1 = (type == "R" || type == "I") ? 1 : 0;
        AluOp0 = (type == "B") ? 1 : 0;
        jump = (type == "U") ? 1 : 0;
    }
};

struct IFID
{
    string IR;
    string DPC;
    string NPC;
};

struct IDEX
{
    string JPC, DPC, imm, func3, rdl, func7;
    int rs1, rs2;
    ControlUnit CW;
    string rsl2;
};

struct EXMO
{
    int ALUOUT;
    ControlUnit CW;
    string rdl;
    int rs2;
    int flag;
};

struct MOWB
{
    int LDOUT, ALUOUT;
    ControlUnit CW;
    string rdl;
};

class InstructionFetch
{
public:
    IFID ifid;
    // control  hazard//

    void fetch(string &PC)
    {
        int pc = stoll(PC, nullptr, 2);
        cout << "pc : " << pc << endl;
        ifid.IR = instruction_Set[PC];
        ifid.DPC = PC;
        bitset<32> bitset(pc + 4);
        string npc = bitset.to_string();
        ifid.NPC = npc; // 1 -> 4
        cout << "Finished" << endl;
    }
    bool checkHazard()
    {
        string opcode = ifid.IR.substr(25, 7);
        string type = get_type(opcode);
        if (type == "B" || type == "U")
        {
            return true;
        }
        return false;
    }
};

class InstructionDecode
{
public:
    IDEX idex;
    bool checkHazard(EXMO &exmo, IFID &ifid)
    {
        cout << "So i am data hazard checker " << endl;

        string rsl1 = ifid.IR.substr(12, 5);
        string rsl2 = ifid.IR.substr(7, 5);
        cout << "rsl1 " << rsl1 << " rsl2 " << rsl2 << endl;
        cout << "previous let check " << exmo.rdl << endl;
        if (exmo.CW.RegWrite && (exmo.rdl == rsl1 || exmo.rdl == rsl2))
        {
            return true;
        }
        return false;
    }
    void decode(IFID &ifid)
    {
        cout << "Called" << endl;
        cout << "IFId " << ifid.DPC << endl;
        cout << "IFId " << ifid.NPC << endl;
        cout << "IFId " << ifid.IR << endl;

        Instruction_Memory IM(ifid.IR);
        cout << "hey there1" << endl;
        idex.DPC = ifid.DPC;
        cout << "hey there2" << endl;
        cout << "Imeed-- " << IM.Immediate << endl;
        idex.imm = IM.Immediate;

        cout << "hey there4  " << idex.imm << endl;
        // idex.JPC = to_string(stoll(idex.imm, nullptr,2) + stoll(ifid.NPC, nullptr, 2)); // need to be checked
        cout << "hey there3" << endl;
        idex.CW = ControlUnit(get_type(IM.opcode));
        cout << "Hii1" << endl;
        Register_Memory Rg;
        idex.rs1 = Rg.read_RS1(IM.RSL1);

        if (idex.CW.AluSrc)
        {
            idex.rs2 = stoll(IM.Immediate, nullptr, 2);
        }
        else
        {
            idex.rs2 = Rg.read_RS2(IM.RSL2);
        }
        string type = get_type(IM.opcode);
        if (type == "Ld" || type == "S")
        { // register no
            idex.rs1 = stoll(IM.RSL1, nullptr, 2);
        }
        idex.rdl = IM.Rd;
        idex.func3 = IM.funct3;
        idex.func7 = IM.funct7;
        idex.rsl2 = IM.RSL2;

        cout << "Finished Decode" << endl;
    }
};

class Execute
{
public:
    EXMO exmo;
    // Execute() {}

    void execute(IDEX &idex)
    {

        string Alu_op = to_string(idex.CW.AluOp1) + to_string(idex.CW.AluOp0);
        string Alu_Select = Alu_Control(Alu_op, idex.func7, idex.func3);

        // Alu Caculation
        cout << "Alu_Select" << Alu_Select << "-- " << Operation[Alu_Select] << endl;
        ;
        int Alu_Output = Alu(Alu_Select, idex.rs1, idex.rs2);
        cout << "rs1 " << idex.rs1 << "rs2" << idex.rs2 << endl;
        cout << "Alu Output  " << Alu_Output << endl;

        int flag = (idex.CW.Branch && Alu_Output == 0) ? 1 : 0;
        exmo.ALUOUT = Alu_Output; // Perform ALU operation
        exmo.CW = idex.CW;
        exmo.rs2 = GPR[idex.rsl2]; // update rs2 here after execution
        exmo.rdl = idex.rdl;
        exmo.flag = flag;
    }
};

// Memory Access (MEM) Stage
class MemoryAccess
{
public:
    MOWB mowb;
    // MemoryAccess() {}

    void memAccess(EXMO &exmo)
    {

        Data_Memory Dm;
        int LD_result;
        if (exmo.CW.MemRead)
        {
            bitset<32> bitset(exmo.ALUOUT);
            string EA = bitset.to_string();
            mowb.LDOUT = Dm.Mem_Read(EA);
            cout << "LOAD Result--========================   " << mowb.LDOUT << endl;
        }
        if (exmo.CW.MemWrite)
        {
            bitset<32> bitset(exmo.ALUOUT);
            string EA = bitset.to_string();
            cout << "EA        ----  " << EA << endl;
            // Rs2 = Rg.read_RS2(Im.RSL2);
            int Rs2 = exmo.rs2;
            cout << "Writing--" << Rs2 << endl;
            Dm.Mem_Write(EA, exmo.rs2);
            cout << "Updated" << endl;
        }

        mowb.ALUOUT = exmo.ALUOUT;
        mowb.CW = exmo.CW;
        mowb.rdl = exmo.rdl;
    }
};

// Write Back (WB) Stage
class WriteBack
{
public:
    WriteBack() {}
    void writeBack(MOWB &mowb, EXMO &exmo)
    {    
        cout<<"RD=== "<<mowb.rdl <<endl;
       
        if (mowb.CW.RegWrite)
        {
            if (mowb.CW.Mem2Reg)
            {     cout<<"value ==="<<mowb.LDOUT<<endl;
                GPR[mowb.rdl] = mowb.LDOUT; // Write memory data to register
            }
            else
            { cout<<"value ==="<<mowb.ALUOUT<<endl;
                GPR[mowb.rdl] = mowb.ALUOUT; // Write ALU result to register
            }
        }
        if(exmo.rdl==mowb.rdl){
                  exmo.rdl = "";
        }
      
    }
};

class RN
{

public:
    string pc;
    IFID ifid;
    IDEX idex;
    EXMO exmo;
    MOWB mowb;

    RN(string &PC)
    {
        this->pc = PC;
    }
    void run()
    {
        init_instruction_set();
        InstructionFetch if_stage;
        InstructionDecode id_stage;
        Execute ex_stage;
        MemoryAccess mem_stage;
        WriteBack wb_stage;
        bool first = false, second = false, third = false, fourth = false;
        bool fetch_check = true;
        bool decode_check = false;
        bool execute_check = false;
        bool memory_check = false;
        bool write_check = false;
        int count = 1;
        bool control_hazard_check = false;
        while (true)
        {
            cout << "****** Iteration no " << count << endl;

            //    if_stage.fetch(pc);
            //    id_stage.decode(if_stage.ifid);
            //    ex_stage.execute(id_stage.idex);
            //    mem_stage.memAccess(ex_stage.exmo);
            //    wb_stage.writeBack(mem_stage.mowb);
            if (!fetch_check && !decode_check && !execute_check && !memory_check && !write_check)
            {
                cout << "Out" << endl;
                break;
            }
            cout << "Starting  ---------------" << endl;
            cout << "fetch  " << fetch_check << endl;
            cout << "decode  " << decode_check << endl;
            cout << "execute  " << execute_check << endl;
            cout << "memoru  " << memory_check << endl;
            cout << "write  " << write_check << endl;
            // cout<<"fetch"<<fetch_check<<endl;
            if (write_check)
            {
                cout <<" ==============        write        ==============" << endl;
                wb_stage.writeBack(mem_stage.mowb, ex_stage.exmo);
            }
            else
            {
                write_check = false;
            }

            if (memory_check)
            {
                cout << " ==============      memory      =============="  << endl;
                mem_stage.memAccess(ex_stage.exmo);
                write_check = true;
            }
            else
            {
                write_check = false;
            }

            if (execute_check)
            {
                cout << "==============     execute     ==============" << endl;
                ex_stage.execute(id_stage.idex);
                memory_check = true;

                if (ex_stage.exmo.CW.Branch)
                {
                    if (ex_stage.exmo.flag)
                    {
                        memory_check = false;
                        int pc1 = stoll(pc, nullptr, 2);
                        int imme = stoll(id_stage.idex.imm, nullptr, 2);
                        int BPC_INT = imme + pc1;
                        cout << "BPC-- " << BPC_INT << endl;
                        bitset<32> bitsetBPC(BPC_INT);
                        string BPC = bitsetBPC.to_string();
                        pc = BPC;
                        if_stage.ifid.NPC = pc;
                        cout << "Update here for branch instruction  pc " << endl;
                        cout << "BPC  " << BPC << endl;
                        cout << "PC  " << pc << endl;
                        control_hazard_check = false;
                    }
                    else
                    {
                        memory_check = false;
                        int pc1 = stoll(pc, nullptr, 2);
                        cout << "pc : " << pc << endl;
                        bitset<32> bitset(pc1 + 4);
                        pc = bitset.to_string();
                        if_stage.ifid.NPC = pc;
                        cout << "Update here for branch instruction" << endl;
                        control_hazard_check = false;
                    }
                    if (instruction_Set.find(pc) == instruction_Set.end())
                    { // last instruction to be executed or single instrcution
                        fetch_check = false;
                        //   decode_check=false;
                    }
                }
            }
            else
            {
                memory_check = false;
            }

            if (decode_check)
            {
                cout << " ==============   decode     ==============" << endl;
                if (id_stage.checkHazard(ex_stage.exmo, if_stage.ifid))
                {
                    execute_check = false; //
                    cout << "Hazard detected, stalling..." << endl;
                    continue;
                }
                else
                {
                    id_stage.decode(if_stage.ifid);
                    execute_check = true;
                }

                if (id_stage.idex.CW.jump)
                {
                    execute_check = false;
                    string imm = id_stage.idex.imm;
                    int JPC_INT = stoll(imm, nullptr, 2);
                    bitset<32> bitsetJPC(JPC_INT);
                    string JPC = bitsetJPC.to_string();
                    pc = bitsetJPC.to_string();
                    if_stage.ifid.NPC = pc;
                    cout << "Update here for branch instruction  pc " << endl;
                    cout << "JPC  " << JPC << endl;
                    cout << "PC  " << pc << endl;
                    control_hazard_check = false;
                    if (instruction_Set.find(pc) == instruction_Set.end())
                    { // last instruction to be executed or single instrcution
                        cout << "Not valid" << endl;
                        fetch_check = false;
                        //   decode_check=false;
                    }
                }
            }
            else
            {
                execute_check = false;
            }

            if (fetch_check)
            {
                cout << "==============     fetch    ==============" << endl;
                cout << "\nFetching with pc --" << pc << endl;
                if (!control_hazard_check)
                {
                    if_stage.fetch(pc);
                    decode_check = true;
                }
                if (control_hazard_check)
                {
                    decode_check = false;
                    cout << "Ending ---------------" << endl;
                    cout << "fetch  " << fetch_check << endl;
                    cout << "decode  " << decode_check << endl;
                    cout << "execute  " << execute_check << endl;
                    cout << "memoru  " << memory_check << endl;
                    cout << "write  " << write_check << endl;

                    count++;

                    // cout << "+++++++++++++ here GPR  +++++++++++++" << endl;
                    // Read_GPR();
                    continue;
                }
                if (if_stage.checkHazard())
                {
                    control_hazard_check = true;
                    cout << "Control Hazard detected wait till the result \n";
                    //   decode_check=false;
                    cout << "Ending ---------------" << endl;
                    cout << "fetch  " << fetch_check << endl;
                    cout << "decode  " << decode_check << endl;
                    cout << "execute  " << execute_check << endl;
                    cout << "memoru  " << memory_check << endl;
                    cout << "write  " << write_check << endl;

                    count++;

                    // cout << "+++++++++++++ here GPR  +++++++++++++" << endl;
                    // Read_GPR();
                    // count++;
                    continue;
                }
            }
            else
            {
                decode_check = false;
            }

            cout << "Pc updated" << endl;
            cout << "le -- " << if_stage.ifid.NPC << endl;
            pc = if_stage.ifid.NPC;
            if (instruction_Set.find(pc) == instruction_Set.end())
            { // last instruction to be executed or single instrcution
                fetch_check = false;
                //   decode_check=false;
            }

            cout << "Ending ---------------" << endl;
            cout << "fetch  " << fetch_check << endl;
            cout << "decode  " << decode_check << endl;
            cout << "execute  " << execute_check << endl;
            cout << "memoru  " << memory_check << endl;
            cout << "write  " << write_check << endl;

            count++;

        }
    }
};

int main()
{

    cout << "Intially GPR" << endl;
    init();
    DM_init();
    Read_GPR();
    // Test_Run(machine_Code);
    string pc = binary(0);
    RN rn(pc);
    rn.run();
    cout << "Finally GPR" << endl;
    Read_GPR();
    return 0;
}
