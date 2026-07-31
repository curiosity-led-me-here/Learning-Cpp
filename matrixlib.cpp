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

enum class overwrite
    {
	True,
	False
    };

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

template <typename T>
T elesum(std::vector<T> arr)
{
    T a = 0;
    for (T t : arr)
    {
	a += t;
    }
    return a;
}

template <typename T>
T elemult(std::vector<T> arr)
{
    T a = 1;
    for (T t : arr)
    {
	a *= t;
    }
    return a;
}

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


class Matrix
{
private:
    std::vector<double> data;
    std::vector<int> dimensions;
    std::vector<int> strides;
    std::vector<int> get_strides(std::vector<int> dims)
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
public:
    // constructor overloading
    Matrix(const std::vector<double>& values, const std::vector<int>& dims) : data(values), dimensions(dims), strides(get_strides(dims))
    {
	if (strides[0]*dimensions[0] != data.size())
	{
	    throw std::invalid_argument("Dimensions do not match with flattened data vector!");
	}
    };
    Matrix(const std::vector<int>& dims) : data(build(dims)), dimensions(dims), strides(get_strides(dimensions)) {};

    std::vector<int> shape()
    {
	return dimensions;
    }
    
    const std::vector<int>& get_strides() const
    {
        return strides;
    }

    void print_list(int depth, int& data_index) const
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
		print_list(depth+1, data_index);
		if (j != dimensions[depth]-1)
		{
		    std::cout << ",\n";
		}
	    }
	}
	
	std::cout << "]";
    }

    void print() const
    {
	int data_index = 0;
	print_list(0, data_index);
    }
    
    void reshape(std::vector<int> new_dims)
    {
	dimensions = new_dims;
    }

    int get_item(std::vector<int> coords)
    {
	if (coords.size() != dimensions.size())
	{
	    throw std::invalid_argument("Invalid indices!");
	}
	int coord = 0;
	for (int i=0; i < coords.size(); i++)
	{
	    if (coords[i] >= dimensions[i])
	    {
		throw std::invalid_argument("Index out of bounds!");
	    }
	    coord += strides[i] * coords[i];
	}
       return coord;
    }
    
    Matrix dot(Matrix& other)
    // (a, b) x (b, c) --> (a, c)
    {
	std::vector<double> out;
	for (int i=0; i < dimensions[0]; i++)
	{
	    for(int j=0; j < other.shape()[1]; j++)
	    {
		double ele = 0;
		for (int k=0; k < dimensions[1]; k++)
		{
		    ele += data[get_item({i, k})] * other.data[other.get_item({k, j})];
		}
		out.push_back(ele);
	    }
	}
	Matrix output(out, {dimensions[0], other.shape()[1]});
	return output;
    }
    
};

// batched prod
// tensor contraction

int main()
{
    Matrix A({4,2});
    std::cout << "Old matrix";
    std::cout << '\n';
    A.print();
    Matrix B({2,3});
    std::cout << '\n';
    std::cout << "New matrix";
    std::cout << '\n';
    B.print();
    std::cout << '\n';
    Matrix C = A.dot(B);
    std::cout << "Dotted Matrix";
    std::cout << '\n';
    C.print();
    return 0;
}
