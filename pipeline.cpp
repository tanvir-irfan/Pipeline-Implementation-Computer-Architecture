#include <iostream>
#include <cassert>
#include <string>
#include <cstdlib>
#include <bitset>
#include "instruction.h"
#include "stage.h"
#include "fetch.h"
#include "decode.h"
#include "execute.h"
#include "memaccess.h"
#include "writeback.h"

#define IMEMSIZE 256    // actual size is 256 * wordsize
#define DMEMSIZE 256    // actual size is 256 * wordsize
#define TOTALCYCLE 1000

using namespace std;

int pc;
int registers[TOTAL_REGISTER_TYPE];
Instruction** imem = NULL;
int dmem[DMEMSIZE];
bool continueSimulation;
int clockCycle;
int stallCycle;
int fetchedIns;
int finishedIns;
string registerNames[] = { "R0", "R1", "R2", "R3", "R4", "R5", "R6", "R7" };

void initialize(int choice);
void finalize();

int getPc();
void setPc(int _pc);
Instruction& getNextIns();
int getReg(RegisterType r);
void setReg(RegisterType r, int val);
int getData(int addr);
void setData(int addr, int val);
void stopSimulation();
int getCycle();
void incStatistics(StatisticsType type);

int main(int argc, char* args[]) {


    int choice;
    if (argc != 2) {
        cout << "ERROR: Wrong number of argument!!!" << endl;
        cout << "USAGE: " << string(args[0]) << " program_num" << endl;
        exit(-1);
    } else
        choice = atoi(args[1]);
    initialize(choice);

    int i;
    AbstractStage* stages[TOTAL_STAGE_TYPE];
    AbstractStage* prevStage = NULL;

    for (i = 0; i < TOTAL_STAGE_TYPE; i++) {
        stages[i] = AbstractStage::createStage((StageType)i, prevStage);
        prevStage = stages[i];
    }

    // Simulates one cycle at a time until end
    while (continueSimulation && (clockCycle <= TOTALCYCLE)) {
        for (int i = TOTAL_STAGE_TYPE - 1; i >= 0; i--) {
            stages[i]->process();
        }
        clockCycle++;
    }

    cout << "Name: Tanvir Irfan Chowdhury, ID: abl905" << endl;
    cout << "Fetched ins " << fetchedIns << endl;
    cout << "Finished ins " << finishedIns << endl;
    cout << "Total cycles " << (clockCycle - 1) << endl;
    cout << "Stall cycles " << stallCycle << endl;
    for (i = 1; i < TOTAL_REGISTER_TYPE; i++)
        cout << registerNames[i] << " " << getReg((RegisterType)i) << endl;

    for (i = 0; i < TOTAL_STAGE_TYPE; i++)
        delete stages[i];
    finalize();

    return 0;
}

