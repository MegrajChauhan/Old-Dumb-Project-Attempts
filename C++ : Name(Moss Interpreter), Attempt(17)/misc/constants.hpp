/*
Holds all of the constants required by the interpreter
*/
#ifndef __CONSTANTS__
#define __CONSTANTS__

#include<string>
#include<queue>

//other signal constants will be defined when they are needed
#define SIG_SUCCESS "SUCCESS"
#define SIG_FAILURE "ERROR"

#define IS_NUM(x) ((x >= '0') && (x <= '9'))
#define IS_ALPHA(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
#define OPERS(x) (x == '+' || x == '-' || x == '/' || x == '*' || x == '^' || x == '%')

//colors for messages
#define RED "\033[0;31m" 
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define WHITE "\033[0;37m"
#define RESET "\033[0m"

// constants for tokens name
#define TYPE_NUM "TYPE_NUM"
#define TYPE_FLOAT "TYPE_FLOAT"
#define TYPE_STRING "STRING"
#define TYPE_CHAR "CHAR"
#define INQUOTE "INQUOTE" //defined to specifically define characters in between single and double quotes
#define TYPE_TRUE "TRUE"
#define TYPE_FALSE "FALSE"

//keywords
#define KEY_AND "K_AND"
#define KEY_OR "K_OR"
#define KEYNOT "K_NOT"
#define KEY_NUM "K_NUM"
#define KEY_FLOAT "K_FLOAT"
#define KEY_STRING "K_STR"
#define KEY_BOOL "K_BOOl"
#define KEY_CHAR "K_CHAR"

#define OPER_PLUS "PLUS"
#define OPER_MINUS "MINUS"
#define OPER_MUL "MUL"
#define OPER_DIV "DIV"
#define OPER_OPAREN "OPAREN"
#define OPER_CPAREN "CPAREN"
#define OPER_INC "INC"
#define OPER_DEC "DEC"
#define OPER_DQUOTE "DQUOTE"
#define OPER_SQUOTE "SQUOTE"
#define OPER_MULTILINE "MULTILINE"
#define OPER_ESCAPE "ESCAPE"
#define OPER_OR "OR"
#define OPER_AND "AND"
#define OPER_NOT "NOT"
#define OPER_IS_EQUAL "IS_EQUAL"
#define OPER_IS_NOT_EQUAL "IS_NOT_EQUAL"
#define OPER_EQUAL "EQUAL"
#define GREATER "GREAT"
#define LESS "LESS"
#define GREAT_THAN "GREAT_THAN"
#define LESS_THAN "LESS_THAN"
#define OPER_MOD "MOD"
#define OPER_EOS "EOS" //end of statement

//context types
#define LEXING "LEXING"
#define PARSING "PARSING"
#define EXECUTING "EXECUTING"

#define IDENTIFIER "IDENTIFIER"

//Global_ contains the current line being executed, the column the program is on and the line number being executed
//Accessed by any component
struct Global_
{
  std::string _file_name = "<stdin>"; //stdin is the default file name
  std::string _line;
  int line_number = 0;
  int column = 0;
  std::string current_context = LEXING;

  inline void update_col(int column = 0)
  {
    this->column += column;
  }

  inline void update_new_line(std::string _new_line)
  {
    _line = _new_line;
  }

};

Global_ global;

//error level
#define FATAL "FATAL"
#define WARNING "WARNING"

//error types
#define UNKNOWN_TOKEN "UNKNOWN_TOKEN"
#define STRAY_PARENTHESIS "STRAY PARENTHESIS"
#define DIVIDE_BY_ZERO "DIV_BY_ZERO"
#define INCORRECT_STATEMENT "INCORRECT_STATEMENT"
#define EXPECTED_NUMBER "EXPECT_NUMBER"
#define STRAY_OPERATOR "STRAY_OPER"
#define EXPECTED_STR "EXPECT_STR"
#define CHAR_LIMIT "CHAR_LIM"
#define UNSUPPORTED_OPER "UNSUPP_OPER"

//nodes
#define EXPRESSION "EXPR"
#define VAR_DECLR "VARDECLR"

struct Node
{
  std::string _type;
  std::queue<std::pair<std::string, std::string>> _toks;
};


//Bytecode opcodes
#define LINE "LINE" //the opcode representing the math expressions

#endif