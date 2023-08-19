#ifndef __BYTECODE__
#define __BYTECODE__

#include "bytecode_generator.hpp"

class ByteCode_read
{
    public:
    ByteCode_read(ByteGen &gen)
    {
        this->_codes = gen.get_code();
    }

    inline void read()
    {
        std::size_t len = this->_codes.size();
        std::size_t ind = 0;
        while (ind < len)
        {
            if(this->_codes[ind].first == LINE)
            {
                std::cout<<this->_codes[ind].second.front().second<<std::endl;
            }
            ind++;
        }
    }

private:
    std::vector<std::pair<std::string, std::queue<std::pair<std::string, std::string>>>> _codes;
};

#endif