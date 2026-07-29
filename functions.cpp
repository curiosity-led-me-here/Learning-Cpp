#include <iostream>
#include <vector>
#include <random>
#include <cmath>

double log(double number, double base)
{
    return std::log(number) / std::log(base);
}

std::vector<int> binary(int x, int max_value)
{
    int closest_pow = std::floor(log(max_value, 2));
    if (x == 0)
    {
	return std::vector<int>(closest_pow, 0);
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

int max(const std::vector<int>& x)
{
    int max_val = x[0];
    for (int i=1; i < x.size(); i++)
    {
	if x[i] > max_val
	{
	    max_val = x[i];
	}
    }
    return max_val;
}
