#include <iostream>
#include <vector>
#include <string>
#include <random>

double randdouble(double min, double max)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

std::vector<double> build(const std::vector<int>& dims)
{
    int len = 1;
    for (int i : dims)
    {
	len = len*i;
    }
    std::vector<double> out;
    for (int i=0; i < len; i++)
    {
	double var = randdouble(0.0, 1.0);
	out.push_back(var);
    }
    return out;
}

class Matrix
{
private:
    std::vector<double> data;
    std::vector<int> get_strides(std::vector<double> input, std::vector<int> dimensions)
    {
	int l = 1;
	for (int i : dimensions)
	{
	    l = l * i;
	}
	if (l != input.size())
	{
	    throw std::invalid_argument("Incorrect dimension mapping. Mult(Dims) must match input size");
	}
	
	std::vector<int> strides;
	// Whatever dimensions are after current dimension, mult them up and append into the strides.
	// Nested loop
	for (int j=0; j < dimensions.size(); j++)
	{
	    int stride = 1;
	    for (int k=j+1; k < dimensions.size()-1; k++)
	    {
		stride = stride * k;
	    }
	    strides.push_back(stride);
	}
	return strides;
    }
public:
    const std::vector<int> dimensions;
    // constructor overloading
    Matrix(const std::vector<double>& values, const std::vector<int>& dims) : data(values), dimensions(dims) {};
    Matrix(const std::vector<int>& dims) : data(build(dims)), dimensions(dims) {};

    int get_item(std::vector<int> coordinate)
    {
	std::vector<int> strides = get_strides(data, dimensions);
	if (strides.size() != dimensions.size())
	{
	    throw std::invalid_argument("Stride dimension should be equal to dimension count");
	}

	if (dimensions.size() != coordinate.size())
	{
	    throw std::invalid_argument("Coordinates do not match the dimensions!");
	}
	
	for (int a=0; a < dimensions.size(); a++)
	{
	    if (coordinate[a] > dimensions[a])
	    {
		throw std::invalid_argument("Index out of bounds!");
	    }
	}
	
	int coord = 0;
	for (int i=0; i < strides.size(); i++)
	{
	    coord += strides[i]*coordinate[i];
	}
	double item = data[coord];
	std::cout << item << '\n';
	return item;
    }

    void print(int depth, int& data_index) const
    {
	std::cout << "[";

	if (depth == dimensions.size()-1)
	{
	    for (int j=0; j < dimensions[depth]; j++)
	    {
		std::cout << data[data_index];
		if (j != dimensions[depth]-1)
		{
		    std::cout << ", ";
		}
		data_index++;
	    }
	}
	else
	{
	    for (int j=0; j < dimensions[depth]; j++)
	    {
		print(depth+1, data_index);
		if (j != dimensions[depth]-1)
		{
		    std::cout << ",\n";
		}
	    }
	}
	
	std::cout << "]";
    }
};

// batched prod
// tensor contraction

int main()
{
    Matrix A({3, 5});
    int data_index = 0;
    A.print(0, data_index);
    
    return 0;

}
