/*
Holds all of the functions outside a component class that helps in interpretation
*/
#ifndef __HELPERS__
#define __HELPERS__

#include <vector>
#include <string>
#include "constants.hpp"
#include <sstream>
#include "../includes/error.hpp"
#include <string.h>

// The vector that represents the operators that the interpreter accepts with their interpreter names
std::vector<std::pair<std::string, std::string>> _oper_tokens = {
    {OPER_PLUS, "+"}, {OPER_DIV, "/"}, {OPER_MINUS, "-"}, {OPER_MUL, "*"}, {OPER_CPAREN, ")"}, {OPER_OPAREN, "("}, {OPER_DEC, "--"}, {OPER_INC, "++"}, {OPER_DQUOTE, "\""}, {OPER_SQUOTE, "\'"}, {OPER_MULTILINE, "\"\""}, {OPER_ESCAPE, "\\"}, {OPER_AND, "&&"}, {OPER_IS_EQUAL, "=="}, {OPER_IS_NOT_EQUAL, "!="}, {OPER_NOT, "!"}, {OPER_OR, "||"}, {OPER_EQUAL, "="}, {GREATER, ">"}, {LESS, "<"}, {GREAT_THAN, ">="}, {LESS_THAN, "<="}, {OPER_MOD, "%"}, {OPER_EOS, ";"}};

std::vector<std::pair<std::string, std::string>> _key_tokens = {{TYPE_FALSE, "false"}, {TYPE_TRUE, "true"}, {KEY_AND, "and"}, {KEY_OR, "or"}, {KEYNOT, "not"},{KEY_BOOL, "bool"}, {KEY_CHAR, "char"}, {KEY_FLOAT, "dec"}, {KEY_NUM, "num"}, {KEY_STRING, "string"}};

/*
@brief Checks if the given given operator is a supported operator or not
@param _to_check
@returns std::pair<std::string, std::string> with the type of operator else SIG_FAILURE
*/
std::pair<std::string, std::string> get_operator_type(std::string _to_check)
{
  for (int i = 0; i < _oper_tokens.size(); i++)
  {
    if (_oper_tokens[i].second == _to_check)
      return _oper_tokens[i];
  }
  return std::make_pair(SIG_FAILURE, "ERROR");
}

std::pair<std::string, std::string> get_key_type(std::string _to_check)
{
  for (int i = 0; i < _oper_tokens.size(); i++)
  {
    if (_key_tokens[i].second == _to_check)
      return _key_tokens[i];
  }
  return std::make_pair(SIG_FAILURE, "ERROR");
}

std::pair<std::string,std::string> is_identifier(std::string _to_check)
{
  int count = 0;
  if(_to_check[0] == '_')
     count++;
  for (char x : _to_check)
  {
    if(count > 0 && IS_NUM(x))
       count++;
    else if(x == '_' || IS_ALPHA(x))
       count++;
  }
  if(count == _to_check.size())
     return std::make_pair(IDENTIFIER, _to_check);
  else 
     return std::make_pair(SIG_FAILURE, "ERROR");
}

/*
@brief Checks for the type of number
@param _to_check -checks if a number or a float
@returns -std::pair<std::string, std::string> if the number is a valid supported number else SIG_FAILURE
*/
inline std::pair<std::string, std::string> get_num_type(std::string _to_check)
{
  int dot_count = 0;
  std::string type = TYPE_NUM;
  int count = 0;
  if (_to_check[0] == '-')
    count++;
  for (int i = 0; i < _to_check.size(); i++)
  {
    if IS_NUM (_to_check[i])
      count++;
    else if (_to_check[i] == '.')
    {
      dot_count++;
      count++;
      type = TYPE_FLOAT;
    }
    if (dot_count > 1)
      break;
  }
  if (dot_count == 1 && count == _to_check.size())
  {
    return std::make_pair(TYPE_FLOAT, _to_check);
  }
  else if (dot_count == 0 && count == _to_check.size())
  {
    return std::make_pair(TYPE_NUM, _to_check);
  }
  return std::make_pair(SIG_FAILURE, "ERROR");
}

