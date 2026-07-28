#include <iostream>
#include <vector>
#include <string>

class Matrix
{
private:
    std::vector<std::vector<double>> data;
public:
    Matrix(std::vector<std::vector<double>> values): data(values) {}

    int rows()
    {
	return data.size();
    }

    int columns()
    {
	return data[0].size();
    }
    
    void print() const
    {
	for (std::vector<double> row : data)
	{
	    int elecount = row.size();
	    int ct = 0;
	    for (double ele : row)
	    {
		if (ct == 0)
		{
		    std::cout << "[";
		}
		if (ct < elecount-1)
		{
		    std::cout << ele << ',';
		}
		else
		{
		    std::cout << ele << "]\n";
		}
		ct++;
	    }
	    
	}
    }
    
    
    Matrix operate(const Matrix& other const std::string& operation) const
    {
	if (columns() != other.columns() || rows() != other.rows())
	{
	    throw std::invalid_argument("Matrices' shape do not match!");
	}

	Matrix result = *this

	for (int row=0; row < rows(); row++)
	{
	    for (int column=0; column < columns(); column++)
	    {
		if (operation == "add")
		{
		    result.data[row][column] = data[row][column] + other.data[row][column];
		}

		if (operation == "mult")
		{
		    result.data[row][column] = data[row][column] * other.data[row][column];
		}

		if (operation == "div")
		{
		    if (other.data[row][column] == 0)
		    {
			throw std::invalid_argument("#DIV/0");
		    }
		    result.data[row][column] = data[row][column] / other.data[row][column];
		}

		if (operation == "sub")
		{
		    result.data[row][column] = data[row][column] - other.data[row][column];
		}
		
	    }
	    
	}
	return result;
    }

    Matrix dot(const Matrix& other)
    {
	if (column() != other.row())
	{
	    throw std::invalid_argument('Invalid dimensions!')
	}

	std::vector<std::vector<double>> result(rows(), std::vector<double>(other.columns(), 0);

	// matrix 1

	for (int g=0; g < rows(); g++)
	{
	    for (int h=0; h < columns(); h++)
	    {
		// matrix 1 endpoint
	    }
	}

	// matrix 2

	for (int i=0; i < columns(); i++)
	{
	    for (int i=0; i < rows(); i++)
	    {
		// matrix 2 endpoint
	    }
	}

	// matrix 3

	for (int j=0; j < result.columns(); j++)
	{
	    for (int k=0; k < result.rows(); k++)
	    {
		// matrix 3 endpoint
	    }
	    
	}
	
	// matrix 3 = summation(matrix_2(i) * matrix_1(i))
	
    }
    
};



int main()
{
    Matrix A({
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}});

    A.print();
    return 0;

}
#include <iostream>
#include <vector>

int main()        
{
    std::vector<std::vector<double>> X({{1,2,3}, {4,5,6}});

    for (double a=0; a < X.size(); a++)
    {
	for (double b=0; b < X[0].size(); b++)
	{
	    int value = X[a][b];
	    std::cout << value;
	}
    }
    
    return 0;
}
