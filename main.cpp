#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <stack>
#include <string>
#include <algorithm>
#include <sstream>
#include <map>
#include <assert.h>
#include <math.h>
using namespace std;


enum registr_code {
r0 = 0,
r1 = 1,
r2 = 2,
r3 = 3,
r4 = 4,
r5 = 5,
r6 = 6,
r7 = 7,
r8 = 8,
r9 = 9,
r10 = 10,
r11 = 11,
r12 = 12,
r13 = 13,
r14 = 14,
r15 = 15
};


map <string, registr_code> str_to_registr {
{"r0" , r0},
{"r1" , r1},
{"r2" , r2},
{"r3" , r3},
{"r4" , r4},
{"r5" , r5},
{"r6" , r6},
{"r7" , r7},
{"r8" , r8},
{"r9" , r9},
{"r10" , r10},
{"r11" , r11},
{"r12" , r12},
{"r13" , r13},
{"r14" , r14},
{"r15" , r15}
};


enum command_code {
HALT = 0,
SYSCALL = 1,
ADD = 2,
ADDI = 3,
SUB = 4,
SUBI = 5,
MUL = 6,
MULI = 7,
DIV = 8,
DIVI = 9,
LC = 12,
SHL = 13,
SHLI = 14,
SHR = 15,
SHRI = 16,
AND = 17,
ANDI = 18,
OR = 19,
ORI = 20,
XOR = 21,
XORI = 22,
NOT = 23,
MOV = 24,
ADDD = 32,
SUBD = 33,
MULD = 34,
DIVD = 35,
ITOD = 36,
DTOI = 37,
PUSH = 38,
POP = 39,
CALL = 40,
CALLI = 41,
RET = 42,
CMP = 43,
CMPI = 44,
CMPD = 45,
JMP = 46,
JNE = 47,
JEQ = 48,
JLE = 49,
JL = 50,
JGE = 51,
JG = 52,
LOAD = 64,
STORE = 65,
LOAD2 = 66,
STORE2 = 67,
LOADR = 68,
LOADR2 = 69,
STORER = 70,
STORER2 = 71
};


map <string, command_code> str_to_command  {
{"halt",HALT},
{"syscall",SYSCALL},
{"add",ADD},
{"addi",ADDI},
{"sub",SUB},
{"subi",SUBI},
{"mul",MUL},
{"muli",MULI},
{"div",DIV},
{"divi",DIVI},
{"lc",LC},
{"shl",SHL},
{"shli",SHLI},
{"shr",SHR},
{"shri",SHRI},
{"and",AND},
{"andi",ANDI},
{"or",OR},
{"ori",ORI},
{"xor",XOR},
{"xori",XORI},
{"not",NOT},
{"mov",MOV},
{"addd",ADDD},
{"subd",SUBD},
{"muld",MULD},
{"divd",DIVD},
{"itod",ITOD},
{"dtoi",DTOI},
{"push",PUSH},
{"pop",POP},
{"call",CALL},
{"calli",CALLI},
{"ret",RET},
{"cmp",CMP},
{"cmpi",CMPI},
{"cmpd",CMPD},
{"jmp",JMP},
{"jne",JNE},
{"jeq",JEQ},
{"jle",JLE},
{"jl",JL},
{"jge",JGE},
{"jg",JG},
{"load",LOAD},
{"store",STORE},
{"load2",LOAD2},
{"store2",STORE2},
{"loadr",LOADR},
{"loadr2",LOADR2},
{"storer",STORER},
{"storer2",STORER2}
};


//+
string positive_int_to_binary (unsigned int n) {
    string result;

    do
    {
        result += ('0' + (n % 2));
        n = n / 2;
    } while (n > 0);

    return result;
}


//-(not sure)
string to_binary (int value, int amount_of_bits, bool can_be_negative) {
    //we check if it fits or not
    assert(abs(value) < pow(2, amount_of_bits));
    //all bits are 0 at the start
    string result(amount_of_bits, '0');

    //WE SHOULD DIFFER SIGNED AND UNSIGNED SOMEHOW!!!...

    string in_binary;
    if (value >= 0) {
        in_binary = positive_int_to_binary(value);
    }
    else if (value < 0) {
        in_binary = positive_int_to_binary(abs(value) - 1);
    }

    int j = result.size() - 1;
    for (int i = in_binary.size()-1; i >=0; i--) {
        result[j] = in_binary[i];
        j--;
    }
    if (value < 0) {
        for (size_t i = 0; i < result.size(); i++) {
            if (result[i] == '0')
                result[i] = '1';
            else
                result[i] = '0';
        }
    }

    return result;
}


//+
class RM {
public :
    RM (vector <string> a) {
        command = a[0];
        registr = a[1];
        address = stoi(a[2]);
        int command_int = str_to_command[command];
        int registr_int = str_to_registr[registr];
        total_value += to_binary(command_int, 8, 0);
        total_value += to_binary(registr_int, 4, 0);
        total_value += to_binary(address, 20, 0);
    }
    string command, registr;
    unsigned int address;
    string total_value;
};


