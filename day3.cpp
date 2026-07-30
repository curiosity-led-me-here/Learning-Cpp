#include <iostream>

int main()
{
    int x = 4;
    int* p = &x;
    *p = 5;
    
    std::cout << &x << '\n';
    std::cout << p << '\n';
    std::cout << *p << '\n';
    
    return 0;
}
