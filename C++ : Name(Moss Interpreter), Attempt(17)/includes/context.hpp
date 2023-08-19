/*
The master class of all the other components. Can change the flow of the entire program, can stop the program etc
*/

#ifndef __CONTEXT__
#define __CONTEXT__

#include <string>
#include "../misc/constants.hpp"
#include "error.hpp"
#include <iostream>
#include"lexer.hpp"
#include "parser.hpp"
#include "bytecode_generator.hpp"
#include"bytecode_reader.hpp"

// after the necessary steps are taken to start execution, the Context class also does some preprocessing
// since the File Reader component has yet to be implemented, it will not be included
class Context
{
  // preprocessing
public:
  Context()
  {
    global.column = 0;
    global.line_number = 1;
  }

  inline std::string read_line()
  {
    std::string _line;
    std::getline(std::cin, _line);
    return _line + "`";
  }

  inline void execute()
  {
    std::string _val;
    while (true)
    {
      std::cout << ">>>";
      _val = read_line();
      Lexer lexer;
      global.update_new_line(_val);
      global.current_context = LEXING;
      std::string __res = lexer.lex();
      if (__res == SIG_FAILURE)
      {
        decode();
      }
      else
      {
        Parser parser(lexer.get_tokens());
        __res = parser.parse();
        if (__res == SIG_FAILURE)
        {
          decode();
        }
        else
        {
          ByteGen gen(parser);
          gen.generate_bytecode();
          ByteCode_read read(gen);
          read.read();
        }
      }
    }
  }
};

#endif