#include <iostream>
#include <vector>
#include <random>
#include <cmath>

// Customizable log base
double log(double number, double base)
{
    return std::log(number) / std::log(base);
}

// Number --> Binary
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

// Binary --> Number
int integer(std::vector<int> X)
{
    int num = 0;
    for (int i=0; i<X.size();i++)
    {
	num += std::pow(2, i) * X[X.size()-i-1];
    }
    return num;
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
    std::vector<int> parsed = binary(0, bins.size()-1);
    for (int x=0; x < bins.size(); x++)
    {
	if (bins[x] == 1)
	{
	    std::vector<int> a = binary(x, bins.size()-1);
	    parsed = parsed + a;
	}
    }
    for (int y=0; y < parsed.size(); y++)
    {
	parsed[y] = parsed[y] % 2;
    }
    return parsed;
}

// Swap
int swap(int number)
{
    return 1 - number;
}

// Fill
std::vector<int> fill(std::vector<int>& X)
{
    std::vector<int> out = X;
    int idxs = std::ceil(log(X.size(),2));
    out.insert(out.begin(), 0);
    for (int i=0; i<idxs; i++)
    {
	out.insert(out.begin()+std::pow(2, i), 0);
    }
    return out;
}

// Extract
std::vector<int> extract(std::vector<int>& X)
{
    std::vector<int> out = X;
    int idxs = std::ceil(log(X.size(),2));
    out.erase(out.begin());
    for (int i=0; i<idxs; i++)
    {
	out.erase(out.begin()+std::pow(2, i)-i-1);
    }
    return out;
}


// Calibrate
std::vector<int> calibrate(std::vector<int>& X)
{
    std::vector<int> out = bxor(X);
    for (int bit_idx=0; bit_idx < out.size(); bit_idx++)
    {
	if (out[bit_idx] == 1)
	{
	    int idx = std::pow(2, out.size() - bit_idx - 1);
	    X[idx] = swap(X[idx]);
	}
	
    }
    return X;
}

// send
std::vector<int> send(std::vector<int> bits)
{
    // in --> fill --> calibrate
    std::vector<int> out = fill(bits);
    out = calibrate(out);
    return out;
}


// Correct
std::vector<int> recover(std::vector<int> X)
{
    // BXOR --> INT --> X[INT] --> SWITCH
    std::vector<int> out = bxor(X);
    int idx = integer(out);
    X[idx] = swap(X[idx]);
    return X;
}

// receipt
std::vector<int> receive(std::vector<int> X)
{
    // in --> check --> extract
    std::vector<int> out = recover(X);
    out = extract(out);
    return out;
}

void print(std::vector<int> X)
{
    for (int i : X)
    {
	std::cout << i;
    }
}

// RNG

int randint(int range)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());

    std::uniform_int_distribution<int> distribution(1, range - 1);
    return distribution(generator);
}

int randbit()
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_int_distribution<int> distribution(0, 1);

    return distribution(generator);
}

// Tamper
std::vector<int> tamper(std::vector<int> data, int intensity)
{
    int range = data.size();
    for (int i=0; i < intensity; i++)
    {
	int rand = 0;
	while (std::floor(log(rand,2)) == log(rand,2))
	{
	    rand = randint(range);
	}
	data[rand] = swap(data[rand]);
    }
    return data;
}

// Build message
std::vector<int> message(int n_bits)
{
    std::vector<int> out;
    int allowed = std::pow(2, n_bits) - (n_bits + 1);
    for (int i=0; i < allowed; i++)
    {
	out.push_back(randbit());
    }
    return out;
}


// Main
int main()
{
    // for n bit, message length should be (2^n)-(n+1)
    // Example: I want to send 4-bit msg: characters allowed: (2^4 - 5) = 11 bits
    std::vector<int> msg = message(10);
    std::vector<int> output = send(msg);
    int intensity_factor = 1;
    std::vector<int> tampered_message = tamper(output, intensity_factor);
    std::vector<int> tampered_msg = extract(tampered_message);
    std::cout << "Original message: ";
    print(msg);
    std::cout << "\n";
    std::cout << "Tampered  message: ";
    print(tampered_msg);
    std::cout << "\n";
    // recover
    std::vector<int>recovered_message = receive(tampered_message);
    std::cout << "Recovered message: ";
    print(recovered_message);
    std::cout << "\n";

    std::cout << "Message length: " << msg.size() << " chars" << '\n';
    double compression = static_cast<double>(msg.size()) / output.size();
    std::cout << "Code Rate: " << compression * 100 << " %" << '\n';
    std::cout << "Redundancy: " << (1-compression) * 100 << " %" << '\n';
    
    if (tampered_msg != msg)
    {
	std::cout << "Message Tampered." << '\n';
    }
    else
    {
	std::cout << "Message Intact." << '\n';
    }
    
    if (recovered_message == msg)
    {
	std::cout << "Recovery Status: Done." << '\n';
    }
    else
    {
	std::cout << "Still tampered." << '\n';
    }
    
    return 0;
}
