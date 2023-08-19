#include<iostream>
#include"../includes/Api.hpp"

int main()
{
  while(true)
  {
    std::string _expr;
    std::cout<<">>>";
    std::getline(std::cin, _expr);
    API pi(_expr);
    int _ret_code = pi.exec();
    if(_ret_code != -1)
    {
      // std::cout<<pi.get_res()<<std::endl;
    }
  }
}