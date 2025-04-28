#include <fstream>
#include <iostream>

int main() {
    std::ifstream file("PA 3 Test Cases/testprog15");
    if (!file.is_open()) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    char ch;
    while (file >> ch) { // Reads non-whitespace characters
        std::cout << "Read character: " << ch << std::endl;
    }

    file.close();
    return 0;
}
