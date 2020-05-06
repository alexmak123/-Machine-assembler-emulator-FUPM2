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


/////////////////////////////////////////////////////////// CONSTANTS ///////////////////////////////////////////////////////////


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
STORER2 = 71,
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
{"storer2",STORER2},
};


//u_int because command cannot be negative
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
{RET, "J"}, // ATTENTION IT HAS TYPE RI IN THE CLAUSE
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


//shows address of the function in my_Emulator address space
//u_int because address cannot be negative
map <string, unsigned int> map_for_functions_and_markers;


//max size of the address space of my_Emulator
//u_int because size cannot be negative
const unsigned int MAX_SIZE = 1048575;


//shows where we should start our executing
//represents address in my emulator address space
unsigned int ADDRESS_IN_MY_EMULATOR_WHERE_EXECUTING_STARTS = 0;


/////////////////////////////////////////////////////////// COMMAND TYPES ///////////////////////////////////////////////////////////


//+
//template because we would use different types with different amounts of bits and we don't want to convert accidentally to a specific type
template <typename T>
string positive_T_to_binary (T n) {
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
//int because we will send signed values to the function and I guarantee that I wont send anything bigger than int ([-2^19;2^20-1])
string to_binary (int value, unsigned int amount_of_bits, string type) {
    //we check if it fits or not
    assert(amount_of_bits <= 20);
    assert(abs(value) < pow(2, amount_of_bits));
    //all bits are 0 at the start
    string result(amount_of_bits, '0');
    if (value == 0) {
        return result;
    }

    string in_binary;
    in_binary = positive_T_to_binary (abs(value));
    int j = result.size() - 1;
    for (int i = in_binary.size() - 1; i >=0; i--) {
        result[j] = in_binary[i];
        j--;
    }

    //if signed
    if (type == "signed") {
        //if negative
        if (value < 0) {
            result[0] = '1';
        }
        //if positive
        else {
            result[0] = '0';
        }
    }

    return result;
}


//+
//int because we will send signed values to the function and I guarantee that I wont send anything bigger than int/unsigned int
int calculate_str_in_binary_to_int (string str_in_binary, string type) {
    //check that we send only values in range int/unsigned int
    assert(str_in_binary.size() >= 4 && str_in_binary.size() <= 32);
    int res = 0;
    size_t i = 0;

    //if signed
    if (type == "signed") {
        i = 1;
    }

    for (;i < str_in_binary.size(); i++) {
        assert (str_in_binary[i] == '0' || str_in_binary[i] == '1');
        res *= 2;
        res += str_in_binary[i] - '0';
    }

    //if negative
    if (type == "signed" && str_in_binary[0] == '1') {
        res *= -1;
        assert (res <= 0);
        if (res == 0) {
            return -pow(2,str_in_binary.size() - 1);
        }
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
        total_value = to_binary(command, 8, "unsigned") + to_binary(registr_1, 4, "unsigned") + to_binary(value, 20, "signed");
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
        command = calculate_str_in_binary_to_int(command_in_binary, "unsigned");
        registr_1 = calculate_str_in_binary_to_int(registr_1_in_binary, "unsigned");
        value = calculate_str_in_binary_to_int(value_in_binary, "signed");
    }
    unsigned int command, registr_1; //u_int because cant be negative
    int value; // int because can be negative
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
        total_value = to_binary(command, 8, "unsigned") + to_binary(registr_1, 4, "unsigned") + to_binary(registr_2, 4, "unsigned") + to_binary(value, 16, "signed");
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
        command = calculate_str_in_binary_to_int(command_in_binary, "unsigned");
        registr_1 = calculate_str_in_binary_to_int(registr_1_in_binary, "unsigned");
        registr_2 = calculate_str_in_binary_to_int(registr_2_in_binary, "unsigned");
        value = calculate_str_in_binary_to_int(value_in_binary, "signed");
    }
    unsigned int command, registr_1, registr_2; //u_int because cant be negative
    int value; //int because can be negative
    string total_value;
};


