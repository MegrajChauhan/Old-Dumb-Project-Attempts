#ifndef _LEXER_
#define _LEXER_

#include <string>
#include <vector>
#include "error_handler.hpp"

#define is_num(x) ((x >= '0') && (x <= '9'))
#define is_char(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && (x <= 'Z')))

template <typename T, typename U, typename V>
struct Tri_pair
{
    T _first;
    U _second;
    V _third;

public:
    Tri_pair(T a, U b, V c)
    {
        _first = a;
        _second = b;
        _third = c;
    }
};

// template <typename A, typename B, typename C>
// inline Tri_pair<A, B,C> make_tri_pair(A a, B b, C c)
// {
//   return Tri_pair(a, b, c);
// }

enum Token_t
{
    NUM = 1,
    FLOAT,
    PLUS,
    MINUS,
    DIV,
    MUL,
    MOD,
    POW,
    OPAREN,
    CPAREN,
    ERROR,
    STOP,
};

struct Tokens
{
    Token_t type;
    std::string _value;
};

inline bool is_oper(Tokens t)
{
    bool i = false;
    switch (t.type)
    {
    case PLUS:
        i = true;
    case MINUS:
        i = true;
    case MUL:
        i = true;
    case DIV:
        i = true;
    case POW:
        i = true;
    }
    return i;
}

inline bool oper(char c)
{
    bool r = false;
    switch(c)
    {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
           r = true;
    }
    return r;
}

inline Tri_pair<int, std::string, Token_t> token_type_number(std::string _tok)
{
    int dot_count = 0;
    Token_t type = NUM;
    int _corr_count = 0;
    bool _is_valid = true;
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
        dot_count > 1 ? _is_valid = false : _is_valid = true;
    }
    if (_is_valid == false)
    {
        return Tri_pair(-1, _tok, ERROR);
    }
    else
    {
        return Tri_pair(1, std::string(""), type);
    }
}

inline Tri_pair<int, std::string, Token_t> token_type_oper_and_symbol(std::string _tok)
{
    std::vector<std::pair<std::string, Token_t>> _oper_pair = {{"+", PLUS}, {"-", MINUS}, {"/", DIV}, {"*", MUL}, {"(", OPAREN}, {")", CPAREN}};
    for(std::pair<std::string, Token_t> p:_oper_pair)
    {
        if(_tok == p.first)
        {
          return Tri_pair(1, std::string(""), p.second);
        }
    }
     return Tri_pair(-2, _tok, ERROR);
}

class Lexer
{
public:
    inline void init()
    {
        this->index = 0;
        this->_current = this->_input[index];
    }

    inline void update()
    {
        this->index++;
        this->index < this->_input.size() ? this->_current = this->_input[index] : this->_current = ';';
    }

    Lexer(std::string expr)
    {
        this->_input = expr;
        this->init();
    }

    inline std::pair<int, Token_t> decode(Tri_pair<int, std::string, Token_t> pair)
    {
        if (pair._first > 0)
        {
            return std::make_pair(1, pair._third);
        }
        else
        {
            switch (pair._first)
            {
            case -1:
                _invalid_number(pair._second);
                return std::make_pair(-1, ERROR);
            case -2:
                _invalid_oper(pair._second);
                return std::make_pair(-1, ERROR);
            default:
            break;
            }
        }
        return std::make_pair(1, ERROR);
    }

    inline int lex()
    {
        while (this->_current != ';')
        {
            if (this->_current == ' ' || this->_current == '\t')
                this->update();
            else if (is_num(this->_current) || this->_current == '.')
            {
                std::string _num = " ";
                while (is_num(this->_current) || this->_current == '.')
                {
                    _num += this->_current;
                    this->update();
                }
                Tri_pair<int, std::string, Token_t> _p = token_type_number(_num);
                std::pair<int, Token_t> _val = this->decode(_p);
                if (_val.first == -1)
                {
                    return -1;
                }
                else
                {
                    this->_tok_list.push_back(Tokens(_val.second, _num));
                }
            }else{
                std::string _oper = "";
                _oper += this->_current;
                char _front = this->peek();
                if(oper(_front))
                {
                    this->_tok_list.push_back(Tokens(NUM, "0"));
                }
                this->update();
                Tri_pair<int, std::string, Token_t> _p = token_type_oper_and_symbol(_oper);
                std::pair<int, Token_t> _val = this->decode(_p);
                if (_val.first == -1)
                {
                    return -1;
                }
                else
                {
                    this->_tok_list.push_back(Tokens(_val.second, _oper));
                }
            }
        }
        return 1;
    }

    inline char peek()
    {
        return this->_input[this->index+1];
    }

    inline char peek_back()
    {
        return this->_input[this->index -1];
    }

    inline void print()
    {
        for (Tokens t : _tok_list)
        {
            std::cout << t.type << " " << t._value << std::endl;
        }
    }

    inline std::vector<Tokens> get_tok_list()
    {
        return this->_tok_list;
    }

private:
    std::string _input;
    char _current;
    int index = 0;
    std::vector<Tokens> _tok_list;
};

#endif