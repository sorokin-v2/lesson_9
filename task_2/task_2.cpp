#include <iostream>
#include <utility>
#include<vector>
#include<algorithm>		//for std::transform

class big_integer {
public:

	big_integer() = default;
	big_integer(const std::string& src){
		for (const auto& val : src) {
			if (isdigit(val)) {
				digits.push_back(val - '0');
			}
			else
			{
				clear();
				throw std::runtime_error("invalid parametr");
			}
		}
	}

	//Конструктор копирования (при добавлении оператора перемещающего присваивания неявно удаляется компиллятором)
	big_integer(const big_integer& src) : digits(src.digits) {}

	//Перемещающий конструктор
	big_integer(big_integer&& src) noexcept : digits(std::move(src.digits)) {
		src.clear();
	}
	//Оператор присваивания
	big_integer& operator= (const big_integer& src) {
		digits = src.digits;
		return *this;
	}

	//Оператор перемещающего присваивания
	big_integer& operator= (big_integer&& src) noexcept {
		clear();
		std::swap(digits, src.digits);
		return *this;
	}

	//Не константная версия operator+ (не знаю нужна ли здесь константная версия)
	big_integer operator+ (const big_integer& rhs) {
		big_integer tmp;
		auto iterator1 = digits.crbegin();
		auto iterator2 = rhs.digits.crbegin();
		char shift = 0;		//Перенос в следующий разряд
		while (iterator1 != digits.crend() && iterator2 != rhs.digits.crend()) {
			tmp.digits.push_back((*iterator1 + *iterator2) % 10 + shift);
			((*iterator1 + *iterator2) + shift) >= 10 ? shift = 1 : shift = 0;
			++iterator1;
			++iterator2;
		}
		//добавляем цифры из более длинного слагаемого
		std::for_each(iterator1, digits.crend(), [&shift, &tmp](const auto& val) {
			tmp.digits.push_back((val + shift) % 10);
			((val + shift) >= 10) ? shift = 1 : shift = 0; });

		//Не знаю что эффективнее: предыдущая реализация через for_earch или эта:
		while (iterator2 != rhs.digits.crend()) {
			tmp.digits.push_back((*iterator2 + shift) % 10);
			((*iterator2 + shift) >= 10) ? shift = 1 : shift = 0;
			++iterator2;
		}
		//Добавляем shift, если он нужен
		if(shift > 0) {
			tmp.digits.push_back(shift);
		}
		return (reverse_vector(tmp));
	}

	//Не константная версия оператора умножения на число от 0 до 9
	big_integer operator* (const int& rhs) {
		if (rhs < 0 || rhs > 9) {
			throw std::runtime_error("Out if range");
		}
		big_integer tmp;
		int shift = 0;		//Перенос в следующий разряд
		std::for_each(digits.crbegin(), digits.crend(), [&shift, &tmp, &rhs](const char& val) {
			tmp.digits.push_back((val * rhs + shift) % 10);
			shift = static_cast<int>((val * rhs + shift) / 10);
			});
		if (shift > 0) {
			tmp.digits.push_back(shift);
		}
		return (reverse_vector(tmp));
	}
	
	big_integer& reverse_vector(big_integer& src) {
		//Убираем незначащие нули
		while (src.digits.size() > 1 && src.digits.at(src.digits.size() - 1) == 0 ) {
			src.digits.pop_back();
		}
		//Разворачиваем результирующий вектор
		std::reverse(src.digits.begin(), src.digits.end());
		return src;
	}

	void clear() { digits.clear(); }
	
private:
	
	friend std::ostream& operator<< (std::ostream& os, const big_integer& src);
	std::vector<short> digits;
};


std::ostream& operator<< (std::ostream& os, const big_integer& src) {
	for (const auto& val : src.digits) {
		os << val;
	}
	return os;
}

int main()
{
	auto number1 = big_integer("114575");
	auto number2 = big_integer("78524");
	auto result = number1 + number2;
	std::cout << result << std::endl; // 193099
	std::cout << (result * 9 + number1 + number2) << std::endl;
}


