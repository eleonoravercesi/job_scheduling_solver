/*
Read the instances from a file that should be in the following format:
m
p_1
p_2
...
p_m
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

std::vector<int> readIntegersFromFile(const std::string& filename) {
    std::vector<int> numbers;
    std::ifstream infile(filename);
    int n;

    if (!infile) {
        std::cerr << "Cannot open file " << filename << std::endl;
        return numbers;
    }

    while (infile >> n) {
        numbers.push_back(n);
    }

    return numbers;
}

// Function that splits a vector
std::pair<int, std::vector<int>> splitVector(const std::vector<int>& input) {
    if (input.empty()) {
        throw std::runtime_error("Input vector is empty");
    }

    int first = input[0]; // first element
    std::vector<int> rest(input.begin() + 1, input.end()); // remaining elements

    return {first, rest};
}

// Function that really reads the file and splits the content
std::pair<int, std::vector<int>> readFile(const std::string& filename){
    std::vector<int> numbers = readIntegersFromFile(filename);
    return splitVector(numbers);
}
