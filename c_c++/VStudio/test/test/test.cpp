#include<windows.h>

#include <iostream>

typedef int (*i_func_i)(int n);

int main()
{
    HMODULE factorialDll = LoadLibraryA("factorial.dll");
    if (factorialDll == nullptr)
        return 1;
    i_func_i factorial = reinterpret_cast<i_func_i>(GetProcAddress(factorialDll, "_factorial"));
    if (factorial == nullptr)
        return 2;

    int number;
    std::cout << "write a number" << std::endl;
    std::cin >> number;
    std::cout << number << "! = " << factorial(number);
}