//+
class RM {
public :
    RM (vector <string> parsed_line_in_assembler) {
        command = str_to_command[parsed_line_in_assembler[0]];
        registr = str_to_registr[parsed_line_in_assembler[1]];
        //it's always an address of a specific value in my_Emulator address space that's why we can just use stoi
        assert (stoi(parsed_line_in_assembler[2]) >= 0);
        address = stoi(parsed_line_in_assembler[2]);
        total_value = to_binary(command, 8, "unsigned") + to_binary(registr, 4, "unsigned") + to_binary(address, 20, "unsigned");
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
        command = calculate_str_in_binary_to_int(command_in_binary, "unsigned");
        registr = calculate_str_in_binary_to_int(registr_in_binary, "unsigned");
        address = calculate_str_in_binary_to_int(address_in_binary, "unsigned");
    }
    unsigned int command, registr, address; //u_int because cant be negative
    string total_value;
};


//+
class J {
public :
    J (vector <string> parsed_line_in_assembler) {
        command = str_to_command[parsed_line_in_assembler[0]];
        //our parsed_line_in_assembler[1] can be a string (address space of my_Compiler) and it can be a number (address space of my_Emulator)
        //if it's a number - it's an address of a specific value in my_Emulator address space
        if (parsed_line_in_assembler[1][0] >= '0' && parsed_line_in_assembler[1][0] <= '9') {
            assert (stoi(parsed_line_in_assembler[1]) >= 0);
            address = stoi(parsed_line_in_assembler[1]);
        }
        //if it's a string - it's an address of function or marker in machine code
        else {
            address = map_for_functions_and_markers[parsed_line_in_assembler[1]];
        }
        total_value = to_binary(command, 8, "unsigned") + "0000" + to_binary(address, 20, "unsigned");
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
        command = calculate_str_in_binary_to_int(command_in_binary, "unsigned");
        address = calculate_str_in_binary_to_int(address_in_binary, "unsigned");
    }
    unsigned int command, address; //u_int because cant be negative
    string total_value;
};


/////////////////////////////////////////////////////////// MY_COMPILER ///////////////////////////////////////////////////////////


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
    string result;

    //at first we normalize our string
    normalize(a);
    //then we make parsing
    vector <string> tokens = parsing(a);
    //should have at least one word
    if (tokens.size() == 0) {
        return result;
    }

    //we can have different results : simple_type(RR,RI,RM,J); function(func: ); directive(end, word); function+simple_type(func: cmpi r0 r1 2); function+directive(func: word)
    string first_word = tokens[0];
    //if first word function
    if (first_word[first_word.size() - 1] == ':') {
        //if we have just a function
        if (tokens.size() == 1) {
            result = "function";
        }
        //if we have word after function
        else if (tokens[1] == "word"){
            result = "function_plus_word";
        }
        //if we have end after function
        else if (tokens[1] == "end"){
            result = "function_plus_end";
        }
        //if we have simple type after function
        else {
            result = "function_plus_" + command_to_type_of_command[str_to_command[tokens[1]]];
        }
    }
    //if first word not function
    else {
        //if we have word
        if (tokens[0] == "word") {
            result = "word";
        }
        //if we have end
        else if (tokens[0] == "end") {
             result = "end";
        }
        //if we have simple type
        else {
            result = command_to_type_of_command[str_to_command[tokens[0]]];
        }
    }

    return result;
}


