#include <iostream>
#include <vector>


void print_layer(std::vector<int> data, std::vector<int> dims, int depth, int idx)
{
    std::cout << "[";

    if (depth == dims.size())
    {
	
    }
    
    std::cout << "]";
    
}

void recurse(int a)
{
    if (a == 10)
    {
	return;
    }

    else
    {
	recurse(a-1)
    }

}



int main()
{
    
    return 0;
}
