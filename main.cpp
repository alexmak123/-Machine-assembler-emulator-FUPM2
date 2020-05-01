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
#include <limits.h>
using namespace std;


/////////////////////////////////////////////////////////// CONSTS ///////////////////////////////////////////////////////////


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


map <unsigned int, string> command_to_type_of_command  {
{HALT, "RI"},
{SYSCALL, "RI"},
{ADD, "RR"},
{ADDI, "RI"},
{SUB, "RR"},
{SUBI, "RI"},
{MUL, "RR"},
{MULI, "RI"},
{DIV, "RR"},
{DIVI, "RI"},
{LC, "RI"},
{SHL, "RR"},
{SHLI, "RI"},
{SHR, "RR"},
{SHRI, "RI"},
{AND, "RR"},
{ANDI, "RI"},
{OR, "RR"},
{ORI, "RI"},
{XOR, "RR"},
{XORI, "RI"},
{NOT, "RI"},
{MOV, "RR"},
{ADDD, "RR"},
{SUBD, "RR"},
{MULD, "RR"},
{DIVD, "RR"},
{ITOD, "RR"},
{DTOI, "RR"},
{PUSH, "RI"},
{POP, "RI"},
{CALL, "RR"},
{CALLI, "J"},
{RET, "RI"},
{CMP, "RR"},
{CMPI, "RI"},
{CMPD, "RR"},
{JMP, "J"},
{JNE, "J"},
{JEQ, "J"},
{JLE, "J"},
{JL, "J"},
{JGE, "J"},
{JG, "J"},
{LOAD, "RM"},
{STORE, "RM"},
{LOAD2, "RM"},
{STORE2, "RM"},
{LOADR, "RR"},
{LOADR2, "RR"},
{STORER, "RR"},
{STORER2, "RR"},
};


//address of the function is an address of the next line after the name of the function
map <string, unsigned int> map_for_functions_and_markers;


//max size of the address space of my_Emulator
const unsigned int MAX_SIZE = 10000001;


/////////////////////////////////////////////////////////// THE FIRST PART ///////////////////////////////////////////////////////////


//+
string positive_int_to_binary (int n) {
    string result;
    assert(n >= 0);

    do
    {
        result += ('0' + (n % 2));
        n = n / 2;
    } while (n > 0);

    //in my view, I want to have numbers from the high bit to the low, so we need to reverse the line
    reverse(result.begin(), result.end());

    return result;
}


//+
string to_binary (int value, int amount_of_bits) {
    //we check if it fits or not
    assert(abs(value) < pow(2, amount_of_bits));
    //all bits are 0 at the start
    string result(amount_of_bits, '0');

    //we will encode our numbers in "additional" representation (2^16=0; 3 + (-3) = 0 => -3 = 2^16-3)
    string in_binary;
    if (value >= 0) {
        in_binary = positive_int_to_binary(value);
    }
    else if (value < 0) {
        in_binary = positive_int_to_binary(abs(value) - 1);
    }

    int j = result.size() - 1;
    for (int i = in_binary.size() - 1; i >=0; i--) {
        result[j] = in_binary[i];
        j--;
    }
    if (value < 0) {
        for (size_t i = 0; i < result.size(); i++) {
            if (result[i] == '0') {
                result[i] = '1';
            }
            else {
                result[i] = '0';
            }
        }
    }

    return result;
}


//+
unsigned int calculate_str_in_binary_to_int (string str_in_binary) {
    unsigned int res = 0;

    for (size_t i = 0; i < str_in_binary.size(); i++) {
        assert (str_in_binary[i] == '0' || str_in_binary[i] == '1');
        res *= 2;
        res += str_in_binary[i] - '0';
    }

    return res;
}