//+
//this function is cleaning our assembler code from everything we don't need + it declares functions and erases them
void declare_func_plus_cleaning (vector<string>& input_assembler) {
    // at first we will declare and erase all functions
    for (size_t i = 0; i < input_assembler.size(); i++) {
        string curr_line = input_assembler[i];
        normalize(curr_line);
        string type = define_a_type(curr_line);

        //if we have function - insert function in map and erase the line from the input;
        if (type == "function") {
            map_for_functions_and_markers.insert({curr_line, i});
            input_assembler.erase(input_assembler.begin() + i);
            i--;
        }

        //if we have function_plus_something_else - insert function in map and erase ONLY function from the line;
        else if (type == "function_plus_RI" || type == "function_plus_RR" || type == "function_plus_RM" || type == "function_plus_J" || type == "function_plus_word" || type == "function_plus_end") {
            vector <string> parsed_line = parsing (curr_line);
            map_for_functions_and_markers.insert({parsed_line[0], i});
            parsed_line.erase(parsed_line.begin());
            string modified_line;
            for (size_t j = 0; j < parsed_line.size(); j++) {
                modified_line += parsed_line[j] + " ";
            }
            normalize(modified_line);
            input_assembler[i] = modified_line;
        }

        //else only not functions left
        else {
            assert(type == "RR" || type == "RI" || type == "RM" || type == "J" || type == "end" || type == "word");
        }
    }

    //we should delete ":" from all functions in the map
    for (auto i = map_for_functions_and_markers.begin(); i != map_for_functions_and_markers.end(); i++) {
        string func = i -> first;
        unsigned int address = i -> second;
        if (func[func.size()-1] == ':') {
            func.erase(func.begin() + func.size() - 1);
            map_for_functions_and_markers.erase(i);
            map_for_functions_and_markers.insert({func, address});
        }
    }

    //and now we will do and erase the directives
    for (size_t i = 0; i < input_assembler.size(); i++) {
        string curr_line = input_assembler[i];
        normalize(curr_line);
        string type = define_a_type(curr_line);

        //if we have "word" - do the "word", erase the line;
        if (type == "word") {
            //do the "word"
            //...
            input_assembler.erase(input_assembler.begin() + i);
            i--;
        }


        //if we have "end" - do the "end", erase the line;
        else if (type == "end") {
            vector <string> parsed_line = parsing (curr_line);
            ADDRESS_IN_MY_EMULATOR_WHERE_EXECUTING_STARTS = map_for_functions_and_markers[parsed_line[1]];
            input_assembler.erase(input_assembler.begin() + i);
            i--;
        }

        //only simple_types has left
        else {
            assert(type == "RR" || type == "RI" || type == "RM" || type == "J");
        }
    }
}


//+
vector<string> to_machine_code (vector<string> input_assembler) {
    vector<string> output;

    //at first we should declare all functions and markers in our input and erase them so during the main pass we cant see them
    //but we need to remember that we can have this type of syntax (lo : cmpi r2 10) so we don't delete the hole line in this case
    declare_func_plus_cleaning (input_assembler);
    //for checking
    /*for (size_t i = 0; i < input_assembler.size(); i++) {
        cout << define_a_type(input_assembler[i]) << " " <<input_assembler[i] << endl;
    }
    cout << "ADDRESS_IN_MY_EMULATOR_WHERE_EXECUTING_STARTS : " << ADDRESS_IN_MY_EMULATOR_WHERE_EXECUTING_STARTS << endl;*/

    for (size_t i = 0; i < input_assembler.size(); i++) {
        string curr_line = input_assembler[i];
        normalize(curr_line);
        string type = define_a_type(curr_line);
        vector <string> parsed_line = parsing(curr_line);
        //I assume that this parsed line is one of the simple_type command because everything else we cleaned previously with function "declare_func_plus_cleaning ()"
        assert(((type == "RM" || type == "RI") && parsed_line.size() == 3) || (type == "RR" && parsed_line.size() == 4) || (type == "J" && parsed_line.size() == 2));
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
            assert (0 == 1);
        }
    }

    return output;
}


//+
//my_Compiler is a class that represents assembler transformation to machine code
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
    words.emplace(words.begin(),"address in my emulator where executing starts : " + to_string(ADDRESS_IN_MY_EMULATOR_WHERE_EXECUTING_STARTS));
    words.emplace(words.begin(),"size of the word : 32bit");
    words.emplace(words.begin(),"size of the const : max 20bit");
    words.emplace(words.begin(),"size of the machine code : " +  to_string(words.size()-4));
    words.emplace(words.begin(),"ThisIsFUPM2Exec");
}


//+
void my_Compiler :: Print () {
    for (size_t i = 0; i < words.size(); i++) {
        cout << "address in my_Compiler : " << i << ";      "<< words[i] << endl;
    }
}


//+
vector <string> my_Compiler :: return_words () {
    return words;
}


/////////////////////////////////////////////////////////// MY_EMULATOR ///////////////////////////////////////////////////////////


