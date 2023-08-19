#ifndef __PARSER__
#define __PARSER__

#include "lexer.hpp"
#include <stack>
#include <queue>
#include "interpreter.hpp"

class Parser
{
public:
    Parser(std::vector<std::pair<std::string, std::string>> &_tok_list)
        : _toks_list(_tok_list)
    {
        global.current_context = PARSING;
        global.column = 0;
        this->_current = this->_toks_list[0];
    }

    inline std::string parse()
    {
        std::size_t len = this->_toks_list.size();
        while (ind < len)
        {
            if (_current.first == TYPE_FLOAT || _current.first == TYPE_NUM || _current.first == OPER_OPAREN || _current.first == OPER_MINUS || _current.first == OPER_INC || _current.first == OPER_DEC || _current.first == OPER_NOT || _current.first == KEYNOT || _current.first == TYPE_FALSE || _current.first == TYPE_TRUE)
            {
                std::string res = this->parseExpr();
                if (res == SIG_FAILURE) // this currently assumes that only numerical statements are expressions which will change
                {
                    return SIG_FAILURE;
                }
            }
            else if (_current.first == OPER_DQUOTE || _current.first == OPER_SQUOTE)
            {
                std::string res = this->parseString();
                if (res == SIG_FAILURE) // this currently assumes that only numerical statements are expressions which will change
                {
                    return SIG_FAILURE;
                }
            }
            else if (_current.first == KEY_BOOL || _current.first == KEY_CHAR || _current.first == KEY_FLOAT || _current.first == KEY_NUM || _current.first == KEY_STRING)
            {
                std::string res = this->parseVarDeclr();
                if (res == SIG_FAILURE) // this currently assumes that only numerical statements are expressions which will change
                {
                    return SIG_FAILURE;
                }
            }
            else
            {
                return SIG_FAILURE;
            }
        }
        return SIG_SUCCESS;
    }

    inline std::string parseExpr() // parses only mathematical expressions  not the boolean and string expressions
    {
        std::stack<std::pair<std::string, std::string>> _oper_stack;
        std::queue<std::pair<std::string, std::string>> _final;
        int o_paren = 0, c_paren = 0;
        while (ind < this->_toks_list.size() || this->_toks_list[ind].first != OPER_EOS)
        {
            if (_current.first == TYPE_FLOAT || _current.first == TYPE_NUM || _current.first == TYPE_FALSE || _current.first == TYPE_TRUE)
            {
                _final.push(_current);
            }
            else if (ind == 0 && _current.second == "-" || ind >= 1 && !_oper_stack.empty() && _current.second == OPER_MINUS && _oper_stack.top().first == OPER_OPAREN && this->_toks_list[ind - 1].first != TYPE_FLOAT && this->_toks_list[ind - 1].first != TYPE_NUM)
            {
                _final.push(std::make_pair(TYPE_NUM, "0"));
                ind++;
                if (this->_toks_list[ind].first == TYPE_FLOAT || this->_toks_list[ind].first == TYPE_NUM || this->_toks_list[ind].first == OPER_OPAREN)
                {
                    ind--;
                }
                else
                {
                    err.err_length = this->_toks_list[ind].second.length();
                    err._error_level = FATAL;
                    err._error_type = EXPECTED_NUMBER;
                    return SIG_FAILURE;
                }
                _oper_stack.push(_current);
            }
            else if (_current.first == OPER_DEC || _current.first == OPER_INC)
            {
                if (_current.first == OPER_DEC)
                {
                    _final.push(std::make_pair(TYPE_NUM, "1"));
                    _oper_stack.push(std::make_pair(OPER_MINUS, "-"));
                }
                else if (_current.first == OPER_INC)
                {
                    _final.push(std::make_pair(TYPE_NUM, "1"));
                    _oper_stack.push(std::make_pair(OPER_MINUS, "+"));
                }
            }
            else if (_current.first == OPER_PLUS || _current.first == OPER_DIV || _current.first == OPER_MINUS || _current.first == OPER_MUL || _current.first == OPER_AND || _current.first == KEY_AND || _current.first == OPER_IS_EQUAL || _current.first == OPER_IS_NOT_EQUAL || _current.first == OPER_NOT || _current.first == OPER_OR || _current.first == KEY_OR || _current.first == KEYNOT || _current.first == OPER_IS_EQUAL || _current.first == OPER_IS_NOT_EQUAL || _current.first == GREAT_THAN || _current.first == GREATER || _current.first == LESS || _current.first == LESS_THAN || _current.first == OPER_MOD)
            {
                while (!_oper_stack.empty() && operator_prcedence(_oper_stack.top().second) >= operator_prcedence(_current.second))
                {
                    _final.push(_oper_stack.top());
                    _oper_stack.pop();
                }
                _oper_stack.push(_current);
            }
            else if (_current.first == OPER_OPAREN)
            {
                _oper_stack.push(_current);
                o_paren++;
            }
            else if (_current.first == OPER_CPAREN)
            {
                while (!_oper_stack.empty() && _oper_stack.top().second != "(")
                {
                    std::pair<std::string, std::string> t = _oper_stack.top();
                    _oper_stack.pop();
                    while (!_oper_stack.empty() && (operator_prcedence(_oper_stack.top().second) >= operator_prcedence(t.second)))
                    {
                        _final.push(_oper_stack.top());
                        _oper_stack.pop();
                    }
                    _final.push(t);
                    c_paren++;
                }
                if (!_oper_stack.empty())
                {
                    _oper_stack.pop();
                }
                else
                {
                    err._error_type = STRAY_PARENTHESIS;
                    err._error_level = FATAL;
                    return SIG_FAILURE;
                }
            }
            ind++;
            if (ind < this->_toks_list.size())
            {
                _current = this->_toks_list[ind];
            }
            global.update_col(_current.second.size());
        }
        while (!_oper_stack.empty() && operator_prcedence(_oper_stack.top().second) >= operator_prcedence(_current.second))
        {
            _final.push(_oper_stack.top());
            _oper_stack.pop();
        }
        while (!_oper_stack.empty())
        {
            _final.push(_oper_stack.top());
            _oper_stack.pop();
        }
        if (c_paren != o_paren)
        {
            err._error_level = FATAL;
            err._error_type = INCORRECT_STATEMENT;
            global.column = 0;
            err.err_length = global._line.length();
            return SIG_FAILURE;
        }
        Node node;
        node._type = EXPRESSION;
        ExprExecuter par(_final);
        std::string __res = par._execute_expression();
        if (__res != SIG_FAILURE)
        {
            node._toks.push(std::make_pair((__res == "true") ? TYPE_TRUE : TYPE_FALSE, __res));
            this->node_list.push_back(node);
        }
        else
        {
            return SIG_FAILURE;
        }
        return SIG_SUCCESS;
    }

