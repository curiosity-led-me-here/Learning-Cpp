#include <iostream>
#include <vector>
#include <string>

class Matrix
{
private:
    std::vector<std::vector<double>> data;
public:
    Matrix(std::vector<std::vector<double>> values): data(values) {}

    int rows() const
    {
	if (data.size() == 0)
	{
	    throw std::invalid_argument("Empty Matrix!!!");
	}
	return data.size();
    }

    int columns() const
    {
	if (data[0].size() == 0)
	{
	    throw std::invalid_argument("Empty Matrix!!!");
	}
	return data[0].size();
    }
    
    void print() const
    {
	for (const std::vector<double>& row : data)
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

    Matrix operator+(const Matrix& other) const
    {
	if (columns() != other.columns() || rows() != other.rows())
	{
	    throw std::invalid_argument("Matrices' shape do not match!");
	}
	
	Matrix result = *this;

	for (int row=0; row < rows(); row++)
	{
	    for (int column=0; column < columns(); column++)
	    {
		result.data[row][column] = data[row][column] + other.data[row][column];
	    }
	}

	return result;
    }

    Matrix operator*(const Matrix& other) const
    {
	if (columns() != other.columns() || rows() != other.rows())
	{
	    throw std::invalid_argument("Matrices' shape do not match!");
	}
	
	Matrix result = *this;

	for (int row=0; row < rows(); row++)
	{
	    for (int column=0; column < columns(); column++)
	    {
		result.data[row][column] = data[row][column] * other.data[row][column];
	    }
	}

	return result;
    }

    Matrix operator-(const Matrix& other) const
    {
	if (columns() != other.columns() || rows() != other.rows())
	{
	    throw std::invalid_argument("Matrices' shape do not match!");
	}
	
	Matrix result = *this;

	for (int row=0; row < rows(); row++)
	{
	    for (int column=0; column < columns(); column++)
	    {
		result.data[row][column] = data[row][column] - other.data[row][column];
	    }
	}

	return result;
    }

    Matrix operator/(const Matrix& other) const
    {
	if (columns() != other.columns() || rows() != other.rows())
	{
	    throw std::invalid_argument("Matrices' shape do not match!");
	}
	
	Matrix result = *this;

	for (int row=0; row < rows(); row++)
	{
	    for (int column=0; column < columns(); column++)
	    {
		if (other.data[row][column] == 0)
		{
		    throw std::invalid_argument("#DIV/O");
		}
		result.data[row][column] = data[row][column] / other.data[row][column];
	    }
	}

	return result;
    }

    

    Matrix dot(const Matrix& other) const
    {
	if (columns() != other.rows())
	{
	    throw std::invalid_argument("Invalid dimensions!");
	}

	std::vector<std::vector<double>> result(rows(), std::vector<double>(other.columns(), 0));

	for (int i = 0; i < rows(); i++)
	{
	    for (int j = 0; j < other.columns(); j++)
	    {
		for (int k = 0; k < columns(); k++)
		{
		    result[i][j] += data[i][k] * other.data[k][j];
		}
	    }
	    
	}
	
	return result;
    }

    Matrix T() const
    {
	std::vector<std::vector<double>> result(columns(), std::vector<double>(rows(), 0));
	for (int i=0; i < rows(); i++)
	{
	    for (int j=0; j < columns(); j++)
	    {
		result[j][i] = data[i][j];
	    }
	}
	return result;
    }
    
};



int main()
{
    Matrix A({
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}});

    Matrix B({
    {10, 11, 12}});
    
    Matrix C = A + A;
    Matrix D = A.dot(B.T());
    C.print();
    std::cout << '\n';
    D.print();
    return 0;

}
