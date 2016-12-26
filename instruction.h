#ifndef INSTRUCTION_H
#define INSTRUCTION_H
typedef enum _InstructionType {
    NOP = 0,   // NOP          : Do nothing
    ADD,     // ADD R1 R2 R3 : R1 = R2 + R3
    SUB,     // SUB R1 R2 R3 : R1 = R2 - R3
    MUL,     // MUL R1 R2 R3 : R1 = R2 * R3
    DIV,     // DIV R1 R2 R3 : R1 = R2 / R3
    XOR,     // XOR R1 R2 R3 : R1 = R2 ^ R3
    AND,     // AND R1 R2 R3 : R1 = R2 & R3
    OR,      // OR  R1 R2 R3 : R1 = R2 | R3
    ADDI,    // ADDI R1 R2 100: R1 = R2 + 100 
    SUBI,    // SUBI R1 R2 100: R1 = R2 - 100 
    LD,      // LD  R1 R2 32 : R1 = Mem[R2+32]
    ST,      // ST  R1 R2 32 : Mem[R2+32] = R1 
    BEQZ,    // BEQZ R1 100  : if R1==0 goto NPC + 4*100
    BNEQZ,   // BNEQZ R1 100 : if R1!= 0 goto NPC + 4*100
    HLT,     // HLT          : Exit program
    TOTAL_INSTRUCTION_TYPE
} InstructionType;
typedef enum _RegisterType {
    R0 = 0, R1, R2, R3, R4,
    R5, R6, R7, TOTAL_REGISTER_TYPE
} RegisterType;

class Instruction {
private:
    InstructionType type;
    RegisterType arg1, arg2, arg3;
    int immidiate;
    int address;
    int A, B;
    int aluOut;
    int loadMemData; // Keeps the data after a load instruction reads it from memory
    int fetchedAtCycle;
    int srcCycle1; // This field is used to identify the instruction that is supposed to provide src1.
    // I used the cycle when that instruction was fetched as the id of the instruction.
    // You can choose to use some other values as an id of source instruction
    // In that case you need to add additional field to hold the id
    int srcCycle2; // Same as srcCycle1 but it is for src2
    int branch_pc;
    bool stalled;
    void init(InstructionType _type, RegisterType _arg1, RegisterType _arg2, RegisterType _arg3,
        int _immidiate, int _address);
public:
    Instruction();
    Instruction(InstructionType _type, int address);
    Instruction(InstructionType _type, RegisterType _arg1, int _immidiate, int address);
    Instruction(InstructionType _type, RegisterType _arg1, RegisterType _arg2, int _immidiate, int address);
    Instruction(InstructionType _type, RegisterType _arg1, RegisterType _arg2, RegisterType _arg3, int address);
    Instruction(Instruction& inst);
    Instruction& operator=(Instruction& rhs);

    void clear();
    ~Instruction();
    inline InstructionType getType() { return type; }
    static inline bool isValid(InstructionType _type) { return ((_type >= NOP) && (_type < TOTAL_INSTRUCTION_TYPE)); }
    inline bool isValid() { return isValid(type); }
    static inline bool isAluReg(InstructionType _type) { return ((_type >= ADD) && (_type <= OR)); }
    inline bool isAluReg() { return isAluReg(type); }
    static inline bool isAluImm(InstructionType _type) { return ((_type >= ADDI) && (_type <= SUBI)); }
    inline bool isAluImm() { return isAluImm(type); }
    static inline bool isMemory(InstructionType _type) { return ((_type == LD) || (_type == ST)); }
    inline bool isMemory() { return isMemory(type); }
    static inline bool isLoad(InstructionType _type) { return _type == LD; }
    inline bool isLoad() { return isLoad(type); }
    static inline bool isStore(InstructionType _type) { return _type == ST; }
    inline bool isStore() { return isStore(type); }
    static inline bool isBranch(InstructionType _type) { return ((_type == BEQZ) || (_type == BNEQZ)); }
    inline bool isBranch() { return isBranch(type); }
    static inline bool isHlt(InstructionType _type) { return (_type == HLT); }
    inline bool isHlt() { return isHlt(type); }
    static inline bool isNop(InstructionType _type) { return (_type == NOP); }
    inline bool isNop() { return isNop(type); }
    static bool isDataDependent(Instruction &src, Instruction& dest);
    inline bool isDataDependent(Instruction& dest) { return isDataDependent(*this, dest); }
    inline RegisterType getArg1() { return arg1; }
    inline RegisterType getArg2() { return arg2; }
    inline RegisterType getArg3() { return arg3; }
    inline int getImmidiate() { return immidiate; }
    inline int getAddress() { return address; }
    inline int getA() { return A; }
    inline void setA(int _A) { A = _A; }
    inline int getB() { return B; }
    inline void setB(int _B) { B = _B; }
    inline int getAluOut() { return aluOut; }
    inline void setAluOut(int _aluOut) { aluOut = _aluOut; }
    inline int getLoadMemData() { return loadMemData; }
    inline void setLoadMemData(int _loadMemData) { loadMemData = _loadMemData; }
    inline int getFetchedAtCycle() { return fetchedAtCycle; }
    inline void setFetchedAtCycle(int _fetchedAtCycle) { fetchedAtCycle = _fetchedAtCycle; }
    inline int getSrcCycle1() { return srcCycle1; }
    inline void setSrcCycle1(int _srcCycle1) { srcCycle1 = _srcCycle1; }
    inline int getSrcCycle2() { return srcCycle2; }
    inline void setSrcCycle2(int _srcCycle2) { srcCycle2 = _srcCycle2; }
    inline int getBranchPc() { return branch_pc; }
    inline void setBranchPc(int bpc) { branch_pc = bpc; }
    inline bool getStalled() { return stalled; }
    inline void setStalled(bool s) { stalled = s; }
    inline bool isStalled() { return stalled; }
};
#endif
