#include <iostream>
#include <string>
int main()
{
    std::string buffer;
    std::cin >> buffer;
    std::cout << buffer.c_str() << std::endl;
    return 0;
}