//+
class RI {
public :
    RI (vector <string> parsed_line_in_assembler) {
        command = str_to_command[parsed_line_in_assembler[0]];
        registr_1 = str_to_registr[parsed_line_in_assembler[1]];
        value = stoi(parsed_line_in_assembler[2]);
        total_value = to_binary(command, 8) + to_binary(registr_1, 4) + to_binary(value, 20);
    }
    RI (string line_in_machine_code) {
        assert (line_in_machine_code.size() == 32);
        string command_in_binary, registr_1_in_binary, value_in_binary;
        for (size_t i = 0; i < line_in_machine_code.size(); i++) {
            if (i < 8) {
                command_in_binary += line_in_machine_code[i];
            } else if (i >= 8 && i < 12) {
                registr_1_in_binary += line_in_machine_code[i];
            } else {
                value_in_binary += line_in_machine_code[i];
            }
        }
        assert (command_in_binary.size() == 8);
        assert (registr_1_in_binary.size() == 4);
        assert (value_in_binary.size() == 20);
        command = calculate_str_in_binary_to_int(command_in_binary);
        registr_1 = calculate_str_in_binary_to_int(registr_1_in_binary);
        value = calculate_str_in_binary_to_int(value_in_binary);
    }
    unsigned int command, registr_1, value;
    string total_value;
};


//+
class RR {
public :
    RR (vector <string> parsed_line_in_assembler) {
        command = str_to_command[parsed_line_in_assembler[0]];
        registr_1 = str_to_registr[parsed_line_in_assembler[1]];
        registr_2 = str_to_registr[parsed_line_in_assembler[2]];
        value = stoi(parsed_line_in_assembler[3]);
        total_value = to_binary(command, 8) + to_binary(registr_1, 4) + to_binary(registr_2, 4) + to_binary(value, 16);

    }
    RR (string line_in_machine_code) {
        assert (line_in_machine_code.size() == 32);
        string command_in_binary, registr_1_in_binary, registr_2_in_binary, value_in_binary;
        for (size_t i = 0; i < line_in_machine_code.size(); i++) {
            if (i < 8) {
                command_in_binary += line_in_machine_code[i];
            } else if (i >= 8 && i < 12) {
                registr_1_in_binary += line_in_machine_code[i];
            } else if (i >= 12 && i < 16) {
                registr_2_in_binary += line_in_machine_code[i];
            } else {
                value_in_binary += line_in_machine_code[i];
            }
        }
        assert (command_in_binary.size() == 8);
        assert (registr_1_in_binary.size() == 4);
        assert (registr_2_in_binary.size() == 4);
        assert (value_in_binary.size() == 16);
        command = calculate_str_in_binary_to_int(command_in_binary);
        registr_1 = calculate_str_in_binary_to_int(registr_1_in_binary);
        registr_2 = calculate_str_in_binary_to_int(registr_2_in_binary);
        value = calculate_str_in_binary_to_int(value_in_binary);
    }
    unsigned int command, registr_1, registr_2, value;
    string total_value;
};


//+
class RM {
public :
    RM (vector <string> parsed_line_in_assembler) {
        command = str_to_command[parsed_line_in_assembler[0]];
        registr = str_to_registr[parsed_line_in_assembler[1]];
        //it's always an address of a specific value in my_Emulator address space that's why we can just use stoi
        address = stoi(parsed_line_in_assembler[2]);
        total_value = to_binary(command, 8) + to_binary(registr, 4) + to_binary(address, 20);
    }
    RM (string line_in_machine_code) {
        assert (line_in_machine_code.size() == 32);
        string command_in_binary, registr_in_binary, address_in_binary;
        for (size_t i = 0; i < line_in_machine_code.size(); i++) {
            if (i < 8) {
                command_in_binary += line_in_machine_code[i];
            } else if (i >= 8 && i < 12) {
                registr_in_binary += line_in_machine_code[i];
            } else {
                address_in_binary += line_in_machine_code[i];
            }
        }
        assert (command_in_binary.size() == 8);
        assert (registr_in_binary.size() == 4);
        assert (address_in_binary.size() == 20);
        command = calculate_str_in_binary_to_int(command_in_binary);
        registr = calculate_str_in_binary_to_int(registr_in_binary);
        address = calculate_str_in_binary_to_int(address_in_binary);
    }
    unsigned int command, registr, address;
    string total_value;
};