//+
//Von Neumann architecture - this means that we have one address space for everything (for values, stack, machine code)
class my_Emulator {
public :
    //functions
    void halt(RI input) ;
    void syscall(RI input);
    void addi (RI input);
    void lc (RI input);
    void mov (RR input);
    void mul (RR input);
    //we should initialize address space when we send machine code to my_Emulator
    my_Emulator (my_Compiler);
    //program which executes machine code (should check with assert that my_Emulator is initialized)
    void Execute ();
    //prints :)
    void Print ();
private :
    vector <unsigned int> registrs;
    bool flags;
    unsigned int end_machine_code_pointer; //address in my_Emulator where is the last line of the machine code
    vector <string> Von_Neumann_Memory;
};


//+
my_Emulator :: my_Emulator (my_Compiler executive_file) : registrs(16,0), Von_Neumann_Memory(MAX_SIZE) {
    registrs[13] = MAX_SIZE;
    registrs[14] = MAX_SIZE;
    registrs[15] =  ADDRESS_IN_MY_EMULATOR_WHERE_EXECUTING_STARTS;
    flags = false;
    vector <string> curr = executive_file.return_words();
    assert(curr.size() > 6);
    end_machine_code_pointer = curr.size() - 1 - 6;
    for (size_t i = 6; i < curr.size(); i++) {
        Von_Neumann_Memory[i-6] = curr[i];
    }
}


//+
void my_Emulator :: Print () {
    cout << "free registers : " << endl;
    for (size_t i = 0; i <= 12; i++) {
        cout << "                       " << registrs[i] << endl;
    }
    cout << "frame-call register : " << endl;
    cout << "                       " << registrs[13] << endl;
    cout << "stack pointer register : " << endl;
    cout << "                       " << registrs[14] << endl;
    cout << "counter register : " << endl;
    cout << "                       " << registrs[15] << endl;
    cout << "flags : " << endl;
    cout << "                       " << flags << endl;
    cout << "end machine code pointer : " << endl;
    cout << "                       " << end_machine_code_pointer << endl;
    cout << "address space : " << endl;
    for (size_t i = 0; i < 25; i++) {
        cout << "address in my_Emulator : " << i << ";      " << Von_Neumann_Memory[i] << endl;
    }
    cout << "..." << endl;
}


//+
void my_Emulator :: Execute () {
    //at first we check if my_Emulator is initialized
    assert (registrs[14] == MAX_SIZE);

    while (registrs[15] <= end_machine_code_pointer) {
        string curr_line_in_binary = Von_Neumann_Memory[registrs[15]];

        string command_in_binary;
        for (int i = 0; i < 8; i++) {
            command_in_binary += curr_line_in_binary[i];
        }
        unsigned int command = calculate_str_in_binary_to_int(command_in_binary, "unsigned");
        string type = command_to_type_of_command[command];

        if (type == "RR") {
            RR input_in_command (curr_line_in_binary);
            //some cases
            switch (command) {
                //case 2 : add (input_in_command); break;
                //case 4 : sub (input_in_command); break;
                case 6 : mul (input_in_command); break;
                //case 8 : div (input_in_command); break;
                //case 13 : shl (input_in_command); break;
                //case 15 : shr (input_in_command); break;
                //case 17 : and (input_in_command); break;
                //case 19 : or (input_in_command); break;
                //case 21 : xor (input_in_command); break;
                case 24 : mov (input_in_command); break;
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
                //default: assert (0 == 1);
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
                case 12 : lc (input_in_command); break;
                //case 14 : shli (input_in_command); break;
                //case 16 : shri (input_in_command); break;
                //case 18 : andi (input_in_command); break;
                //case 20 : ori (input_in_command); break;
                //case 22 : xori (input_in_command); break;
                //case 23 : not (input_in_command); break;
                //case 38 : push (input_in_command); break;
                //case 39 : pop (input_in_command); break;
                //case 44 : cmpi (input_in_command); break;
                //default: assert (0 == 1);
            }
        } else if (type == "RM") {
            RM input_in_command (curr_line_in_binary);
            //mmm some more cases
            switch (command) {
                //case 64 : load (input_in_command); break;
                //case 65 : store (input_in_command); break;
                //case 66 : load2 (input_in_command); break;
                //case 67 : store2 (input_in_command); break;
                //default: assert (0 == 1);
            }
        } else if (type == "J") {
            J input_in_command (curr_line_in_binary);
            //finally
            switch (command) {
                //case 41 : calli (input_in_command); break;
                //case 42 : ret (input_in_command); break;
                //case 46 : jmp (input_in_command); break;
                //case 47 : jne (input_in_command); break;
                //case 48 : jeq (input_in_command); break;
                //case 49 : jle (input_in_command); break;
                //case 50 : jl (input_in_command); break;
                //case 51 : jge (input_in_command); break;
                //case 52 : jg (input_in_command); break;
                //default: assert (0 == 1);
            }
        } else {
            assert (0 == 1);
        }

        registrs[15]++;
    }
}


