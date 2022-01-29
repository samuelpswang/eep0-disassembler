#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <cmath>
#include <regex>
using namespace std;


// Datamap Functions
string hex_map(char hexchar) {
    map<char, string> hex_map;
    hex_map['0'] = "0000";
    hex_map['1'] = "0001";
    hex_map['2'] = "0010";
    hex_map['3'] = "0011";
    hex_map['4'] = "0100";
    hex_map['5'] = "0101";
    hex_map['6'] = "0110";
    hex_map['7'] = "0111";
    hex_map['8'] = "1000";
    hex_map['9'] = "1001";
    hex_map['A'] = "1010";
    hex_map['B'] = "1011";
    hex_map['C'] = "1100";
    hex_map['D'] = "1101";
    hex_map['E'] = "1110";
    hex_map['F'] = "1111";
    if (!hex_map.count(hexchar)) throw invalid_argument("non-hexadecimal character");
    else return hex_map[hexchar];
}

string opcode_map(const string& bincode) {
    map<string, string> opcode_map;
    opcode_map["000"] = "MOV";
    opcode_map["001"] = "ADD";
    opcode_map["010"] = "SUB";
    opcode_map["011"] = "ADC";
    opcode_map["100"] = "LDR";
    opcode_map["101"] = "STR";
    opcode_map["1100"] = "JMP";
    opcode_map["1101"] = "JNE";
    opcode_map["1110"] = "JCS";
    opcode_map["1111"] = "JMI";
    return opcode_map[bincode];
}


// Utility Functions
string dec_str_to_bin_str(const string& dec) {
    int num = stoi(dec);
    string binstr = "";
    for (; num != 0; num /= 2) {
        binstr = to_string(num % 2) + binstr;
    }
    while (binstr.length() < 16) binstr = "0" + binstr;
    return binstr;
}

int bin_str_to_dec_int(const string& bin_str) {
    int dec = 0;
    for (int i = 0; i < bin_str.length(); i++) {
        dec = dec*2 + stoi(bin_str.substr(i, 1));
    }
    return dec;
}

string hex_str_to_bin_str(const string& hex) {
    string binstr;
    for (int i = 0; i < hex.length(); i++) {
        binstr += hex_map(hex[i]);
    }
    return binstr;
}


// Instruction Class
class Instruction {
    friend ostream& operator<<(ostream& os, const Instruction& instr) {
        os << instr.opcode << instr.i << instr.ra << instr.rb << instr.imm8;
        return os;
    }

public:
    Instruction(const string& mc) {
        string bin;
        if (mc.substr(0, 2) == "0x") {
            regex hex("0x[0-9A-F]{4}");
            if (!regex_match(mc, hex)) throw new invalid_argument("invalid hexadecimal instruction");
            bin = hex_str_to_bin_str(mc.substr(2, 4));
        }
        else if (mc.substr(0, 2) == "0b") {
            regex bin1("0b[0-1]{16}");
            if (!regex_match(mc, bin1)) throw new invalid_argument("invalid hexadecimal instruction");
            bin = mc.substr(2, 16);
        }
        else {
            regex bin2("[0-1]{16}");
            if (!regex_match(mc, bin2)) throw new invalid_argument("invalid hexadecimal instruction");
            bin = mc;
        }

        opcode = bin.substr(0, 3);
        i = bin.substr(3, 1);
        ra = bin.substr(4, 2);
        rb = bin.substr(6, 2);
        imm8 = bin.substr(8, 8);
    }

    string disassemble() {
        string assembly;
        assembly += this->get_opcode();
        if (assembly.substr(0, 1) == "J") {
            assembly += " " + this->get_imm8();
            return assembly;
        }
        else if (assembly == "LDR" || assembly == "STR") {
            assembly += " " + this->get_ra();
            assembly += ", [" + this->get_imm8() + "]";
            return assembly;
        }
        else {
            assembly += " " + this->get_ra();
            if (this->i == "0") assembly += ", " + this->get_rb();
            else assembly += ", " + this->get_imm8();
            return assembly;
        }
    }

private:
    // Data Mambers
    string opcode;
    string i;
    string ra;
    string rb;
    string imm8;

    // Getter Simplication Layer
    string get_opcode() {
        if (this->opcode.substr(0, 2) == "11") {
            return opcode_map(this->opcode + this->i);
        }
        else
            return opcode_map(this->opcode);
    }
    
    string get_ra() {
        return "R" + to_string(bin_str_to_dec_int(this->ra));
    }

    string get_rb() {
        return "R" + to_string(bin_str_to_dec_int(this->rb));
    }

    string get_imm8() {
        return "#" + to_string(bin_str_to_dec_int(this->imm8));
    }
};


// User Interface
int main() {
    // Instructions
    cout << "Type your command in the following format: " << endl;
    cout << "1. Hexadecimal line commands (caps only): 0xABCD" << endl;
    cout << "2. Binary line commands: 0b0000000000000000 or 0000000000000000" << endl;
    cout << "3. File input (reads from \"input.txt\" & writes to \"output.txt\"): input" << endl;
    cout << "4. Exit: exit" << endl;
    cout << "5. **see existing input.txt for examples**" << endl;
    
    // Command Line 
    string user;
    while(cout << "> " && cin >> user) {
        if (user == "exit") {
            cout << "Program exited..." << endl;
            return 0;
        }
        else if (user == "input") {
            // Opening Input file
            ifstream ifs;
            ifs.open("input.txt");
            if (!ifs) {
                cout << "File Not Found Error: input.txt not found" << endl;
                continue;
            }
            
            // Opening output file
            ofstream ofs;
            ofs.open("output.txt");
            
            // Parsing...
            string line;
            int line_count = 1;
            while (ifs >> line) {
                try {
                    Instruction instr = Instruction(line);
                    ofs << line_count << "\t" << line << "\t" << instr.disassemble() << endl;
                }
                catch (...) {
                    ofs << line_count << "\t" << line << "\t" << "input error" << endl;
                    continue;
                }
                line_count++;
            }

            // Cleanup
            ifs.close();
            ofs.close();
            cout << "Done: output.txt written" << endl;
        }
        else {
            try {
                Instruction instr = Instruction(user);
                cout << instr.disassemble() << endl;
            }
            catch (...) {
                cout << "Input Error: input is not valid machine code" << endl;
                continue;
            }
        }
    }

    // For Verification Purposes Only
    // * Create file called "possible_commands.txt".
    // * Will write decimal command, binary commande, and assembly code.
    // * Run also if you need a match table.
    // ofstream ofs;
    // ofs.open("possible_commands.txt");
    // for (int i = 0; i < (pow(2, 16)); i++) {
    //     Instruction instr = Instruction(dec_str_to_bin_str(to_string(i)));
    //     ofs << i << "\t\t" << instr << "\t" << instr.disassemble() << endl;
    // }
    // ofs.close();
}
 