    inline std::string parseString() // this function parses the expressions that include strings and characters
    {
        std::stack<std::pair<std::string, std::string>> _oper_stack; // in strings and characters only the + operation can be done
        std::queue<std::pair<std::string, std::string>> _final;
        while (ind < this->_toks_list.size() && this->_toks_list[ind].first != OPER_EOS)
        {
            if (_current.first == OPER_DQUOTE || _current.first == OPER_SQUOTE)
            {
                std::string _val;
                bool _d_quote;
                if (_current.first == OPER_DQUOTE)
                {
                    _d_quote = true;
                }
                else
                {
                    _d_quote = false;
                }
                ind++;
                if (ind < this->_toks_list.size())
                {
                    _current = this->_toks_list[ind];
                }
                else
                {
                    err.err_length = _current.second.length();
                    global.column = global._line.find_first_of(_current.second);
                    err._error_level = FATAL;
                    err._error_type = STRAY_OPERATOR;
                    return SIG_FAILURE;
                }
                if (_current.first != INQUOTE && _current.first != OPER_DQUOTE && _current.first != OPER_SQUOTE)
                {
                    err.err_length = _current.second.length();
                    global.column = global._line.find_first_of(_current.second);
                    err._error_level = FATAL;
                    err._error_type = EXPECTED_STR;
                    return SIG_FAILURE;
                }
                if (_d_quote == true)
                {
                    while (ind < this->_toks_list.size() && _current.first != OPER_DQUOTE)
                    {
                        _val += _current.second;
                        ind++;
                        if (ind < this->_toks_list.size())
                        {
                            _current = this->_toks_list[ind];
                        }
                        else if (ind > this->_toks_list.size())
                        {
                            err.err_length = global._line.length() - 1;
                            global.column = 1;
                            err._error_level = FATAL;
                            err._error_type = STRAY_OPERATOR;
                            return SIG_FAILURE;
                        }
                    }
                    _final.push(std::make_pair(INQUOTE, _val));
                    ind++;
                }
                else if (_d_quote == false)
                {
                    if (_current.first == INQUOTE)
                    {
                        if (_current.second.length() > 1)
                        {
                            err.err_length = _current.second.length();
                            err._error_level = FATAL;
                            err._error_type = CHAR_LIMIT;
                            global.column = 1;
                            return SIG_FAILURE;
                        }
                        _val += _current.second;
                        ind++;
                    }
                    if (ind < this->_toks_list.size())
                    {
                        _current = this->_toks_list[ind];
                    }
                    else
                    {
                        err.err_length = global._line.length() - 1;
                        global.column = 1;
                        err._error_level = FATAL;
                        err._error_type = STRAY_OPERATOR;
                        return SIG_FAILURE;
                    }
                    if (_current.first != OPER_SQUOTE)
                    {
                        err.err_length = global._line.length() - 1;
                        global.column = 1;
                        err._error_level = FATAL;
                        err._error_type = STRAY_OPERATOR;
                        return SIG_FAILURE;
                    }
                    else
                    {
                        ind++;
                        _final.push(std::make_pair(INQUOTE, _val));
                    }
                }
            }
            else if (_current.first == OPER_PLUS)
            {
                _oper_stack.push(_current);
                ind++;
            }
            else if (_current.first == INQUOTE)
            {
                _final.push(_current);
                ind++;
            }
            else
            {
                err.err_length = _current.second.length();
                global.column = global._line.find_first_of(_current.second);
                err._error_level = FATAL;
                err._error_type = UNSUPPORTED_OPER;
                return SIG_FAILURE;
            }
            if (ind < this->_toks_list.size())
            {
                _current = this->_toks_list[ind];
            }
        }
        while (!_oper_stack.empty())
        {
            _final.push(_oper_stack.top());
            _oper_stack.pop();
        }
        Node node;
        node._type = EXPRESSION;
        ExprExecuter par(_final);
        std::string __res = par._execute_string();
        node._toks.push(std::make_pair(INQUOTE, __res));
        this->node_list.push_back(node);
        return SIG_SUCCESS;
    }

