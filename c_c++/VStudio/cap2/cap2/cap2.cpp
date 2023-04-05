#include <exception>
#include <iostream>
#include <limits>
#include <vector>

void TruthTable(std::vector<bool> vec4)
{
    if (vec4.size() < 4)
        throw std::invalid_argument("vector must have at least 4 values");

    std::cout << "function: " << ((vec4[0]) ? ((vec4[1] || vec4[2] || vec4[3]) ? " A'B' +" : " A'B'") : "") << std::flush;
    std::cout << ((vec4[1]) ? ((vec4[2] || vec4[3]) ? " AB' +" : " AB'") : "") << std::flush;
    std::cout << ((vec4[2]) ? ((vec4[3]) ? " A'B +" : " A'B") : "") << std::flush;
    std::cout << ((vec4[3]) ? " AB" : "") << std::endl;

    std::cout << "| A | B | f |" << std::endl;
    std::cout << "| 0 | 0 | " << ((vec4[0]) ? "1 |" : "0 |") << std::endl;
    std::cout << "| 1 | 0 | " << ((vec4[1]) ? "1 |" : "0 |") << std::endl;
    std::cout << "| 0 | 1 | " << ((vec4[2]) ? "1 |" : "0 |") << std::endl;
    std::cout << "| 1 | 1 | " << ((vec4[3]) ? "1 |" : "0 |") << std::endl;
}

void TruthTableFuncNum(uint8_t terms, uint64_t function)
{
    if (terms > 63 || terms < 1)
        throw std::overflow_error("terms must be in range [1, 63]");
    for (uint8_t i = 0; i < terms; ++i)
        std::cout << "| " << static_cast<char>('A' + i) << ' ' << std::flush;
    std::cout << "| f |" << std::endl;
    for (uint8_t i = 1 << terms; i > 0; --i)
    {
        for (uint8_t j = terms; j > 0; --j)
            std::cout << "| " << static_cast<char>('0' + ((((i << 1)-1) >> j) & 0x1)) << ' ' << std::flush;
        std::cout << "| " << static_cast<char>('0' + (((function << 1) >> ((1 << terms) + (i - (1 << terms)))) & 0x1)) << " |" << std::endl;
    }
}

int main()
{
    std::vector<bool> vec4;
    uint16_t val;
    uint64_t functionNumber;
    uint8_t save;
    bool hasError;

    vec4.resize(4);
    save = 0;
    do {
        try {
            hasError = false;

            for (uint8_t i = save; i < 4; ++i)
            {
                std::cout << "set " << ((i % 2 == 0) ? "A'" : "A") << ((i > 1) ? "B: " : "B': ") << std::flush;
                if (!(std::cin >> val) || (val != 0 && val != 1))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    save = i;
                    throw std::invalid_argument("value must be 1 or 0");
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                vec4[i] = static_cast<bool>(val);
            }

            TruthTable(vec4);
        } catch (std::exception& except)
        {
            hasError = true;
            std::cout << except.what() << std::endl;
        }
    } while (hasError);

    do {
        std::cout << "write the number of input and function number (ii fffff...): " << std::flush;

        try {
            hasError = false;
            
            if (!(std::cin >> val >> functionNumber))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("input must be in range [0, 8]\nfunction number must be in range [0, ULONG_MAX]");
            }
            TruthTableFuncNum(val, functionNumber);

        } catch (std::exception& except)
        {
            hasError = true;
            std::cout << except.what() << std::endl;
        }
    } while (hasError);

    return 0;
}