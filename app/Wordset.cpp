#include "Wordset.hpp"
#include <string>
#include <iostream>
#include <cassert>
#include <sstream>

// returns s, as a number in the given base, mod the given modulus
unsigned polynomialHashFunction(const std::string& s, unsigned base, unsigned mod)
{
    //s = "abz" (1 * 5^2 + 2 * 5 + 26)
    //base is BASE_H1 for first base and BASE_H2 for second base.
    //mod is initial size
    unsigned significant_digit{};
    unsigned base_power = 1;
    unsigned hash_value{};
    for (int i = s.length() - 1; i >= 0; i--)
    {
        significant_digit = s[i] - 'a' + 1;
        hash_value += (significant_digit * base_power) % mod;
        base_power *= base;
    }
    return hash_value % mod;
}

//num would be base_h1 or base_h2
bool WordSet::isPrime(unsigned n)
{

    if (n <= 1)  return false;
    if (n <= 3)  return true;


    if (n % 2 == 0 || n % 3 == 0) return false;

    for (int i = 5; i * i <= n; i = i + 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}
//11 -> 23
unsigned WordSet::generatePrime(unsigned num)
{
    // Base case
    if (num <= 1)
        return 2;

    unsigned prime = num * 2; // because next prime number should twice greater than first one. e.g. if the first one is 11, then next one is 23
    bool found = false;

    // Loop continuously until isPrime returns
    // true for a number greater than n
    while (!found) {
        prime++;

        if (isPrime(prime))
            found = true;
    }

    return prime;
}

WordSet::WordSet(unsigned initialCapacity, unsigned evictionThreshold)
{
    H1_ptr = new std::string[initialCapacity];
    for (int i = 0; i < initialCapacity; i++)
    {
        H1_ptr[i] = "";
    }
    H2_ptr = new std::string[initialCapacity];
    for (int i = 0; i < initialCapacity; i++)
    {
        H2_ptr[i] = "";
    }
    array_size = initialCapacity;
    //evictionThreshold is default by 5
    //not sure
}

WordSet::~WordSet()
{
    //just need to delete wordset_ptr at this moment
    delete[] H1_ptr;
    delete[] H2_ptr;
}


void WordSet::resize()
{
    threshold_count = 0;
    unsigned new_array_size = generatePrime(array_size);
    std::string* tmp = new std::string[new_array_size];
    // tmp initialization
    for (int i = 0; i < new_array_size; i++)
    {
        tmp[i] = "";
    }
    int pos = 0;
    // assigned strings into tmp
    for (int i = 0; i < array_size; i++)
    {
        tmp[pos] = H1_ptr[i];
        pos++;
        tmp[pos] = H2_ptr[i];
        pos++;
       
    }

    delete[] H1_ptr;
    delete[] H2_ptr;

    std::string* new1 = new std::string[new_array_size];
    std::string* new2 = new std::string[new_array_size];
    H1_ptr = new1;
    H2_ptr = new2;

    //at this moment think only tmp, H1_ptr, H2_ptr
    array_size = new_array_size;
    unsigned loop = 0;
    while (loop < array_size)
    {
        insert(tmp[loop]);
        loop++;
    }


    delete[] tmp;
}

void WordSet::insert(const std::string& s)
{
    unsigned bucket;
    bucket = polynomialHashFunction(s, BASE_H1, array_size);
    //base case1
    if (H1_ptr[bucket] == "")
    {
        H1_ptr[bucket] = s;
        threshold_count = 0;
        return;
    }

    std::string prev_string = H1_ptr[bucket];
    H1_ptr[bucket] = s;
    bucket = polynomialHashFunction(prev_string, BASE_H2, array_size);
    threshold_count++; // since kickout 
    std::string prev_holder;


    //resize
    if (threshold_count == DEFAULT_EVICT_THRESHOLD)
    {
        resize();
        threshold_count = 0;
        return insert(prev_string);
    }

    //base case2
    if (H2_ptr[bucket] == "")
    {
        H2_ptr[bucket] = prev_string;
        threshold_count = 0;
        return;
    }

    else
    {
        prev_holder = H2_ptr[bucket];
        H2_ptr[bucket] = prev_string;
        threshold_count++;
        return insert(prev_holder);
    }


}

bool WordSet::contains(const std::string& s) const
{
    // case 1: item is located in Hash1
    int num = polynomialHashFunction(s, BASE_H1, array_size);
    if (H1_ptr[num] == s)
        return true;
    else // case 2: item is located in Hash2
    {
        num = polynomialHashFunction(s, BASE_H2, array_size);
        if (H2_ptr[num] == s)
        {
            return true;
        }
    }
    return false;
}

// return how many distinct strings are in the set
unsigned WordSet::getCount() const
{
    // traverse through hash tables to count how many unique words there are.
    unsigned count = 0;
    for (int i = 0; i < array_size; i++) {
        if (H1_ptr[i] != "")
            count++;
        if (H2_ptr[i] != "")
            count++;
    }
    return count;
}

// return how large the underlying array is.
unsigned WordSet::getCapacity() const
{
    return array_size;
}

void WordSet::print_val() {
    std::cout << "H1 is ";
    for (int i = 0; i < array_size; i++)
    {
        if (H1_ptr[i] == "")
        {
            std::cout << " ";
        }
        else
            std::cout << H1_ptr[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "H2 is ";
    for (int i = 0; i < array_size; i++)
    {
        if (H2_ptr[i] == "")
        {
            std::cout << " ";
        }
        else
            std::cout << H2_ptr[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Both size are " << array_size << std::endl;
}
