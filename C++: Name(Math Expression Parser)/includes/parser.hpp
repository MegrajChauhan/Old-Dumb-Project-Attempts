#ifndef _PARSER_
#define _PARSER_

// 2 + 3 - 4 * 8: look for - and if none exist look for +, if - is found look at the left and right
// method used: 2 + 3: parsed to 2 3 +, 2 + 3 * 4, 3 4 * 2 +, (3+2) * 3, 3 2 + 3 *

#include "lexer.hpp"
#include <stack>
#include <queue>
#include <sstream>

int to_int(std::string num)
{
    return std::stoi(num);
}

float to_float(std::string num)
{
    float f;
    sscanf(num.c_str(), "%f", &f);
    return f;
}

std::string to_str(auto in)
{
    std::stringstream stream;
    std::string str;
    stream << in;
    stream >> str;
    return str;
}

inline int getPrecedence(Token_t tokenType)
{
    switch (tokenType)
    {
    case PLUS:
    case MINUS:
        return 1;
    case MUL:
    case DIV:
    case MOD:
        return 2;
    case POW:
        return 3;
    default:
        return 0;
    }
}

inline Token_t analyze(std::string _tok)
{
    int dot_count = 0;
    Token_t type = NUM;
    int _corr_count = 0;
    for (char _c : _tok)
    {
        if (is_num(_c))
        {
            _corr_count++;
        }
        else if (_c == '.')
        {
            dot_count++;
            type = FLOAT;
        }
    }
    return type;
}

class Parser
{
    std::vector<Tokens> _tok_list;
    std::queue<Tokens> _stack;
    int pos;
    Tokens curr;

public:
    Parser(Lexer &lex)
    {
        this->_tok_list = lex.get_tok_list();
        this->init();
    }

    inline void init()
    {
        this->pos = 0;
        this->curr = this->_tok_list[this->pos];
    }

    inline void update()
    {
        this->pos++;
        if (this->pos < this->_tok_list.size())
        {
            this->curr = this->_tok_list[this->pos];
        }
        else
        {
            this->curr = Tokens(STOP, std::string(""));
        }
    }

    inline int parse()
    {
        // iterate through the tokens list
        std::stack<Tokens> _op_stack;
        while (this->curr.type != STOP)
        {
            if (this->curr.type == NUM || this->curr.type == FLOAT)
            {
                this->_stack.push(this->curr);
                this->update();
            }
            else if (is_oper(this->curr))
            {
                switch (this->curr.type)
                {
                case PLUS:
                case MINUS:
                case MUL:
                case DIV:
                {
                    while (_op_stack.empty() == false && (getPrecedence(_op_stack.top().type) >= getPrecedence(this->curr.type)))
                    {
                        this->_stack.push(_op_stack.top());
                        _op_stack.pop();
                    }
                    _op_stack.push(this->curr);
                    this->update();
                    break;
                }
                }
            }
            else if (this->curr.type == OPAREN)
            {
                _op_stack.push(this->curr);
                this->update();
            }
            else if (this->curr.type == CPAREN)
            {
                while (!_op_stack.empty() && _op_stack.top()._value != "(")
                {
                    Tokens t = _op_stack.top();
                    _op_stack.pop();
                    while (!_op_stack.empty() && (getPrecedence(_op_stack.top().type) >= getPrecedence(t.type)))
                    {
                        this->_stack.push(_op_stack.top());
                        _op_stack.pop();
                    }
                    _stack.push(t);
                }
                if (!_op_stack.empty())
                {
                    _op_stack.pop();
                }
                else
                {
                    _stary_paren();
                    return -1;
                }

                this->update();
            }
        }
        while (!_op_stack.empty())
        {
            this->_stack.push(_op_stack.top());
            _op_stack.pop();
        }
        return 1;
    }

    inline void print()
    {
        while (!this->_stack.empty())
        {
            std::cout << this->_stack.front()._value << std::endl;
            this->_stack.pop();
        }
    }

    inline std::string evaluate()
    {
        std::stack<std::string> _outStack;
        while (!this->_stack.empty())
        {
            Tokens token = _stack.front();
            _stack.pop();
            switch (token.type)
            {
            case NUM:
            case FLOAT:
                _outStack.push(token._value);
                break;
            case PLUS:
            {
                std::string _left = _outStack.top();
                auto left = analyze(_left) == NUM ? to_int(_left) : to_float(_left);
                _outStack.pop();
                std::string _right = _outStack.top();
                auto right = analyze(_right) == NUM ? to_int(_right) : to_float(_right);
                _outStack.pop();
                _outStack.push(to_str(left + right));
                break;
            }
            case MINUS:
            {
                std::string _left;
                if (_outStack.empty())
                {
                    _left = "0";
                }
                else
                {
                    _left = _outStack.top();
                }
                auto left = analyze(_left) == NUM ? to_int(_left) : to_float(_left);
                _outStack.pop();
                std::string _right;
                if (_outStack.empty())
                {
                    _right = "0";
                }
                else
                {
                    _right = _outStack.top();
                }
                auto right = analyze(_right) == NUM ? to_int(_right) : to_float(_right);
                if (!_outStack.empty())
                {
                    _outStack.pop();
                }
                _outStack.push(to_str(right - left));
                break;
            }
            case MUL:
            {
                std::string _left = _outStack.top();
                auto left = analyze(_left) == NUM ? to_int(_left) : to_float(_left);
                _outStack.pop();
                std::string _right = _outStack.top();
                auto right = analyze(_right) == NUM ? to_int(_right) : to_float(_right);
                _outStack.pop();
                _outStack.push(to_str(right * left));
                break;
            }
            case DIV:
            {
                std::string _left = _outStack.top();
                auto left = analyze(_left) == NUM ? to_int(_left) : to_float(_left);
                _outStack.pop();
                std::string _right = _outStack.top();
                auto right = analyze(_right) == NUM ? to_int(_right) : to_float(_right);
                _outStack.pop();
                if (left == 0)
                {
                    std::cout << "Divided by zero" << std::endl;
                    _outStack.push("0");
                }
                else
                {
                    _outStack.push(to_str(right / left));
                }
                break;
            }
            }
        }
        return _outStack.top();
    }
};

#endif