//+
class RI {
public :
    RI (vector <string> a) {
        command = a[0];
        registr1 = a[1];
        value = stoi(a[2]);
        int command_int = str_to_command[command];
        int registr1_int = str_to_registr[registr1];
        total_value += to_binary(command_int, 8, 0);
        total_value += to_binary(registr1_int, 4, 0);
        total_value += to_binary(value, 20, 1);
    }
    string command, registr1;
    int value;
    string total_value;
};


//+
class RR {
public :
    RR (vector <string> a) {
        command = a[0];
        registr1 = a[1];
        registr2 = a[2];
        value = stoi(a[3]);
        int command_int = str_to_command[command];
        int registr1_int = str_to_registr[registr1];
        int registr2_int = str_to_registr[registr2];
        total_value += to_binary(command_int, 8, 0);
        total_value += to_binary(registr1_int, 4, 0);
        total_value += to_binary(registr2_int, 4, 0);
        total_value += to_binary(value, 16, 1);
    }
    string command, registr1, registr2;
    int value;
    string total_value;
};


//-
class J {
public :
    J (vector <string> a) {
        command = a[0];
        //when we use a function cant use stoi or it should be modified
        address = stoi(a[1]);
        int command_int = str_to_command[command];
        total_value += to_binary(command_int, 8, 0);
        total_value += to_binary(address, 20, 0);
    }
    string command;
    unsigned int address;
    string total_value;
};


map <string, int> map_for_functions_and_markers;
int MAX_VALUE_IN_MAP = 0;


//-
class functions_and_markers {
public:
    functions_and_markers (string a) {

    }
    string total_value = "it's a function will deal with it later :)";
};


//+
void normalize (string& a) {
    while (a[0] == ' ') {
        a.erase(a.begin());
    }
    while (a[a.size()-1] == ' ') {
        a.erase(a.begin() + a.size() - 1);
    }
}


//+
vector <string> parsing (string a) {
    stringstream ss(a);
    string item;
    vector<string> tokens;
    while (getline(ss, item, ' ')) {
        tokens.push_back(item);
    }
    return tokens;
}


//+
string define_a_type (string a) {
    //at first we normalize our string
    normalize(a);
    //then we make parsing
    vector<string>tokens = parsing(a);
    if (tokens.size() == 3) {
        //RM or RI
        if (tokens[0] == "LOAD" || tokens[0] == "STORE" || tokens[0] == "LOAD2" || tokens[0] == "STORE2") {
            return "RI";
        } else {
            return "RM";
        }
    } else if (tokens.size() == 4) {
        return "RR";
    } else if (tokens.size() == 1 || (tokens[0] == "end" && tokens[1] == "main")) {
        return "functions_and_markers";
    } else {
        return "J";
    }
}


//+-
vector<string> to_machine_code (vector<string> input_assembler) {
    vector<string> output;
    for (size_t i = 0; i < input_assembler.size(); i++) {
        string curr_line = input_assembler[i];
        normalize(curr_line);
        string type = define_a_type(curr_line);
        vector <string> parsed_line = parsing(curr_line);
        if (type == "RM") {
            RM curr_line_in_machine_code(parsed_line);
            output.push_back(curr_line_in_machine_code.total_value);
        } else if (type == "RI") {
            RI curr_line_in_machine_code(parsed_line);
            output.push_back(curr_line_in_machine_code.total_value);
        } else if (type == "RR") {
            RR curr_line_in_machine_code(parsed_line);
            output.push_back(curr_line_in_machine_code.total_value);
        } else if (type == "J") {
            J curr_line_in_machine_code(parsed_line);
            output.push_back(curr_line_in_machine_code.total_value);
        } else {
            functions_and_markers curr_line_in_machine_code(curr_line);
            output.push_back(curr_line_in_machine_code.total_value);
        }
    }
    return output;
}


//my_Compiler is a class that represents assembler transformation to machine code
//+
class my_Compiler {
public :
    my_Compiler (vector <string>);
    void Print ();
private :
    vector <string> words; // it's 2^{20} words which have length 32. Each word consists from 0 and 1 and has address from 0 to 2^{20}-1
};


//+
my_Compiler :: my_Compiler (vector <string> input_assembler) {
    words = to_machine_code(input_assembler);
}


//+
void my_Compiler :: Print () {
    for (size_t i = 0; i < words.size(); i++) {
        cout << "address : " << i << "; word : " << words[i] << endl;
    }
}


int main(int argc, char* argv[])
{
    vector<string> input_assembler;
    string line;


    //we open file for reading and we fill in the vector with lines
    ifstream in("input.fasm.txt");
    if (in.is_open()) {
        while (getline(in, line)) {
           input_assembler.push_back(line);
        }
    }
    in.close();


    my_Compiler programm1(input_assembler);
    programm1.Print();
    return 0;
}
