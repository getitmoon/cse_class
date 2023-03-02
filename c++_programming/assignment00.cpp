#include <iostream>

int main(){
    int x, y;
    std::cout << "Enter a number: " << std::flush;
    std::cin >> x;
    std::cout << "Enter another one: " << std::flush;
    std::cin >> y;
    std::cout << "Difference between two numbers is " << std::flush;
    if (x > y) {
        std::cout << x-y << std::endl;
    }
    else {
        std::cout << y-x << std::endl;
    }
    return 0;
}
