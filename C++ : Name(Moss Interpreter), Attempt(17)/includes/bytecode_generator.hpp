#ifndef __BYTECODE__GEN__
#define __BYTECODE__GEN__

#include"../misc/constants.hpp"
#include"interpreter.hpp"
/* Generates Bytecode that the bytecode reader executes and understands from the Nodes generated by the Interpreter*/
// Simple bytecode instruction: consists of <opcode, operands> opcode will be singular but operands can be multiple

class ByteGen
{
  public:
  ByteGen(Parser& nodes)
  {
    this->_nodes = nodes._get_nodes();
  }

  inline void generate_bytecode()
  {
    std::size_t len = this->_nodes.size();
    std::size_t ind = 0;
    while(ind < len)
    {
        if(_nodes[ind]._type == EXPRESSION)
        {
            __bytecode.push_back(std::make_pair(LINE, _nodes[ind]._toks));
        }
        ind++;
    }
  }

  inline std::vector<std::pair<std::string,std::queue<std::pair<std::string,std::string>>>> get_code()
  {
    return this->__bytecode;
  }

  private:
  std::vector<std::pair<std::string,std::queue<std::pair<std::string,std::string>>>> __bytecode;
  std::vector<Node> _nodes;
};

#endif