#include <iostream>
#include "../include/PyString.h"    

using namespace std;

int main() {
    PyString py;
    std::string input = "pouya";

    std::cout<<"count:"<< py.count("banana", "an")<<endl;
    cout << "find: " << py.find(input, "uya") << endl;
    cout << "input: " << input << "\n";
    cout << "revers: " << py.revers(input) << "\n";
    cout << "remove vowels: " << py.removeVowels(input) << endl;
    cout << "slice(1, 4, 1): " << py.slice(input, 1, 4, 1) << "\n";
    cout << "slice(-1, -4, -1): " << py.slice(input, -1, -4, -1) << "\n";
    cout << "slice(4, 1, -1): " << py.slice(input, 4, 1, -1) << "\n";
    
    return 0;
}
