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

template <typename T>
void swap(std::vector<T>& inp, int a, int b)
{
    int last = inp[b];
    int target = inp[a];
    inp[b] = target;
    inp[a] = last;
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

    void place_item(std::vector<int> coords, double value)
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
	data[coord] = value;
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
    
    void squish_layer(int axis, int layer, std::vector<int> prefix, std::vector<double>& out, std::vector<int> dims)
    // (a, b, c, d) --> (a, c, d)
    {	
	if (axis > dimensions.size()-1)
	{
	    throw std::invalid_argument("axis is out of bounds!");
	}
	if (layer == dims.size()-1)
	{
	    double summed = 0;
	    for (int i=0; i < dims[layer]; i++)
	    {
		std::vector<int> coordinates = prefix;
		coordinates.push_back(i);
		swap(coordinates, axis, dims.size()-1);
		summed += data[get_item(coordinates)];
		
	    }
	    out.push_back(summed);
	}
	else
	{
	    for (int a=0; a < dims[layer]; a++)
	    {
		prefix.push_back(a);
		squish_layer(axis, layer+1, prefix, out, dims);
		prefix.pop_back();
	    }
	}
    }

    Matrix squish(int axis)
    {
	std::vector<int> prefix;
	std::vector<double> out;
	std::vector<int> dims = dimensions;
	swap(dims, axis, dims.size()-1);
	squish_layer(axis, 0, prefix, out, dims);
	std::vector<int> out_dims = dimensions;
	out_dims.erase(out_dims.begin()+axis);
	return Matrix(out, out_dims);
    }

    void bdot(Matrix& other, int layer, std::vector<int> prefix, std::vector<double>& out)
    {
	if (dimensions.size() - layer == 2)
	{
	    for (int i=0; i < dimensions[layer]; i++)
	    {
		for (int j=0; j < other.shape()[layer+1]; j++)
		{
		    double x = 0;
		    for (int k=0; k < dimensions[layer+1]; k++)
		    {
			std::vector<int> a = prefix;
			a.push_back(i);
			a.push_back(k);
			std::vector<int> b = prefix;
			b.push_back(k);
			b.push_back(j);
			x += data[get_item(a)] * other.data[other.get_item(b)];
		    }
		    out.push_back(x);
		}
	    }
	}
	else
	{
	    for (int l=0; l < dimensions[layer]; l++)
	    {
		prefix.push_back(l);
		bdot(other, layer+1, prefix, out);
		prefix.pop_back();
	    }
	}
    }

    Matrix b_dot(Matrix& other)
    {
	std::vector<int> prefix;
	std::vector<double> out;
	bdot(other, 0, prefix, out);
	std::vector<int> out_dims = dimensions;
	// (a, b, c, d) x (a, b, d, e) ==> (a, b, c, e)
	out_dims[dimensions.size()-1] = other.shape().back();
	return Matrix(out, out_dims);
    }

    template <typename Function>
    void travrse(int layer, std::vector<int> prefix, Function logic)
    {
	if (layer == dimensions.size()-1)
	{
	    for (int i=0; i < dimensions[layer]; i++)
	    {
		std::vector<int> coordinates = prefix;
		coordinates.push_back(i);
		logic(coordinates, data[get_item(coordinates)]);
	    }
	}
	else
	{
	    for (int j=0; j < dimensions[layer]; j++)
	    {
		prefix.push_back(j);
		travrse(layer+1, prefix, logic);
		prefix.pop_back();
	    }
	}
    }

    template <typename Function>
    void traverse(Function logic)
    {
	std::vector<int> prefix;
	travrse(0, prefix, logic);
    }

    Matrix operator+(Matrix& other)
    {
	
	std::vector<double> out;
	traverse([&other, &out](std::vector<int>& coords, double value)
	{
	    out.push_back(value + other.data[other.get_item(coords)]);
	}
	);
	std::vector<int> dims = dimensions;
	return Matrix(out, dims);
    }

    Matrix operator-(Matrix& other)
    {
	std::vector<double> out;
	traverse([&other, &out](std::vector<int>& coords, double value)
	{
	    out.push_back(value - other.data[other.get_item(coords)]);
	}
	);
	std::vector<int> dims = dimensions;
	return Matrix(out, dims);
    }

    Matrix operator*(Matrix& other)
    {
	std::vector<double> out;
	traverse([&other, &out](std::vector<int>& coords, double value)
	{
	    out.push_back(value * other.data[other.get_item(coords)]);
	}
	);
	std::vector<int> dims = dimensions;
	return Matrix(out, dims);
    }

    Matrix operator/(Matrix& other)
    {
	std::vector<double> out;
	traverse([&other, &out](std::vector<int>& coords, double value)
	{
	    if (other.data[other.get_item(coords)] == 0)
	    {
		throw std::invalid_argument("#DIV/0!");
	    }
	    out.push_back(value / other.data[other.get_item(coords)]);
	}
	);
	std::vector<int> dims = dimensions;
	return Matrix(out, dims);
    }
    
    static Matrix array(std::vector<int> dims, double value)
    {
	std::vector<double> out;
	
	for (int i=0; i < elemult(dims); i++)
	{
	    out.push_back(value);
	}
	return Matrix(out, dims);
    }
    
    Matrix transpose()
    {
	std::vector<int> new_dim = dimensions;
	swap(new_dim, 0, new_dim.size()-1);
	Matrix out = Matrix::array(new_dim, 0.0);
	out.traverse(
	[&out, this](std::vector<int>& coords, double value)
	{
	    std::vector<int> new_coords = coords;
	    swap(coords, 0, coords.size()-1);
	    out.data[out.get_item(new_coords)] = data[get_item(coords)];
	}
	);
	return out;
    }

    Matrix transpose(int axis1, int axis2)
    {
	if (axis1 > dimensions.size()-1 || axis2 > dimensions.size())
	{
	    throw std::invalid_argument("transpose(int axis1, int axis2) --> Axis out of bounds!");
	}
	std::vector<double> out;
	traverse(
	[&out, this, axis1, axis2](std::vector<int>& coords, double value)
	{
	    swap(coords, axis1, axis2);
	    out.push_back(data[get_item(coords)]);
	}
	);
	std::vector<int> new_dim = dimensions;
	swap(new_dim, axis1, axis2);
	return Matrix(out, new_dim);
    }
};

// batched prod
// tensor contraction

int main()
{
    /*
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
    Matrix X({2,6,5,8});
    std::cout << "Array 1 with dimension: ";
    print(X.shape());
    std::cout << '\n';
    X.print();
    std::cout << '\n';
    std::cout << '\n';
    Matrix Y({2,6,8,9});
    std::cout << "Array 2 with dimension: ";
    print(Y.shape());
    std::cout << '\n';
    Y.print();
    std::cout << '\n';
    std::cout << '\n';
    Matrix out = X.b_dot(Y);
    std::cout << "Resulting array with dimension: ";
    print(out.shape());
    std::cout << '\n';
    out.print();
    Matrix A({4,2,3});
    std::cout << "Unsquished matrix shape: ";
    std::cout << '\n';
    A.print();
    std::cout << '\n';
    Matrix B = A.squish(0);
    std::cout << "New shape: ";
    std::cout << '\n';
    B.print();
    std::vector<int> arr = {2,4};
    Matrix out = Matrix::array(arr, 1.2);
    out.print();
    */
    Matrix C({3,4});
    C.print();
    std::cout << '\n';
    Matrix X = C.transpose();
    std::cout << '\n';
    X.print();
    return 0;
}