void initialize(int choice) {
    int i;
    pc = 0;
    for (i = 0; i < TOTAL_REGISTER_TYPE; i++) {
        registers[i] = 0;
    }
    imem = new Instruction*[IMEMSIZE];
    assert(imem == NULL);
    for (i = 0; i < IMEMSIZE; i++)
        imem[i] = NULL;
    for (i = 0; i < DMEMSIZE; i++)
        dmem[i] = 0;
    continueSimulation = true;
    clockCycle = 1;
    AbstractStage::setPtrGetPc(&getPc);
    AbstractStage::setPtrSetPc(&setPc);
    AbstractStage::setPtrGetNextIns(&getNextIns);
    AbstractStage::setPtrGetReg(&getReg);
    AbstractStage::setPtrSetReg(&setReg);
    AbstractStage::setPtrGetData(&getData);
    AbstractStage::setPtrSetData(&setData);
    AbstractStage::setPtrStopSimulation(&stopSimulation);
    AbstractStage::setPtrGetCycle(&getCycle);
    AbstractStage::setPtrIncStatistics(&incStatistics);

    // Load program

    //provided golden binary will run Program # 1 & 6.
    //choice = 5;
    switch (choice) {
        // Program 1
    case 1:
        i = 0;
        imem[i] = new Instruction(ADDI, R1, R0, 10, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R5, R0, 2, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R2, R0, 196, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R3, R0, 396, i);
        i += 4;
        imem[i / 4] = new Instruction(ST, R1, R2, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(MUL, R4, R1, R5, i);
        i += 4;
        imem[i / 4] = new Instruction(ST, R4, R3, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R2, R2, 4, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R3, R3, 4, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R1, R1, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(BNEQZ, R1, -7, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R1, R0, 10, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R2, R0, 196, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R3, R0, 396, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R4, R0, 596, i);
        i += 4;
        imem[i / 4] = new Instruction(LD, R5, R2, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(LD, R6, R3, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(ADD, R5, R5, R6, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R7, R0, 4, i);
        i += 4;
        imem[i / 4] = new Instruction(DIV, R5, R5, R7, i);
        i += 4;
        imem[i / 4] = new Instruction(ST, R5, R4, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R2, R2, 4, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R3, R3, 4, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R4, R4, 4, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R1, R1, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(BNEQZ, R1, -11, i);
        i += 4;
        imem[i / 4] = new Instruction(HLT, i);
        break;

        // Program 2
    case 2:
        i = 0;
        imem[i] = new Instruction(ADDI, R7, R0, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R1, R0, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R2, R1, 10, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R2, 12, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R3, R0, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R4, R0, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(SUB, R2, R4, R1, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R2, R2, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R2, 3, i);
        i += 4;
        imem[i / 4] = new Instruction(MUL, R3, R3, R4, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R4, R4, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R0, -6, i);
        i += 4;
        imem[i / 4] = new Instruction(ST, R3, R7, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R7, R7, 4, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R1, R1, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R0, -14, i);
        i += 4;
        imem[i / 4] = new Instruction(HLT, i);
        break;

        // Program 3
    case 3:
        i = 0;
        imem[i] = new Instruction(ADDI, R1, R0, 10, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R1, R0, 8, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R1, R0, 16, i);
        i += 4;
        imem[i / 4] = new Instruction(ST, R1, R1, 4, i);
        i += 4;
        imem[i / 4] = new Instruction(HLT, i);
        break;

        // Program 4
    case 4:
        i = 0;
        imem[i] = new Instruction(ADDI, R4, R0, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R1, R0, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R5, R1, 10, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R5, 13, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R2, R0, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R5, R2, 10, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R5, 8, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R3, R0, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R5, R3, 5, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R5, 3, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R4, R4, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R3, R3, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R0, -5, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R2, R2, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R0, -10, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R1, R1, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R0, -15, i);
        i += 4;
        imem[i / 4] = new Instruction(HLT, i);
        break;

        // Program 5
    case 5:
        i = 0;
        imem[i] = new Instruction(ADDI, R1, R0, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R2, R0, 1023, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R3, R0, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R7, R0, 2, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R4, R0, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R5, R4, 32, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R5, 6, i);
        i += 4;
        imem[i / 4] = new Instruction(AND, R6, R2, R3, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R6, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R1, R1, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(MUL, R3, R3, R7, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R4, R4, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(BEQZ, R0, -8, i);
        i += 4;
        imem[i / 4] = new Instruction(HLT, i);
        break;

        //Program 6
    case 6:
        i = 0;
        imem[i] = new Instruction(ADDI, R1, R0, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R2, R0, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(ADDI, R5, R0, 10, i);
        i += 4;
        imem[i / 4] = new Instruction(ADD, R3, R1, R2, i);
        i += 4;
        imem[i / 4] = new Instruction(ADD, R1, R2, R0, i);
        i += 4;
        imem[i / 4] = new Instruction(ADD, R2, R3, R0, i);
        i += 4;
        imem[i / 4] = new Instruction(SUBI, R5, R5, 1, i);
        i += 4;
        imem[i / 4] = new Instruction(BNEQZ, R5, -5, i);
        i += 4;
        imem[i / 4] = new Instruction(ST, R3, R0, 0, i);
        i += 4;
        imem[i / 4] = new Instruction(HLT, i);
        break;

    default:
        cout << "No program exists!!!" << endl;
        assert(0);
        break;
    }

}

void finalize() {
    int i;
    for (i = 0; i < IMEMSIZE; i++)
        if (imem[i] != NULL)
            delete imem[i];
    delete[] imem;
}

int getPc() {
    return pc;
}

void setPc(int _pc) {
    pc = _pc;
}

Instruction& getNextIns() {
    Instruction* ins = imem[pc >> 2];
    assert(ins != NULL);
    return *ins;
}

int getReg(RegisterType r) {
    return registers[r];
}

void setReg(RegisterType r, int val) {
    registers[r] = val;
}

int getData(int addr) {
    return dmem[addr >> 2];
}

void setData(int addr, int val) {
    dmem[addr >> 2] = val;
}

void stopSimulation() {
    continueSimulation = false;
}

int getCycle() {
    return clockCycle;
}

string getRegisterName(int reg) {
    return registerNames[reg];
}

void incStatistics(StatisticsType type) {
    switch (type) {
    case STALLCYCLE:
        stallCycle++;
        break;
    case FETCHEDINS:
        fetchedIns++;
        break;
    case FINISHEDINS:
        finishedIns++;
        break;
    default:
        assert(0);
        break;
    }
}
