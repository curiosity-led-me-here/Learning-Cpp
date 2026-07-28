#include <iostream>
#include <vector>

int main()
{
    std::vector<int> vec;
    std::vector<int> bits = {0,1,1,0,0,1,0,1,0,0,1,0};

    for (int i=0; i <= 10; i++)
    {
	vec.push_back(i);
    }

    for (int j=0; j < vec.size(); j++)
    {
	std::cout << "The Index " << j;

	std::cout << " contains the element: " << vec[j] << "\n";
    }

    int counts = 0;

    for (int b: bits)
    {
	if (b == 1)
	{
	    counts++;
	}
    }

    std::cout << "Number of ones: " <<  counts << '\n';
    
     return 0;
}