/*
@brief returns the precedence level of different operators
@param _oper -the operator which operator precedence is to be known
@returns a value
*/
inline int operator_prcedence(std::string _oper)
{
  if (_oper == "||" || _oper == "or")
    return 2;
  else if (_oper == "&&" || _oper == "and")
    return 3;
  else if (_oper == "==" || _oper == "!=")
    return 4;
  else if (_oper == "<" || _oper == "<=" || _oper == ">" || _oper == " >=")
    return 5;
  else if (_oper == "+" || _oper == "-")
    return 6;
  else if (_oper == "/" || _oper == "*" || _oper == "%")
    return 7;
  else if (_oper == "!")
    return 8;
  return 1;
}

inline float _to_float(std::pair<std::string, std::string> _to_change)
{
  float ans;
  try
  {
    ans = std::stof(_to_change.second);
  }
  catch (std::invalid_argument &arg)
  {
    return -1.00;
  }
  return ans;
}

inline double _to_double(std::string _to_change)
{
  double ans;
  try
  {
    ans = std::stod(_to_change);
  }
  catch (std::invalid_argument &arg)
  {
    return -1.000000;
  }
  return ans;
}

inline int to_int(std::string num)
{
  return std::stoi(num);
}

inline std::string to_str(auto in)
{
  std::stringstream stream;
  std::string str;
  stream << in;
  stream >> str;
  return str;
}

inline std::string _solve(std::pair<std::string, std::string> left, std::pair<std::string, std::string> right, std::string _oper)
{
  auto left_val = (left.first == TYPE_FLOAT) ? _to_double(left.second) : to_int(left.second);
  auto right_val = (right.first == TYPE_FLOAT) ? _to_double(right.second) : to_int(right.second);
  if (_oper == "+")
  {
    return to_str(left_val + right_val);
  }
  else if (_oper == "-")
  {
    return to_str(right_val - left_val);
  }
  else if (_oper == "/")
  {
    std::string s = to_str(left_val);
    if (left_val == 0)
    {
      err._error_level = FATAL;
      err._error_type = DIVIDE_BY_ZERO;
      global.column -= s.size();
      err.err_length = s.length();
      return SIG_FAILURE;
    }
    return to_str(right_val / left_val);
  }
  else if (_oper == "*")
  {
    return to_str(right_val * left_val);
  }
  else if (_oper == "%")
  {
    int lef = left_val;
    int rig = right_val;
    return to_str(rig % lef);
  }
  return SIG_FAILURE;
}

inline std::string _concat(std::pair<std::string, std::string> left, std::pair<std::string, std::string> right)
{
  return right.second + left.second;
}

inline std::string __bool_logic(std::pair<std::string, std::string> left, std::pair<std::string, std::string> right, std::string _oper)
{
  auto left_v = (left.first == TYPE_FALSE) ? 0 : (left.first == TYPE_TRUE) ? 1
                                             : (left.first == TYPE_NUM)    ? to_int(left.second)
                                                                           : _to_double(left.second);
  auto right_v = (right.first == TYPE_FALSE) ? 0 : (right.first == TYPE_TRUE) ? 1
                                               : (right.first == TYPE_NUM)    ? to_int(right.second)
                                                                              : _to_double(right.second);
  bool res;
  if (_oper == "==")
    res = left_v == right_v;
  else if (_oper == "!=")
    res = left_v != right_v;
  else if (_oper == ">")
    res = right_v > left_v;
  else if (_oper == "<")
    res = right_v < left_v;
  else if (_oper == ">=")
    res = right_v >= left_v;
  else if (_oper == "<=")
    res = right_v <= left_v;
  else if (_oper == "&&" || _oper == "and")
    res = right_v && left_v;
  else if (_oper == "||" || _oper == "or")
    res = right_v || left_v;
  if (res == true)
    return "true";
  else
    return "false";
}

inline std::string _bool_not(std::pair<std::string, std::string> _bool)
{
  if (_bool.first == TYPE_FALSE)
    return "true";
  else
    return "false";
}

#endif