    inline std::string parseVarDeclr()
    {
        std::queue<std::pair<std::string, std::string>> _datas;
        std::string _type;
        bool eos = false;
        if (ind == 0 && _current.first == KEY_BOOL || _current.first == KEY_CHAR || _current.first == KEY_FLOAT || _current.first == KEY_NUM || _current.first == KEY_STRING)
        {
            _datas.push(_current);
            ind++;
            _type = _current.first;
        }
        if (ind == 1 && this->_toks_list[ind].first == IDENTIFIER)
        {
            _datas.push(this->_toks_list[ind]);
            ind++;
        }
        if (ind == 2 && this->_toks_list[ind].first == OPER_EQUAL || this->_toks_list[ind].first == OPER_EOS)
        {
            if (this->_toks_list[ind].first == OPER_EOS)
            {
                eos = true;
            }
            if (eos == true)
            {
                if (_type == KEY_CHAR)
                    _datas.push(std::make_pair(INQUOTE, " "));
                else if (_type == KEY_STRING)
                    _datas.push(std::make_pair(INQUOTE, " "));
                else if (_type == KEY_BOOL)
                    _datas.push(std::make_pair(TYPE_FALSE, "false"));
                else if (_type == KEY_NUM)
                    _datas.push(std::make_pair(TYPE_NUM, "0"));
                else if (_type == KEY_FLOAT)
                    _datas.push(std::make_pair(TYPE_FLOAT, "0.00"));
                ind++;
            }
            else
            {
                ind++;
            }
        }
        if (eos == false && ind == 3)
        {
            std::string _res;
            std::pair<std::string, std::string> ans;
            _current = this->_toks_list[ind];
            if (_current.first == OPER_DQUOTE || _current.first == OPER_SQUOTE)
            {
                _res = this->parseString();
            }
            else
            {
                _res = this->parseExpr();
            }
            if (_res != SIG_FAILURE)
            {
                ans = this->node_list[this->node_list.size() - 1]._toks.front();
                this->node_list.pop_back();
            }
            else
            {
                return SIG_FAILURE;
            }
            if (_type == KEY_BOOL && ans.first == TYPE_TRUE || ans.first == TYPE_FALSE)
                _datas.push(ans);
            else if (_type == KEY_CHAR && ans.first == INQUOTE)
                _datas.push(ans);
            else if (_type == KEY_NUM && ans.first == TYPE_NUM)
                _datas.push(ans);
            else if (_type == KEY_FLOAT && ans.first == TYPE_FLOAT)
                _datas.push(ans);
            else if (_type == KEY_STRING && ans.first == INQUOTE)
                _datas.push(ans);
            else
            {
                return SIG_FAILURE; // incorrect data types
            }
        }
        if (this->_toks_list[this->_toks_list.size() - 1].first == OPER_EOS)
        {
            ind++;
        }
        else
        {
            return SIG_FAILURE; // expected eos
        }
        if (_datas.size() != 3)
        {
            return SIG_FAILURE; // wrong declaration
        }
        else
        {
            Node node;
            node._type = VAR_DECLR;
            node._toks = _datas;
            this->node_list.push_back(node);
        }
        return SIG_SUCCESS;
    }

    inline std::vector<Node> _get_nodes()
    {
        return this->node_list;
    }

private:
    std::vector<std::pair<std::string, std::string>> _toks_list;
    std::vector<Node> node_list;
    std::size_t ind = 0;
    std::pair<std::string, std::string> _current;
};

#endif