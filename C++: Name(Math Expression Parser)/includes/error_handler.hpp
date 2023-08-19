#ifndef _ERR_
#define _ERR_

#include<iostream>

enum Error_level{
    TERMINATE_EXEC = 1,
    EXIT_PROG,
};

inline void _invalid_number(std::string _num)
{
    std::cout<<"The given number\""<<_num<<"\" is not a valid number."<<std::endl;
}

inline void _invalid_oper(std::string _oper)
{
    std::cout<<"The given character \'"<<_oper<<"\' is invalid"<<std::endl;
}

inline void _stary_paren()
{
    std::cout<<"Stary ("<<std::endl;
}

#endif