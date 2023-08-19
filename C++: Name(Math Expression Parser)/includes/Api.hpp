#ifndef _API_HPP_
#define _API_HPP_

#include "lexer.hpp"
#include "parser.hpp"

class API
{
public:
    API(std::string _val) { this->_inp = _val; }

    inline int exec()
    {
        Lexer lex(this->_inp);
        int _ret_code = lex.lex();
        if (_ret_code == -1)
        {
            return -1;
        }
        else
        {
            Parser p(lex);
            int x = p.parse();
            if (x != -1)
            {
                p.print();
                // _res = p.evaluate();
            }
            else
            {
                return -1;
            }
        }
        return 1;
    }

    inline std::string get_res()
    {
        return this->_res;
    }

private:
    std::string _inp;
    std::string _res;
};

#endif