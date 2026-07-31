#include <iostream>
#include <vector>

template <typename T>
void print(std::vector<T> values)
{
    for (int i=0 ; i < values.size(); i++)
    {
	if (i == 0)
	{
	    std::cout << "[";
	}
	std::cout << values[i];
	if (i == values.size()-1)
	{
	    std::cout << "]";
	}
	else
	{
	    std::cout << ", ";
	}
    }
}

std::vector<int> strides(std::vector<int> dims)
{
    std::vector<int> out;
    for (int j=0; j < dims.size()-1; j++)
    {
	int a = 1;
	for (int i=j+1; i < dims.size(); i++)
	{
	    a = a*dims[i];
	}
	out.push_back(a);
    }
    out.push_back(1);
    return out;
}



int main()
{
    std::vector<int> dims = {5,3,1,5,8};
    std::vector<int> out = strides(dims);
    print(out);
}