/////////////////////////////////////////////////////////// FUNCTIONS ///////////////////////////////////////////////////////////


//+
unsigned int _32_low_bits_of_ull (unsigned long long int value) {
    string ull_to_str (64,'0');
    string a = positive_T_to_binary(value);
    int j = 63;
    for (int i = a.size() - 1; i >= 0; i--) {
        ull_to_str[j] = a[i];
        j--;
    }
    string _32_low_bits (32,'0');
    for (int i = 32; i < 64; i++) {
        _32_low_bits[i-32] = ull_to_str[i];
    }
    return calculate_str_in_binary_to_int(_32_low_bits, "unsigned");
}


//+
unsigned int _32_high_bits_of_ull (unsigned long long int value) {
    string ull_to_str (64,'0');
    string a = positive_T_to_binary(value);
    int j = 63;
    for (int i = a.size() - 1; i >= 0; i--) {
        ull_to_str[j] = a[i];
        j--;
    }
    string _32_high_bits (32,'0');
    for (int i = 0; i < 32; i++) {
        _32_high_bits[i] = ull_to_str[i];
    }
    return calculate_str_in_binary_to_int(_32_high_bits, "unsigned");
}


//+
void my_Emulator :: halt (RI input) {
    //stop the executing by going to the end of the machine code
    registrs[15] = end_machine_code_pointer + 1;
}


//...
void my_Emulator :: syscall (RI input) {
    if (input.value == 0) {
        //exit
        halt(input);
    } else if (input.value == 100) {
        //scan int
        int a;
        cin >> a;
        registrs[input.registr_1] = a;
    } else if (input.value == 101) {
        //scan double

    } else if (input.value == 102) {
        //print int
        int a = registrs[input.registr_1];
        cout << a;
    } else if (input.value == 103) {
        //print double

    } else if (input.value == 104) {
        //scan char
        char a;
        cin >> a;
        registrs[input.registr_1] = a;
    } else if (input.value == 105) {
        //print char
        char a = registrs[input.registr_1];
        cout << a;
    }
}


//+
void my_Emulator :: addi (RI input) {
    registrs[input.registr_1] += input.value;
}


//...
void my_Emulator :: lc (RI input) {
    //NOTE : we can have RI.value more than 2^20 when we send assembler code to my_Compiler. In this case we need to do something about it, for example, RI.value mod 2^20
    registrs[input.registr_1] = input.value;
}


//+
void my_Emulator :: mov (RR input) {
    unsigned int b = registrs[input.registr_2] + input.value;
    registrs[input.registr_1] = b;
}


//+
void my_Emulator :: mul (RR input) {
    unsigned long long int a = registrs[input.registr_1] * (registrs[input.registr_2] + input.value);
    assert (input.registr_1 != 15);
    registrs [input.registr_1] = _32_low_bits_of_ull (a);
    registrs [input.registr_1 + 1] = _32_high_bits_of_ull (a);
}


/////////////////////////////////////////////////////////// MAIN ///////////////////////////////////////////////////////////


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
        cout << "function : " << i.first << "; address of function in my_Emulator address space : " << i.second << endl;
    }
    cout << endl;

    my_Emulator programm(executable_file);
    programm.Print();
    cout << endl;
    programm.Execute();

    return 0;
}
