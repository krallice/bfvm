#include <iostream>
#include <vector>
#include <array>

#include <fstream>
#include <sstream>

class BfVirtualMachine {
  private:
      
      uint16_t datapointer = 0;
      std::array<uint8_t, 30000> memory {};

      uint16_t instructionpointer = 0;
      std::string program = "";

  public:
    BfVirtualMachine(const std::string& s) {
      program = s;
    } 
    void MainLoop() {
      while(instructionpointer < program.length()) {
        // std::cout << "DEBUG: Instruction Pointer:" << instructionpointer << std::endl;
        // std::cout << "\tDEBUG: Instruction:" << program[instructionpointer] << std::endl;
        switch(program[instructionpointer]) {
          case '.':
            std::cout << char(memory[datapointer]);
            instructionpointer++;
            break;
          case ',':
            // Todo: implement unbuffered version
            std::cin >> memory[datapointer];
            instructionpointer++;
            break;
          case '>':
            datapointer++;
            instructionpointer++;
            break;
          case '<':
            datapointer--;
            instructionpointer++;
            break;
          case '+':
            memory[datapointer]++;
            instructionpointer++;
            break;
          case '-':
            memory[datapointer]--;
            instructionpointer++;
            break;
          case '[':
            // [a]
            if (memory[datapointer] == 0) {
              int count = 1;
              while (count != 0) {
                instructionpointer++;
                if (program[instructionpointer] == '[') {
                  count++;
                } else if (program[instructionpointer] == ']') {
                  count--;
                }
                // Found our matching bracket:
                if (count == 0) {
                  instructionpointer++;
                }
              }
            } else {
              instructionpointer++;
            }
            break;
          case ']':
            // [[-]++++++++++.[-]<[-]>]
            if (memory[datapointer] != 0) {
              int count = 1;
              while (count != 0) {
                instructionpointer--;
                if (program[instructionpointer] == '[') {
                  count--;
                } else if (program[instructionpointer] == ']') {
                  count++;
                }
                // Found our matching bracket:
                if (count == 0) {
                  instructionpointer++;
                }
              }
            } else {
              instructionpointer++;
            }
            break;
        }
      }

      // Flush at end of program execution:
      std::cout << std::endl;
    }
};

std::string ReadFileIntoString(const std::string& path) {
    auto ss = std::ostringstream{};
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '" << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    ss << input_file.rdbuf();
    std::string returnstring = ss.str();
    returnstring.pop_back();
    return returnstring;
}

int main(int argc, char** argv) {

  std::string program;
  try {
    program = ReadFileIntoString(argv[1]);
  } catch (...) {
    program = ">++++++++[<+++++++++>-]<.>++++[<+++++++>-]<+.+++++++..+++.>>++++++[<+++++++>-]<++.------------.>++++++[<+++++++++>-]<+.<.+++.------.--------.>>>++++[<++++++++>-]<+.";
  }
  
  BfVirtualMachine bfvm(program);
  bfvm.MainLoop();

  return 0;
}