# **🎯 RISC-V Machine Code Simulation**

---

## **1. Language**

The code is written in **C++** and simulates the execution of a **RISC-V pipeline processor**. It interprets machine-level instructions to perform operations like addition, subtraction, memory access, and branching.

---

## **2. Overview**

This project implements a basic **RISC-V-like instruction execution framework**, simulating how a pipeline processor fetches, decodes, and executes instructions while managing memory.

### Key Components:

- **General Purpose Registers (GPR)**: Simulated using a map where each register is identified by a binary string.
- **Instruction Memory**: Simulates machine code instructions loaded into memory.
- **Data Memory (MEMORY)**: Stores memory values accessed by load/store instructions.
- **Pipeline Stages**: Simulates stages of instruction fetch, decode, execution, memory access, and write-back.

The program reads machine instructions, decodes them, and performs the appropriate arithmetic, logic, or memory operations.

---

## **3. Key Components**
The code is designed to **solve data hazards** by implementing mechanisms like hazard detection and **control hazards** by handling branching efficiently. 
### **Instruction Memory**
- **machine_Code**: Stores machine-level instructions.
- **instruction_Set**: Maps instruction addresses (in binary) to machine instructions.

### **General Purpose Registers (GPR)**
- **GPR map**: Stores values of registers using binary keys.

### **Pipeline Stages**
- **InstructionFetch**: Fetches instructions from memory.
- **InstructionDecode**: Decodes the instruction to determine the operation type, operands, and control signals.
- **RegisterMemory**: Reads/writes register values.
- **ALU (Arithmetic Logic Unit)**: Performs arithmetic/logic operations based on decoded instructions.
- **DataMemory**: Handles memory read/write operations.

---

## **4. How the Code is Implemented**

### **Initialization**
- **init()** initializes general-purpose registers with preset values.
- **DM_init()** initializes data memory with values for factorial calculations or sums of natural numbers.

### **Instruction Fetch**
- The **InstructionFetch** class fetches instructions using the program counter (**PC**).
- The fetched instruction is stored in the pipeline registers (**IFID**) and passed to the next stage.

### **Instruction Decode**
- The **InstructionDecode** class decodes the instruction into **opcode**, **register addresses**, **immediate values**, etc.
- It checks for data hazards due to instruction dependencies.

### **Execution**
- The **ALU** receives operands and the ALU operation code, performing the required operation (ADD, SUB, MUL, etc.).
- The result is stored in the destination register or used to compute a memory address.

### **Memory Access**
- In the **Data_Memory** class, if the instruction is a **load** or **store**, the corresponding memory address is accessed to read or write data.

### **Write-Back**
- The result of the ALU or memory operation is written back to the destination register, completing the instruction execution.

### **Control Unit**
- Generates control signals based on the instruction type (R, I, S, B, Ld, etc.).
- Controls data flow between pipeline stages.

---

## **5. Flowchart**

Below is a simplified flowchart describing the program's flow:

```plaintext
+-------------------------------+
|       Initialize Registers     |
+-------------------------------+
                |
                v
+-------------------------------+
|      Instruction Fetch (IF)    |
|   Fetch instruction using PC   |
+-------------------------------+
                |
                v
+-------------------------------+
|   Instruction Decode (ID)      |
|  Decode opcode, funct3, funct7 |
|     Generate immediate values  |
+-------------------------------+
                |
                v
+-------------------------------+
|   Execute (EX)                 |
|  Perform ALU operations,       |
|  Compute branch target address |
+-------------------------------+
                |
                v
+-------------------------------+
|   Memory Access (MEM)          |
| Read from/write to memory if   |
| required by the instruction    |
+-------------------------------+
                |
                v
+-------------------------------+
|  Write-Back (WB)               |
| Write result to destination    |
| register or memory             |
+-------------------------------+
                |
                v
+-------------------------------+
|        End/Repeat Cycle        |
+-------------------------------+

## **6. Ending Note**

This project serves as a basic simulation of how **machine-level instructions** are executed on a **RISC-V-like processor**. It incorporates fundamental concepts of pipeline processing, such as instruction fetch, decode, and execution stages, along with memory operations.

The code can be extended by:
- Adding more instructions for richer functionality.
- Enhancing pipeline features like **forwarding** to handle data hazards more efficiently.
- Implementing **branch prediction** to improve performance by reducing control hazards.

---

**Happy coding!** 🎉
