#include <iostream>
#include <vector>

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
    
};


\
int main()
{
    Matrix A({
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}});

    A.print();
    return 0;

}
