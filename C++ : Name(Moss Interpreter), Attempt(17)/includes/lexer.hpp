#ifndef __LEXER__
#define __LEXER__

/*
Lexer: lexes the line or statement to tokens
*/

#include "../misc/helpingfuncs.hpp"
#include "error.hpp"

class Lexer
{
public:
    Lexer() = default;

    /*
    @brief lex() lexes the line
    @note the return is a SIGNAL, returns SIG_SUCCESS if successfully lexed else SIG_FAILURE
    */
    inline std::string lex()
    {
        std::size_t index = 0;
        char current = global._line[0];
        bool is_dq = false, is_sq = false;
        while (index < global._line.size() && current != '`')
        {
            if IS_NUM (current)
            {
                std::string _temp;
                while (IS_NUM(current) || (current == '.') && current != '`')
                {
                    _temp += current;
                    index++;
                    current = global._line[index];
                }
                std::pair<std::string, std::string> _num_type = get_num_type(_temp);
                if (_num_type.first != SIG_FAILURE)
                {
                    this->_tok_list.push_back(_num_type);
                    global.update_col(_num_type.second.size());
                }
                else
                {
                    err.err_length = _temp.size();
                    err._error_type = UNKNOWN_TOKEN;
                    return SIG_FAILURE;
                }
            }
            else if (current == '\"' || current == '\'')
            {
                std::string _temp;
                _temp += current;
                index++;
                this->_tok_list.push_back(get_operator_type(_temp));
                if (current == '\'')
                {
                    if (is_sq == true)
                    {
                        is_sq = false;
                    }
                    else
                    {
                        is_sq = true;
                    }
                }
                else
                {
                    if (is_dq == true)
                    {
                        is_dq = false;
                    }
                    else
                    {
                        is_dq = true;
                    }
                }
            }
            else if ((is_sq == true || is_dq == true) && index > 0 && (this->_tok_list[_tok_list.size() - 1].first == OPER_DQUOTE || this->_tok_list[_tok_list.size() - 1].first == OPER_SQUOTE))
            {
                std::string _temp;
                if (is_sq == true)
                {
                    while (current != '\'' && index < global._line.length())
                    {
                        if (current == '\\')
                        {
                            index++;
                            current = global._line[index];
                        }
                        index++;
                        _temp += current;
                        current = global._line[index];
                    }
                }
                if (is_dq == true)
                {
                    while (current != '\"' && index < global._line.length())
                    {
                        if (current == '\\')
                        {
                            index++;
                            current = global._line[index];
                        }
                        index++;
                        _temp += current;
                        current = global._line[index];
                    }
                }
                this->_tok_list.push_back(std::make_pair(INQUOTE, _temp));
            }
            else if OPERS (current)
            {
                std::string _temp;
                _temp += current;
                index++;
                current = global._line[index];
                if ((_temp == "+" || _temp == "-") && (current == '+' || current == '-') && (OPERS(current))) // this is where the possible double operators are defined
                {
                    _temp += current;
                    index++;
                    global.update_col(2);
                }
                else if (OPERS(current))
                {
                    err.err_length = _temp.size() + 1;
                    err._error_type = UNKNOWN_TOKEN;
                    return SIG_FAILURE;
                }
                this->_tok_list.push_back(get_operator_type(_temp));
            }
            else if (current == '(' || current == ')')
            {
                index++;
                std::string _temp;
                _temp += current;
                this->_tok_list.push_back(get_operator_type(_temp));
            }
            else if (IS_ALPHA(current))
            {
                std::string _temp;
                _temp += current;
                index++;
                current = global._line[index];
                while (IS_ALPHA(current) && index < global._line.length())
                {
                    _temp += current;
                    index++;
                    current = global._line[index];
                }
                std::pair<std::string, std::string> _res = get_key_type(_temp);
                if (_res.first != SIG_FAILURE)
                {
                    this->_tok_list.push_back(_res);
                }
                else
                {
                    _res = is_identifier(_temp);
                    if (_res.first != SIG_FAILURE)
                        this->_tok_list.push_back(_res);
                    else
                    {
                        err.err_length = global._line.size() - 1;
                        err._error_type = UNKNOWN_TOKEN;
                        return SIG_FAILURE;
                    }
                }
            }
            else if (current == '|' || current == '&')
            {
                std::string _temp;
                _temp += current;
                index++;
                if (global._line[index] != _temp[0])
                    return SIG_FAILURE;
                else
                {
                    _temp += current;
                    index++;
                }
                this->_tok_list.push_back(get_operator_type(_temp));
            }
            else if (current == '!' || current == '>' || current == '<' || current == '=')
            {
                std::string _temp;
                _temp += current;
                index++;
                if (global._line[index] == '=')
                {
                    current = global._line[index];
                    _temp += current;
                    index++;
                }
                this->_tok_list.push_back(get_operator_type(_temp));
            }
            else if (current == ' ')
            {
                index++;
                global.update_col(1);
            }
            else if (current == ';')
            {
                std::string _temp;
                _temp+=current;
                index++;
                this->_tok_list.push_back(get_operator_type(_temp));
            }
            else
            {
                err.err_length = global._line.size() - 1;
                err._error_type = UNKNOWN_TOKEN;
                return SIG_FAILURE;
            }
            current = global._line[index];
        }
        return SIG_SUCCESS;
    }

    inline std::vector<std::pair<std::string, std::string>> &get_tokens()
    {
        return _tok_list;
    }

private:
    std::vector<std::pair<std::string, std::string>> _tok_list;
};

#endif