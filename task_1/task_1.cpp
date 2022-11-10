#include <iostream>
#include<vector>

void move_vectors(std::vector<std::string>& src, std::vector<std::string>& target) {
	target = std::move(src);
}

int main()
{
	std::vector <std::string> one = { "test_string1", "test_string2" };
	std::vector <std::string> two;
	move_vectors(one, two);
	for (const auto& val : two) {
		std::cout << val << ' ';
	}
	std::cout << std::endl;
}
