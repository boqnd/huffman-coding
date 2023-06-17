#include <map>
#include <vector>
#include <iostream>

class Coder {
public:
  struct node {
    char value;
    int freq;
    node* left;
    node* right;
  };

  class tree {
  public:
    node* root;

    void print() {
      print("", root, false);
    }

  private:
    void print(const std::string& prefix, const node* node, bool isLeft) {
      if( node != nullptr )
      {
        std::cout << prefix;

        std::cout << (isLeft ? "├──" : "└──" );

        if (node->value) {
          std::cout << node->freq << " -> " << node->value << std::endl;
        } else {
          std::cout << node->freq << std::endl;
        }

        print( prefix + (isLeft ? "│   " : "    "), node->left, true);
        print( prefix + (isLeft ? "│   " : "    "), node->right, false);
      }
    }
  };

private:

  static void sortByFreq(std::vector<node*>& nodes) {
    sort(nodes.begin(), nodes.end(), [](const node* a, const node* b) -> bool { return a->freq > b->freq; });
  }

  static void fillTable(const node* node, std::map<char, std::string>* codingTable, std::string current) {
    if ( node != nullptr )
    {
      if ( node->value ) {
        codingTable->insert({node->value, current});
      } else {
        fillTable(node->left, codingTable, current + "0");
        fillTable(node->right, codingTable, current + "1");
      }
    }
  }

  static std::map<char, std::string> getCodingTable(tree& t) {
    std::map<char, std::string> codingTable;

    fillTable(t.root, &codingTable, "");

    return codingTable;
  }

  static std::string helper(node* root, node* current, std::string code, int i) {

    if (i > code.size()) return "";

    if (!current->left && !current->right) {
      return current->value + helper(root, root, code, i);
    } else {
      if (code[i] == '0') {
        current = current->left;
      } else if(code[i] == '1') {
        current = current->right;
      }
      i++;
    }

    return helper(root, current, code, i);
  }

public:
  static void print_code_to_decimal(std::string& coded) {
    int size = coded.size();

    for (int i = 0; i < size; i += 8) {
      std::bitset<8>  x(coded.substr(i,8));
      std::cout << x.to_ulong() << std::endl;
    }
  }

  static std::map<char, int> getFrqTable(std::string text) {
    std::map<char, int> frqTable;

    for(char& c : text) {
      frqTable[c]++;
    }

    return frqTable;
  }

  static tree getHuffmanTree(std::map<char, int> frqTable) {
    std::vector<node*> nodes;

    for(const std::pair<char, int>& p : frqTable) {
      node* n = new node;
      n->value = p.first;
      n->freq = p.second;
      nodes.push_back(n);
    }

    while (nodes.size() >= 2) {
      // printNodes(nodes);

      sortByFreq(nodes);

      node* left = nodes.back();
      nodes.pop_back();
      node* right = nodes.back();
      nodes.pop_back();

      node* newNode = new node;
      newNode->left = left;
      newNode->right = right;
      newNode->freq = left->freq + right->freq;
      nodes.push_back(newNode);
    }

    tree t;
    t.root = nodes[0];

    return t;
  }

  static std::string decode(std::string& coded, Coder::tree& huffmanTree) {
    return helper(huffmanTree.root, huffmanTree.root, coded, 0);
  }

  static std::string code(std::string& str) {
    std::map<char, int> frqTable = getFrqTable(str);
    tree t = getHuffmanTree(frqTable);
    std::map<char, std::string> ct =  getCodingTable(t);

    std::string result = "";

    for(char& c : str) {
      result += ct[c];
    }

    return result;
  }
};