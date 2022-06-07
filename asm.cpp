/*
CS-UY 2214
Jeff Epstein
Starter code for E20 assembler
asm.cpp
*/

#include <cstddef>
#include <iostream> 
#include <string.h>
#include <vector>
#include <fstream>
#include <bitset>
#include <sstream>
#include <regex>
#include <map>

using namespace std;

/**
 * @brief Set the pararmters values for all E20 instuctions.
 * @param line each line instruction of memory
 * @param reg sets the registers
 */
void set_pararmters_values(string line, string reg[]);

/**
 * @brief Gets the register number.
 * @param reg Register
 * @return int8_t register number value.
 */
int8_t getReg(string reg);

/**
 * @brief Get the machine code valud
 * 
 * @param opcode  instruction that specifies the operation to be performed.
 * @param reg     registers 
 * @param imm     immediate value
 * @return int16_t  
 */
int16_t get_machine_code(string opcode, string reg[], int16_t imm);
/**
    print_line(address, num)
    Print a line of machine code in the required format.
    Parameters:
        address = RAM address of the instructions
        num = numeric value of machine instruction
    */
void print_machine_code(unsigned address, unsigned num);

/**
    Main function
    Takes command-line args as documented below
*/
int main(int argc, char *argv[])
{

    /*
        Parse the command-line arguments
    */
    char *filename = nullptr;
    bool do_help = false;
    bool arg_error = false;
    for (int i = 1; i < argc; i++)
    {
        string arg(argv[i]);
        if (arg.rfind("-", 0) == 0)
        {
            if (arg == "-h" || arg == "--help")
                do_help = true;
            else
                arg_error = true;
        }
        else
        {
            if (filename == nullptr)
                filename = argv[i];
            else
                arg_error = true;
        }
    }
    /* Display error message if appropriate */
    if (arg_error || do_help || filename == nullptr)
    {
        cerr << "usage " << argv[0] << " [-h] filename" << endl
             << endl;
        cerr << "Assemble E20 files into machine code" << endl
             << endl;
        cerr << "positional arguments:" << endl;
        cerr << "  filename    The file containing assembly language, typically with .s suffix" << endl
             << endl;
        cerr << "optional arguments:" << endl;
        cerr << "  -h, --help  show this help message and exit" << endl;
        return 1;
    }

    /* iterate through the line in the file, construct a list
       of numeric values representing machine code */
    ifstream f(filename);
    if (!f.is_open())
    {
        cerr << "Can't open file " << filename << endl;
        return 1;
    }

    /* our final output is a list of ints values representing
       machine code instructions */

    vector<unsigned> instructions; 
    map<string, int> label;       

    size_t const NUM_PAR = 3;      
    size_t const NUM_INST = 8191;
    string reg[NUM_PAR], myRom[NUM_INST];
    string line, opcode;

    int16_t machineCode = 0;
    int16_t imm = 0;
    int16_t programCounter = 0;

    while (getline(f, line))
    {
        size_t pos = line.find("#");
        if (pos != string::npos)
            line = line.substr(0, pos);

        // converts the string to lowercase.
        transform(line.begin(), line.end(), line.begin(), ::tolower);

        if (line.length() != 0)
        {
            myRom[programCounter] = line;

            // removes any intial whitespace before the string.
            myRom[programCounter] = regex_replace(myRom[programCounter], regex("^ +| +$|( ) +"), "$1");

            size_t pos1 = myRom[programCounter].find(":");  // Locate labels.

            if (pos1 != string::npos) 
            {
                
                label.insert(pair<string, int>(myRom[programCounter].substr(0, pos1), programCounter));
                myRom[programCounter].erase(0, pos1);  // Delete the label from Ram.
                myRom[programCounter].erase(0, 2);

                if (myRom[programCounter] == "") // If ram is empty space.
                    programCounter--;            // reset the programcounter.
            }
             
            if (programCounter > 8191)
            {
                cerr << "Exceed memory instuctions" << endl;
            }
            else
            {
                programCounter++;
            }
        }
    }

    for (int i = 0; i < programCounter; i++)
    {
        size_t pos = myRom[i].find(" ");

        if (pos != string::npos)
        {
            opcode = myRom[i].substr(0, pos);
        }

        set_pararmters_values(myRom[i], reg);

        if (opcode == "j" || opcode == "jal")
        {

            string temp = myRom[i].substr(pos + 1, myRom[i].length());

            if (isdigit(temp[0]) == true || temp[0] == '-')
                imm = stoi(temp);

            else
                imm = label.find(temp)->second;
        }
        else if (opcode == "addi" || opcode == "slti" || opcode == "lw" || opcode == "sw")
        {

            if (isdigit(reg[2].at(0)) == false && reg[2].at(0) != '-')
                reg[2] = to_string(label.find(reg[2])->second);
        }
        else if (opcode == "movi")
        {

            if (isdigit(reg[1].at(0)) == false && reg[1].at(0) != '-')
                reg[1] = to_string(label.find(reg[1])->second);
        }
        else if (opcode == "jeq")
        {
            if (isdigit(reg[2].at(0)) == false && reg[2].at(0) != '-')
                reg[2] = to_string((label.find(reg[2])->second) - i - 1);

            else
                reg[2] = to_string(stoi(reg[2]) - i - 1);
        }
        else if (opcode == ".fill")
        {
            imm = stoi(myRom[i].substr(pos, (myRom[i].length()) - pos));
        }

        if (myRom[i] == "halt")
        {
            opcode = "halt";
            imm = i;
        }

        machineCode = get_machine_code(opcode, reg, imm);

        instructions.push_back(machineCode);
    }

    /* print out each instruction in the required format */
    unsigned address = 0;
    for (unsigned instruction : instructions)
    {
        print_machine_code(address, instruction);
        address++;
    }

    return 0;
}