//+
class J {
public :
    J (vector <string> parsed_line_in_assembler) {
        command = str_to_command[parsed_line_in_assembler[0]];
        //our a[1] can be a string (address space of my_Compiler) and it can be a number (address space of my_Emulator)
        //if it's a number - it's an address of a specific value in my_Emulator address space
        if (parsed_line_in_assembler[1][0] >= '0' && parsed_line_in_assembler[1][0] <= '9') {
            address = stoi(parsed_line_in_assembler[1]);
        }
        //if it's a string - it's an address of function or marker in machine code
        else {
            address = map_for_functions_and_markers[parsed_line_in_assembler[1]];
        }
        total_value = to_binary(command, 8) + "0000" + to_binary(address, 20);
    }
    J (string line_in_machine_code) {
        assert (line_in_machine_code.size() == 32);
        string command_in_binary, address_in_binary;
        for (size_t i = 0; i < line_in_machine_code.size(); i++) {
            if (i < 8) {
                command_in_binary += line_in_machine_code[i];
            } else if (i >= 8 && i < 12) {
                //just skip it;
            } else {
                address_in_binary += line_in_machine_code[i];
            }
        }
        assert (command_in_binary.size() == 8);
        assert (address_in_binary.size() == 20);
        command = calculate_str_in_binary_to_int(command_in_binary);
        address = calculate_str_in_binary_to_int(address_in_binary);
    }
    unsigned int command, address;
    string total_value;
};


