#include<iostream>
#include<string>
#include<sstream>

int main()
{
	int num;
	std::cin >> num;
	std::basic_ostringstream<char16_t> stream;
	stream << num;

	std::basic_string<char16_t> tmp = stream.str();
	std::basic_string<wchar_t> tmp2(std::begin(tmp), std::end(tmp));
	for(size_t i = 0; i < tmp.length(); i++)
	{
		std::cout << "0x" << std::hex << static_cast<uint64_t>(tmp[i]) << ' ';
	}

	std::wcout << std::endl << tmp2 << std::endl;

	return 0;
}
