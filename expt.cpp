#include <iostream>
#include <vector>


int main()
{
    std::vector<int> X;
    X.push_back(0);
    for (int i : X)
    {
	std::cout << i;
    }
    return 0;
}
