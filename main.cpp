#include <iostream>
#include <vector>
#include <random>
#include <cmath>

// Customizable log base
double log(double number, double base)
{
    return std::log(number) / std::log(base);
}

// Binary Converter
std::vector<int> binary(int x, int max_value)
{
    int closest_pow = std::floor(log(max_value, 2));
    if (x == 0)
    {
	return std::vector<int>(closest_pow+1, 0);
    }
    int size = closest_pow;
    std::vector<int> bins(size+1, 0);
    while (x != 0)
    {
	closest_pow = std::floor(log(x,2));
	bins[size - closest_pow]++;
	x -= std::pow(2, closest_pow);
    }
    return bins;
}

// Addition operator
 std::vector<int> operator+(const std::vector<int>& X, const std::vector<int>& other)
 {
     std::vector<int> result = X;
     if (X.size() != other.size())
     {
	 throw std::invalid_argument("Shape does not match!");
     }
     
     for (int row=0; row < X.size(); row++)
     {
	 result[row] = X[row] + other[row];
     }
     return result;
 }
    
// Bitwise XOR
std::vector<int> bxor(std::vector<int> bins)
{
    std::vector<int> parsed = binary(0, bins.size());
    for (int x=0; x < bins.size(); x++)
    {
	if (bins[x] == 1)
	{
	    std::vector<int> a = binary(x+1, bins.size());
	    parsed = parsed + a;
	}
    }
    for (int y=0; y < bins.size(); y++)
    {
	parsed[y] = parsed[y] % 2;
    }
    return parsed;
}

// Placing correctors
std::vector<int> fill(std::vector<int> bits)
{
    int closest_pow = std::ceil(log(bits.size(), 2)) + 1;
}



// Main
int main()
{
    std::vector<int> raw_bins = {0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1};
    std::vector<int> bins = bxor(raw_bins);
    for (int i : bins)
    {
	std::cout << i;
    }
    return 0;
}
