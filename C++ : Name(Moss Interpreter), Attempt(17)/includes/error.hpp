#ifndef __ERROR__
#define __ERROR__

/*
 Error handling:
*/
#include "../misc/constants.hpp"
#include <iostream>
#include <algorithm>

struct Error
{
    std::string _error_type;
    std::string _error_level = FATAL;
    int err_length;
    std::string _str;
};

Error err;

inline void error_print_unknown_token()
{
    std::cout << "In file: " << global._file_name << ":" << global.line_number << " : " << global.column + 1 << " : " << std::endl;
    std::cout << "Error: " << RED + err._error_level + RESET << " :err code: 101: Unknown Token type"
              << " : While " << global.current_context << std::endl;
    std::cout << "In line: " << std::endl
              << "`" << global._line << std::endl;
    std::string _disp_;
    for (int i = 0; i <= global.column; i++)
    {
        _disp_ += " ";
    }
    for (int i = 0; i < err.err_length; i++)
    {
        _disp_ += "^";
    }
    std::cout << _disp_ << std::endl;
    std::cout << "Token type not supported or not defined" << std::endl;
}

inline void stray_parenthesis()
{
    std::cout << "In file: " << global._file_name << ":" << global.line_number << " : " << global.column + 1 << " : " << std::endl;
    std::cout << "Error: " << RED + err._error_level + RESET << " :err code: 102: Expected a opening parenthesis here: A closing parenthesis without opening: Stray parenthesis"
              << " : While " << global.current_context << std::endl;
    std::cout << "In line: " << std::endl
              << "`" << global._line << std::endl;
    std::string _disp;
    for (int i = 0; i <= global.column; i++)
    {
        _disp += " ";
    }
    _disp += "^";
    std::cout << _disp << std::endl;
    std::cout << "Unopened closing parenthesis" << std::endl;
}

inline void div_by_zero()
{
    global._line.erase(std::remove_if(global._line.begin(), global._line.end(), [](unsigned char c)
                                      { return std::isspace(c); }),
                       global._line.end());
    std::cout << "In file: " << global._file_name << ":" << global.line_number << " : " << global.column + 1 << " : " << std::endl;
    std::cout << "Error: " << RED + err._error_level + RESET << " :err code: 103: Divide by zero"
              << " : While " << global.current_context << std::endl;
    std::cout << "In line: " << std::endl
              << "`" << global._line << std::endl;
    std::string _disp_;
    for (int i = 0; i <= global.column; i++)
    {
        _disp_ += " ";
    }
    for (int i = 0; i < err.err_length; i++)
    {
        _disp_ += "^";
    }
    std::cout << _disp_ << std::endl;
    std::cout << "The dividend cannot be zero." << std::endl;
}

inline void incorrect_statement()
{
    std::cout << "In file: " << global._file_name << ":" << global.line_number << " : " << global.column + 1 << " : " << std::endl;
    std::cout << "Error: " << RED + err._error_level + RESET << " :err code: 104: Incorrect statement: Syntatically incorrect"
              << " : While " << global.current_context << std::endl;
    std::cout << "In line: " << std::endl
              << "`" << global._line << std::endl;
    std::string _disp_ = " ";
    for (int i = 0; i < err.err_length - 1; i++)
    {
        _disp_ += "^";
    }
    std::cout << _disp_ << std::endl;
    std::string msg = "Note: This is caused by many factors in the statement. Please recheck the statement.";
    std::cout << WHITE + msg + RESET << std::endl;
}

inline void expected_a_number()
{
    global._line.erase(std::remove_if(global._line.begin(), global._line.end(), [](unsigned char c)
                                      { return std::isspace(c); }),
                       global._line.end());
    std::cout << "In file: " << global._file_name << ":" << global.line_number << " : " << global.column + 1 << " : " << std::endl;
    std::cout << "Error: " << RED + err._error_level + RESET << " :err code: 105: Expected a number here"
              << " : While " << global.current_context << std::endl;
    std::cout << "In line: " << std::endl
              << "`" << global._line << std::endl;
    std::string _disp_ = " ";
    for (int i = 0; i < global.column; i++)
    {
        _disp_ += " ";
    }
    for (int i = 0; i < err.err_length; i++)
    {
        _disp_ += "^";
    }
    std::cout << _disp_ << std::endl;
}

inline void stray_operator()
{
    std::cout << "In file: " << global._file_name << ":" << global.line_number << " : " << global.column + 1 << " : " << std::endl;
    std::cout << "Error: " << RED + err._error_level + RESET << " :err code: 106: Invalid operator: Expected a number or string or a character after or before:"
              << " : While " << global.current_context << std::endl;
    std::cout << "In line: " << std::endl
              << "`" << global._line << std::endl;
    std::string _disp = " ";
    for (int i = 0; i <= global.column; i++)
    {
        _disp += " ";
    }
    for (int i = 0; i < err.err_length - 1; i++)
    {
        _disp += "^";
    }
    std::cout << _disp << std::endl;
    std::cout << "Invalid use of operator" << std::endl;
}

inline void expected_str_or_char()
{
    std::cout << "In file: " << global._file_name << ":" << global.line_number << " : " << global.column + 1 << " : " << std::endl;
    std::cout << "Error: " << RED + err._error_level + RESET << " :err code: 107: Expected a string or a character here:"
              << " : While " << global.current_context << std::endl;
    std::cout << "In line: " << std::endl
              << "`" << global._line << std::endl;
    std::cout << "Invalid value pass" << std::endl;
}

inline void char_lim()
{
    std::cout << "In file: " << global._file_name << ":" << global.line_number << " : " << global.column + 1 << " : " << std::endl;
    std::cout << "Error: " << RED + err._error_level + RESET << " :err code: 107: Char limit reached: A char can only hold one character:"
              << " : While " << global.current_context << std::endl;
    std::cout << "In line: " << std::endl
              << "`" << global._line << std::endl;
    std::string _disp = " ";
    for (int i = 0; i <= global.column; i++)
    {
        _disp += " ";
    }
    for (int i = 0; i < err.err_length - 1; i++)
    {
        _disp += "^";
    }
    std::cout << _disp << std::endl;
}

inline void unsupported_operation()
{
    std::cout << "In file: " << global._file_name << ":" << global.line_number << " : " << global.column + 1 << " : " << std::endl;
    std::cout << "Error: " << RED + err._error_level + RESET << " :err code: 108: Unsupported operation on strings and chars: only concat supported:"
              << " : While " << global.current_context << std::endl;
    std::cout << "In line: " << std::endl
              << "`" << global._line << std::endl;
}

inline void decode()
{
    std::string _err_type = err._error_type;
    if (_err_type == UNKNOWN_TOKEN)
    {
        error_print_unknown_token();
    }
    else if (_err_type == STRAY_PARENTHESIS)
    {
        stray_parenthesis();
    }
    else if (_err_type == DIVIDE_BY_ZERO)
    {
        div_by_zero();
    }
    else if (_err_type == INCORRECT_STATEMENT)
    {
        incorrect_statement();
    }
    else if (_err_type == EXPECTED_NUMBER)
    {
        expected_a_number();
    }
    else if (_err_type == STRAY_OPERATOR)
    {
        stray_operator();
    }
    else if (_err_type == EXPECTED_STR)
    {
        expected_str_or_char();
    }
    else if (_err_type == CHAR_LIMIT)
    {
        char_lim();
    }else if(_err_type == UNSUPPORTED_OPER)
    {
        unsupported_operation();
    }
}

#endif