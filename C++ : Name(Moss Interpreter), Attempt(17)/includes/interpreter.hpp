#ifndef __INTERPRETER__
#define __INTERPRETER__

#include "../misc/constants.hpp"
#include "../misc/helpingfuncs.hpp"
#include "parser.hpp"

/*Creates ByteCode to be executed by the Bytecode executor*/
class ExprExecuter
{
public:
    ExprExecuter(std::queue<std::pair<std::string, std::string>> &toks)
    {
        this->_nodes = toks;
    }

    inline std::string _execute_expression()
    {
        std::stack<std::pair<std::string, std::string>> _num_stack;
        std::size_t len = this->_nodes.size();
        std::size_t ind = 0;
        std::pair<std::string, std::string> _curr = this->_nodes.front();
        while (ind < len)
        {
            if (_curr.first == TYPE_FLOAT || _curr.first == TYPE_NUM || _curr.first == TYPE_TRUE || _curr.first == TYPE_FALSE)
            {
                _num_stack.push(_curr);
            }
            else if (_curr.first == OPER_PLUS || _curr.first == OPER_DIV || _curr.first == OPER_MINUS || _curr.first == OPER_MUL || _curr.first == OPER_MOD)
            {
                if (_num_stack.empty())
                {
                    err.err_length = _curr.second.length();
                    global.column = global._line.find_first_of(_curr.second);
                    err._error_level = FATAL;
                    err._error_type = STRAY_OPERATOR;
                    return SIG_FAILURE;
                }
                std::pair<std::string, std::string> _left = _num_stack.top();
                _num_stack.pop();
                if (_num_stack.empty())
                {
                    err.err_length = _curr.second.length();
                    global.column = global._line.find_first_of(_curr.second);
                    err._error_level = FATAL;
                    err._error_type = STRAY_OPERATOR;
                    return SIG_FAILURE;
                }
                std::pair<std::string, std::string> _right = _num_stack.top();
                _num_stack.pop();
                std::string res = _solve(_left, _right, _curr.second);
                if (res != SIG_FAILURE)
                {
                    _num_stack.push(get_num_type(res));
                }
                else
                {
                    return SIG_FAILURE;
                }
            }
            else if (_curr.first == OPER_AND || _curr.first == KEY_AND || _curr.first == OPER_IS_EQUAL || _curr.first == OPER_IS_NOT_EQUAL || _curr.first == OPER_OR || _curr.first == OPER_OR || _curr.first == KEY_OR || _curr.first == KEYNOT || _curr.first == GREAT_THAN || _curr.first == GREATER || _curr.first == LESS || _curr.first == LESS_THAN)
            {
                if (_num_stack.empty())
                {
                    err.err_length = _curr.second.length();
                    global.column = global._line.find_first_of(_curr.second);
                    err._error_level = FATAL;
                    err._error_type = STRAY_OPERATOR;
                    return SIG_FAILURE;
                }
                std::pair<std::string, std::string> _left = _num_stack.top();
                _num_stack.pop();
                if (_num_stack.empty())
                {
                    err.err_length = _curr.second.length();
                    global.column = global._line.find_first_of(_curr.second);
                    err._error_level = FATAL;
                    err._error_type = STRAY_OPERATOR;
                    return SIG_FAILURE;
                }
                std::pair<std::string, std::string> _right = _num_stack.top();
                _num_stack.pop();
                std::string res = __bool_logic(_left, _right, _curr.second);
                _num_stack.push(std::make_pair((res == "true") ? TYPE_TRUE : TYPE_FALSE, res));
            }
            else if (_curr.first == OPER_NOT || _curr.first == KEYNOT)
            {
                if (_num_stack.empty())
                {
                    err.err_length = _curr.second.length();
                    global.column = global._line.find_first_of(_curr.second);
                    err._error_level = FATAL;
                    err._error_type = STRAY_OPERATOR;
                    return SIG_FAILURE;
                }
                std::pair<std::string, std::string> _left = _num_stack.top();
                _num_stack.pop();
                std::string res = _bool_not(_left);
                _num_stack.push(std::make_pair((res == "true") ? TYPE_TRUE : TYPE_FALSE, res));
            }
            this->_nodes.pop();
            ind++;
            if (ind < len)
                _curr = this->_nodes.front();
        }
        return _num_stack.top().second;
    }

    inline std::string _execute_string()
    {
        std::stack<std::pair<std::string, std::string>> _ans;
        std::size_t len = this->_nodes.size();
        std::size_t ind = 0;
        std::pair<std::string, std::string> _curr = this->_nodes.front();
        while (ind < len)
        {
            if (_curr.first == INQUOTE)
            {
                _ans.push(_curr);
            }
            else if (_curr.first == OPER_PLUS)
            {
                if (_ans.empty())
                {
                    err.err_length = _curr.second.length();
                    global.column = global._line.find_first_of(_curr.second);
                    err._error_level = FATAL;
                    err._error_type = STRAY_OPERATOR;
                    return SIG_FAILURE;
                }
                std::pair<std::string, std::string> _left = _ans.top();
                _ans.pop();
                if (_ans.empty())
                {
                    err.err_length = _curr.second.length();
                    global.column = global._line.find_first_of(_curr.second);
                    err._error_level = FATAL;
                    err._error_type = STRAY_OPERATOR;
                    return SIG_FAILURE;
                }
                std::pair<std::string, std::string> _right = _ans.top();
                _ans.pop();
                std::string _res = _concat(_left, _right);
                _ans.push(std::make_pair(INQUOTE, _res));
            }
            this->_nodes.pop();
            ind++;
            if (ind < len)
                _curr = this->_nodes.front();
        }
        return _ans.top().second;
    }

private:
    std::queue<std::pair<std::string, std::string>> _nodes;
};

#endif