void set_pararmters_values(string line, string reg[])
{

    string regLine;

    // Removes any whitespce
    line.erase(remove(line.begin(), line.end(), ' '), line.end());

    size_t pos1 = line.find("$");
    if (pos1 != string::npos)
    {
        regLine = line.substr(pos1, string::npos);

        stringstream s_strem(regLine);
        int num = 0;
        while (s_strem.good())
        {
            string temp;
            getline(s_strem, temp, ',');
            reg[num] = temp;
            num++;
        }

        size_t pos2 = line.find("(");
        size_t pos3 = line.find(")");
        size_t pos4 = line.find(",");

        if ((pos2 != string::npos))
        {
            reg[1] = line.substr(pos2 + 1, (pos3 - pos2) - 1);
            reg[2] = line.substr(pos4 + 1, (pos2 - pos4) - 1);
        }
    }
}

int8_t getReg(string reg)
{

    int8_t regNum = 0;

    if (reg == "$0")
        regNum = 0;
    else if (reg == "$1")
        regNum = 1;
    else if (reg == "$2")
        regNum = 2;
    else if (reg == "$3")
        regNum = 3;
    else if (reg == "$4")
        regNum = 4;
    else if (reg == "$5")
        regNum = 5;
    else if (reg == "$6")
        regNum = 6;
    else if (reg == "$7")
        regNum = 7;
    else
        regNum = stoi(reg);
    return regNum;
}

int16_t get_machine_code(string opcode, string reg[], int16_t imm)
{

    int16_t code = 0;

    if (opcode == "add")
        code = (0b000 << 13) | (getReg(reg[1]) << 10) | (getReg(reg[2]) << 7) | (getReg(reg[0]) << 4) | (0b0000);

    else if (opcode == "sub")
        code = (0b000 << 13) | (getReg(reg[1]) << 10) | (getReg(reg[2]) << 7) | (getReg(reg[0]) << 4) | (0b0001);

    else if (opcode == "and")
        code = (0b000 << 13) | (getReg(reg[1]) << 10) | (getReg(reg[2]) << 7) | (getReg(reg[0]) << 4) | (0b0010);

    else if (opcode == "or")
        code = (0b000 << 13) | (getReg(reg[1]) << 10) | (getReg(reg[2]) << 7) | (getReg(reg[0]) << 4) | (0b0011);

    else if (opcode == "slt")
        code = (0b000 << 13) | (getReg(reg[1]) << 10) | (getReg(reg[2]) << 7) | (getReg(reg[0]) << 4) | (0b0100);

    else if (opcode == "jr")
        code = (0b000 << 13) | (getReg(reg[0]) << 10) | (getReg("$0") << 7) | (getReg("$0") << 4) | (0b1000);

    else if (opcode == "slti")
        code = (0b111 << 13) | (getReg(reg[1]) << 10) | (getReg(reg[0]) << 7) | (getReg(reg[2]) & 127);

    else if (opcode == "lw")
        code = (0b100 << 13) | (getReg(reg[1]) << 10) | (getReg(reg[0]) << 7) | (getReg(reg[2]) & 127);

    else if (opcode == "sw")
        code = (0b101 << 13) | (getReg(reg[1]) << 10) | (getReg(reg[0]) << 7) | (getReg(reg[2]) & 127);

    else if (opcode == "jeq")
        code = (0b110 << 13) | (getReg(reg[0]) << 10) | (getReg(reg[1]) << 7) | (getReg(reg[2]) & 127);

    else if (opcode == "addi")
        code = (0b001 << 13) | (getReg(reg[1]) << 10) | (getReg(reg[0]) << 7) | (getReg(reg[2]) & 127);

    else if (opcode == "j")
        code = (0b010 << 13) | (imm & 8191);

    else if (opcode == "jal")
        code = (0b011 << 13) | (imm & 8191);

    else if (opcode == "movi")
        code = (0b001 << 13) | (getReg("$0") << 10) | (getReg(reg[0]) << 7) | (getReg(reg[1]) & 127);

    else if (opcode == "nop")
        code = (0b000 << 13) | (0 << 10) | (0 << 7) | (0 << 4) | (0b0000);

    else if (opcode == "halt")
        code = code = (0b010 << 13) | (imm & 8191);

    else if (opcode == ".fill")
        code = imm;

    return code;
}

void print_machine_code(unsigned address, unsigned num)
{
    bitset<16> instruction_in_binary(num);
    cout << "ram[" << address << "] = 16'b" << instruction_in_binary << ";" << endl;
}

// ra0Eequ6ucie6Jei0koh6phishohm9