//+
void normalize (string& a) {
    while (a[0] == ' ') {
        a.erase(a.begin());
    }
    while (a[a.size()-1] == ' ' || a[a.size()-1] == ':') {
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


//+
void declare_all_functions_and_markers (vector<string>& input_assembler) {
    for (size_t i = 0; i < input_assembler.size(); i++) {
        string curr_line = input_assembler[i];
        normalize(curr_line);
        string type = define_a_type(curr_line);
        if (type == "functions_and_markers") {
            map_for_functions_and_markers.insert({curr_line, i});
            input_assembler.erase(input_assembler.begin() + i);
            i--;
        }
    }
}


//+
vector<string> to_machine_code (vector<string> input_assembler) {
    vector<string> output;

    //at first we should declare all functions in our input and erase them so during the main pass we cant see them
    declare_all_functions_and_markers(input_assembler);

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
    vector <string> return_words ();
private :
    vector <string> words; // it's 2^{20} words which have length 32. Each word consists from 0 and 1 and has address from 0 to 2^{20}-1
};


//+
my_Compiler :: my_Compiler (vector <string> input_assembler) {
    words = to_machine_code(input_assembler);
    //now we will add extra information to the start so our executive file will be pretty :)
    words.emplace(words.begin(),"start address of the stack : 10^8");
    words.emplace(words.begin(),"number of the line where machine code starts : 7");
    words.emplace(words.begin(),"size of the words : 32bits");
    words.emplace(words.begin(),"size of the const : differs");
    words.emplace(words.begin(),"size of the machine code : " +  to_string(words.size()-4));
    words.emplace(words.begin(),"ThisIsFUPM2Exec");
}


//+
void my_Compiler :: Print () {
    for (size_t i = 0; i < words.size(); i++) {
        if (i >= 6) {
            cout << "address : " << i - 6<< "; word : ";
        }
        cout << words[i] << endl;
    }
}


//+
vector <string> my_Compiler :: return_words () {
    return words;
}


/////////////////////////////////////////////////////////// THE SECOND PART ///////////////////////////////////////////////////////////


//+
//Von Neumann architecture - this means that we have one address space for everything (for values, stack, machine code)
class my_Emulator {
public :
    //functions
    void halt(RI input) ;
    void syscall(RI input);
    void addi (RI input);
    //we should initialize address space when we send machine code to my_Emulator
    my_Emulator (my_Compiler);
    //program which executes machine code (should check with assert that my_Emulator is initialized)
    void Execute ();
    //prints :)
    void Print ();
private :
    vector <unsigned int> free_registrs;
    unsigned int frame_call_registr;
    unsigned int stack_pointer_registr;
    unsigned int counter_registr;
    bool flags;
    unsigned int end_machine_code_pointer;
    vector <string> Von_Neumann_Memory;
};


//+
my_Emulator :: my_Emulator (my_Compiler executive_file) : free_registrs(13,0), Von_Neumann_Memory(MAX_SIZE) {
    stack_pointer_registr = MAX_SIZE - 1;
    frame_call_registr = stack_pointer_registr;
    counter_registr = 0;
    flags = false;
    vector <string> curr = executive_file.return_words();
    end_machine_code_pointer = curr.size() - 6;
    for (size_t i = 6; i < curr.size(); i++) {
        Von_Neumann_Memory[i-6] = curr[i];
    }
}


//+
void my_Emulator :: Print () {
    cout << "free registers: " << endl;
    for (size_t i = 0; i < free_registrs.size(); i++) {
        cout << "                       " << free_registrs[i] << endl;
    }
    cout << "frame-call register: " << endl;
    cout << "                       " << frame_call_registr << endl;
    cout << "stack pointer register: " << endl;
    cout << "                       " << stack_pointer_registr << endl;
    cout << "counter register: " << endl;
    cout << "                       " << counter_registr << endl;
    cout << "flag: " << endl;
    cout << "                       " << flags << endl;
    cout << "end machine code pointer: " << endl;
    cout << "                       " << end_machine_code_pointer << endl;
    cout << "address space: " << endl;
    for (size_t i = 0; i < 25; i++) {
        cout << "                       " << Von_Neumann_Memory[i] << endl;
    }
}


//...
void my_Emulator :: Execute () {
    //at first we check if my_Emulator is initialized
    assert (end_machine_code_pointer != 0);

    while (counter_registr < end_machine_code_pointer) {
        string curr_line_in_binary = Von_Neumann_Memory[counter_registr];

        string command_in_binary;
        for (int i = 0; i < 8; i++) {
            command_in_binary += curr_line_in_binary[i];
        }
        unsigned int command = calculate_str_in_binary_to_int(command_in_binary);
        string type = command_to_type_of_command[command];

        if (type == "RR") {
            RR input_in_command (curr_line_in_binary);
            //some cases
            switch (command) {
                //case 2 : add (input_in_command); break;
                //case 4 : sub (input_in_command); break;
                //case 6 : mul (input_in_command); break;
                //case 8 : div (input_in_command); break;
                //case 13 : shl (input_in_command); break;
                //case 15 : shr (input_in_command); break;
                //case 17 : and (input_in_command); break;
                //case 19 : or (input_in_command); break;
                //case 21 : xor (input_in_command); break;
                //case 24 : mov (input_in_command); break;
                //case 32 : addd (input_in_command); break;
                //case 33 : subq (input_in_command); break;
                //case 34 : muld (input_in_command); break;
                //case 35 : divd (input_in_command); break;
                //case 36 : itod (input_in_command); break;
                //case 37 : dtoi (input_in_command); break;
                //case 40 : call (input_in_command); break;
                //case 43 : cmp (input_in_command); break;
                //case 45 : cmpd (input_in_command); break;
                //case 68 : loadr (input_in_command); break;
                //case 69 : storer (input_in_command); break;
                //case 70 : loadr2 (input_in_command); break;
                //case 71 : storer2 (input_in_command); break;
            }
        } else if (type == "RI") {
            RI input_in_command (curr_line_in_binary);
            //also some cases
            switch (command) {
                case 0 : halt (input_in_command); break;
                case 1 : syscall (input_in_command); break;
                case 3 : addi (input_in_command); break;
                //case 5 : subi (input_in_command); break;
                //case 7 : muli (input_in_command); break;
                //case 9 : divi (input_in_command); break;
                //case 12 : lc (input_in_command); break;
                //case 14 : shli (input_in_command); break;
                //case 16 : shri (input_in_command); break;
                //case 18 : andi (input_in_command); break;
                //case 20 : ori (input_in_command); break;
                //case 22 : xori (input_in_command); break;
                //case 23 : not (input_in_command); break;
                //case 38 : push (input_in_command); break;
                //case 39 : pop (input_in_command); break;
                //case 42 : ret (input_in_command); break;
                //case 44 : cmpi (input_in_command); break;
            }
        } else if (type == "RM") {
            RM input_in_command (curr_line_in_binary);
            //mmm some more cases
            switch (command) {
                //case 64 : load (input_in_command); break;
                //case 65 : store (input_in_command); break;
                //case 66 : load2 (input_in_command); break;
                //case 67 : store2 (input_in_command); break;
            }
        } else if (type == "J") {
            J input_in_command (curr_line_in_binary);
            //finally
            switch (command) {
                //case 41 : calli (input_in_command); break;
                //case 46 : jmp (input_in_command); break;
                //case 47 : jne (input_in_command); break;
                //case 48 : jeq (input_in_command); break;
                //case 49 : jle (input_in_command); break;
                //case 50 : jl (input_in_command); break;
                //case 51 : jge (input_in_command); break;
                //case 52 : jg (input_in_command); break;
            }
        } else {
            cout << "Not appropriate type of the command in execute" << endl;
        }
        counter_registr++;
    }
}


/////////////////////////////////////////////////////////// THE THIRD PART ///////////////////////////////////////////////////////////


//...
void my_Emulator :: halt (RI input) {

}


//...
void my_Emulator :: syscall (RI input) {
    if (input.value == 100) {
        unsigned int a;
        cin >> a;
        //this doesn't look nice i ll fix this later
        if (input.registr_1 >= 0 && input.registr_1 <= 12) {
            free_registrs[input.registr_1] = a;
        } else if (input.registr_1 == 13) {
            frame_call_registr = a;
        } else if (input.registr_1 == 14) {
            stack_pointer_registr = a;
        } else if (input.registr_1 == 15) {
            counter_registr = a;
        }
    } else if (input.value == 101) {

    } else if (input.value == 102) {
        //the same
        if (input.registr_1 >= 0 && input.registr_1 <= 12) {
            cout << free_registrs[input.registr_1] << endl;
        } else if (input.registr_1 == 13) {
            cout << frame_call_registr << endl;
        } else if (input.registr_1 == 14) {
            cout << stack_pointer_registr << endl;
        } else if (input.registr_1 == 15) {
            cout << counter_registr << endl;
        }
    } else if (input.value == 103) {

    } else if (input.value == 104) {

    } else if (input.value == 105) {

    }
}


//+
void my_Emulator :: addi (RI input) {
    if (input.registr_1 >= 0 && input.registr_1 <= 12) {
        free_registrs[input.registr_1] += input.value;
    } else if (input.registr_1 == 13) {
        frame_call_registr += input.value;
    } else if (input.registr_1 == 14) {
        stack_pointer_registr += input.value;
    } else if (input.registr_1 == 15) {
        counter_registr += input.value;
    } else  {
        cout << "something wrong with registers in addi function" << endl;
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

    my_Compiler executable_file(input_assembler);
    executable_file.Print();
    cout << endl;
    for (auto i : map_for_functions_and_markers) {
        cout << i.first << " " << i.second << endl;
    }
    cout << endl;

    my_Emulator programm(executable_file);
    programm.Print();
    cout << endl;
    programm.Execute();
    return 0;
}
