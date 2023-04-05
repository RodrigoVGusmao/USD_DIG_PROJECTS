#include<bitset>
#include<cstdint>
#include<exception>
#include<iostream>
#include<limits>
#include<string>

char toupper(char lowerCase)
{
	if (lowerCase >= 'a' && lowerCase <= 'z')
	{
		return (lowerCase ^ 0x20);
	}
	else
		throw std::invalid_argument("not a lower case letter");
}

char tolower(char upperCase)
{
	if (upperCase >= 'A' && upperCase <= 'Z')
	{
		return (upperCase | 0x20);
	}
	else
		throw std::invalid_argument("not a upper case letter");
}

int32_t CharToInt(std::string number)
{
	bool hasSignal = false;
	uint32_t num = 0;

	if (number[0] == '+' || number[0] == '-')
		hasSignal = true;
	for (uint8_t i = hasSignal; i < number.length(); i++)
	{
		if (10*num < static_cast<uint32_t>(std::numeric_limits<int32_t>::max()))																	//error 10*num may cause overflow
			num *= 10;
		else
			throw std::overflow_error("number out of range");
		if (number[i] - 0x30 > 9 || number[i] - 0x30 < 0)																							//error number[i]-0x30 may cause underflow
			throw std::invalid_argument("the string must contain only digits optionally preceded by a sign");
		num += number[i] - 0x30;
	}

	if (num > static_cast<uint32_t>(std::numeric_limits<int32_t>::max()) && number[0] != '-')
		throw std::overflow_error("number out of range");

	if (num > static_cast<uint32_t>(std::numeric_limits<int32_t>::max())+1 && number[0] == '-')
		num = ~num + 1;

	return static_cast<int32_t>(num);
}

uint16_t ToDate(uint16_t month, uint16_t day, uint16_t year)
{
	if (month > 12 || month < 1)
		throw std::out_of_range("month must range [1, 12]");
	if (day > 31 || day < 1)
		throw std::out_of_range("day must range [1, 31]");
	if (year > 100)
		throw std::out_of_range("year must range [0, 100]");

	return month << 12 | day << 7 | year;
}

uint8_t ExtractMonth(uint16_t date)
{
	return date >> 12;
}

uint8_t ExtractDay(uint16_t date)
{
	return date >> 7 & 0x1F;
}

uint8_t ExtractYear(uint16_t date)
{
	return date & 0x7F;
}

uint8_t CntBits(uint16_t var)
{
	uint8_t bitCount = 0;

	for (uint8_t i = 0; i < 16; ++i)
		if (var >> i & 0x1)
			++bitCount;

	return bitCount;
}

bool TestBit(uint16_t data, int16_t bit)
{
	if (bit > 15 || bit < 0)
		return false;
	return data >> bit & 0x1;
}

template <typename T> 
T ROR(T data)
{
	if (data & 0x1)
		return data >> 1 | ~(~static_cast<T>(0x0) >> 0x1);
	return data >> 1;
}

template <typename T>
T ROL(T data)
{
	if (data & ~(~static_cast<T>(0x0) >> 0x1))
		return data << 1 | 0x1;
	return data << 1;
}

int main()
{
	bool hasError;
	char letter;
	int32_t num;
	std::string strNumber;
	uint16_t month, day, year, date;
	uint8_t count;
	int16_t data, bit;
	bool bitResult;
	uint32_t number;

	do
	{
		std::cout << "write a letter:   " << std::flush;
		std::cin >> letter;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		try
		{
			hasError = false;
			if (letter >= 'a' && letter <= 'z')
				letter = toupper(letter);
			else if (letter >= 'A' && letter <= 'Z')
				letter = tolower(letter);
			else
				throw std::invalid_argument("character is not an english a-z or A-Z letter");

		} catch (std::exception& except)
		{
			std::cout << except.what() << std::endl;
			hasError = true;
		}
	} while (hasError);
	std::cout << "your toggled case letter is:   " << letter << std::endl;

	do
	{
		std::cout << "write a number between [" 
			<< std::numeric_limits<int32_t>::max() << ", " << std::numeric_limits<int32_t>::min() << "]:   " << std::flush;
		std::cin >> strNumber;
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		try
		{
			hasError = false;

			num = CharToInt(strNumber);
		} catch (std::exception& except)
		{
			std::cout << except.what() << std::endl;
			hasError = true;
		}
	} while (hasError);
	std::cout << "your number was:   " << num << std::endl;

	do 
	{
		std::cout << "write date (mm dd yy):   " << std::flush;
		try
		{
			hasError = false;
			if (!(std::cin >> month >> day >> year))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				throw std::invalid_argument("input must be three numbers in format mm dd yy");
			}
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			date = ToDate(month, day, year);
		} catch (std::exception& except)
		{
			std::cout << except.what() << std::endl;
			hasError = true;
		}
	} while (hasError);
	std::cout << "your compressed date is:   " << date << "\nyour uncompressed date is:   " << std::flush;
	std::cout << +ExtractMonth(date) << '/' << +ExtractDay(date) << '/' << +ExtractYear(date) << std::endl;

	do
	{
		std::cout << "write a number between [" 
			<< std::numeric_limits<uint16_t>::max() << ", " << std::numeric_limits<int16_t>::min() << "]:   " << std::flush;
		try
		{
			hasError = false;
			if (!(std::cin >> num))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				throw std::invalid_argument("input must be a number in range");
			}
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			if (num > std::numeric_limits<uint16_t>::max() || num < std::numeric_limits<int16_t>::min())
				throw std::overflow_error("input must be in range");

			count = CntBits(static_cast<uint16_t>(num));
		} catch (std::exception& except)
		{
			std::cout << except.what() << std::endl;
			hasError = true;
		}
	} while (hasError);
	std::cout << num << " (" << std::bitset<16>(num) << ')' << " has " << +count << " set bits" << std::endl;

	do
	{
		std::cout << "write a number and a byte to be read (nnnnn bb):" << std::flush;
		try
		{
			hasError = false;
			if (!(std::cin >> data >> bit))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				throw std::invalid_argument("input must be a number in range");
			}
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			bitResult = TestBit(data, bit);

		} catch (std::exception& except)
		{
			std::cout << except.what() << std::endl;
			hasError = true;
		}
	} while (hasError);
	std::cout << "The " << bit << " bit of number " << data << " (" << std::bitset<16>(data) << ')' << " is" << ((bitResult == true) ? " set" : " not set") << std::endl;

	do
	{
		std::cout << "write a number in range [" << std::numeric_limits<uint32_t>::max() << ", " << std::numeric_limits<uint32_t>::min() << "]:   " << std::flush;
		try
		{
			hasError = false;
			if (!(std::cin >> number))
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				throw std::range_error("number out of range");
			}
		} catch (std::exception& except)
		{
			std::cout << except.what() << std::endl;
			hasError = true;
		}
	} while (hasError);
	std::cout << number << " (" << std::bitset<32>(number) << ")\nRotate right:" << ROR(number) << " (" << std::bitset<32>(ROR(number)) << ")\nRotate left:" 
		<< ROL(number) << " ("<< std::bitset<32>(ROL(number)) << ")." << std::endl;

	return 0;
}