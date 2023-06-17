#include <iostream>
#include <fstream>

#include "Coder.cpp"

class App {
public:
  enum Mode {compress, decompress, debug, undefined};
private:
  Mode mode;
  std::string in_filename;
  std::string out_filename;

  void output(std::string o, std::string f) {
    std::ofstream out(f);
    out << o;
    out.close();
  }

  std::string input(std::string f) {

    std::string line;
    std::string res;

    std::ifstream in(f);

    while (getline (in, line)) {
      res += line;
    }

    in.close();

    return res;
  }

public:
  App() : mode(undefined) {}
  App(int argc, char *argv[]) {
    for (size_t i = 0; i < argc; i++) {
      if (strcmp(argv[i], "-c") == 0) {
        this->setMode(App::compress);
      } else
      if (strcmp(argv[i], "-d") == 0) {
        this->setMode(App::decompress);
      } else
      if (strcmp(argv[i], "-i") == 0) {
        if (i+1 < argc) {
          this->setInFilename(argv[++i]);
        }
      } else
      if (strcmp(argv[i], "-o") == 0) {
        if (i+1 < argc) {
          this->setOutFilename(argv[++i]);
        }
      } else
      if (strcmp(argv[i], "-v") == 0) {
        this->setMode(App::debug);
      }
    }
  }


  char getMode() {
    switch (this->mode) {
      case compress: return 'c';
      case decompress: return 'd';
      case debug: return 'v';
      default: return 'u';
    }
  }

  void setMode(Mode m) {
      this->mode = m;
  }

  void setInFilename(std::string f_name) {
    this->in_filename = f_name;
  }

  std::string getInFilename() {
    return this->in_filename;
  }

  void setOutFilename(std::string f_name) {
    this->out_filename = f_name;
  }

  std::string getOutFilename() {
    return this->out_filename;
  }

  void execute() {
    std::string in = this->input(this->in_filename);
    std::string out;
    std::map<char, int> table;
    std::string table_str;
    Coder::tree t;

    char c;
    bool has_char = false;
    std::string frq;

    switch (this->mode) {
      case compress:
        out = Coder::code(in);
        this->output(out, this->out_filename);

        table = Coder::getFrqTable(in);

        for(auto it = table.begin(); it != table.end(); it++) {
          table_str += (it->first);
          table_str += std::to_string(it->second);
          table_str += ';';
        }
        this->output(table_str, "key_" + this->out_filename);

        std::cout << "\n-> " << this->in_filename << " compressed successfuly in " << this->out_filename << "\n" << std::endl;
        break;
      case decompress:
        table_str = input("key_" + this->in_filename);

        for (size_t i = 0; i < table_str.size(); i++)
        {
          if (!has_char) {
            c = table_str[i];
            has_char = true;
          } else {
            if (table_str[i] != ';') {
              frq += table_str[i];
            } else {
              table[c] = std::stoi(frq);
              has_char = false;
              frq = "";
            }
          }
        }
        t = Coder::getHuffmanTree(table);
        out = Coder::decode(in, t);
        this->output(out, this->out_filename);

        std::cout << "\n-> " << this->in_filename << " decompressed successfuly in " << this->out_filename << "\n" << std::endl;
        break;
      case debug:

        std::cout << "\n-> " << this->in_filename << " compressed in debug mode. " << "\n" << std::endl;
        out = Coder::code(in);
        std::cout << out << std::endl;
        Coder::print_code_to_decimal(out);
        std::cout << std::endl;
        break;
      default:
        break;
    }
  }
};