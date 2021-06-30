#include <mpreal.h> // mpfr::mpreal https://github.com/advanpix/mpreal
#include <gmpxx.h> // mpz_class https://gmplib.org/manual/C_002b_002b-Interface-General

#include <iostream>
#include <fstream>

// Very generous (a bit too generous) upper bound to number of digits in nth fibonacci number without needing to update precision
int expectedDigits(unsigned int n) {
	if (n == 0 || n == 1) {
		return 1;
	}

	mpfr::mpreal sqrt_5 = sqrt(mpfr::mpreal(5));

	mpfr::mpreal nReal(n);

	mpfr::mpreal d = (n * log10(sqrt_5)) - (log10(5) / 2);

	mpfr::mpreal dCeil = ceil(d);

	return int(dCeil);
}

mpfr::mpreal fibonacci(unsigned int n) {
	if (n == 0) {
		return 0;
	} else if (n == 1) {
		return 1;
	}

	// Only achieves 100% accuracy up until n = 72 without increasing precision
	mpfr::mpreal::set_default_prec(mpfr::digits2bits(expectedDigits(n)));

	mpfr::mpreal sqrt_5 = sqrt(mpfr::mpreal(5));

	mpfr::mpreal nReal(n);

	mpfr::mpreal result = (pow(1 + sqrt_5, nReal) - pow(1 - sqrt_5, nReal)) / (pow(2, nReal) * sqrt_5);

	mpfr::mpreal resultRounded = round(result);
	
	return resultRounded;
}

bool testListMPFR(std::string path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cout << "Error opening file." << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::string nStr = line.substr(0, line.find(' '));
		std::string fibStr = line.substr(line.find(' ') + 1);

		mpfr::mpreal fib = fibonacci(std::stoi(nStr));

		if (fib.toString() != fibStr) {
			return false;
		}
	}

	return true;
}

bool testListMPIR(std::string path) {
	std::ifstream file(path);

	if (!file.is_open()) {
		std::cout << "Error opening file." << std::endl;
		return false;
	}

	std::string line;
	while (std::getline(file, line)) {
		std::string nStr = line.substr(0, line.find(' '));
		std::string fibStr = line.substr(line.find(' ') + 1);

		mpz_class n(nStr);
		mpz_class fib = fibonacci(n);

		if (fib.get_str() != fibStr) {
			return false;
		}
	}

	return true;
}

int main() {
	//mpfr::mpreal result = fibonacci(1240);
	//std::cout << result.toString() << std::endl;

	std::cout << "Every fibonacci number in list correct for mpfr? " << testListMPFR("../b000045.txt") << std::endl;
	std::cout << "Every fibonacci number in list correct for mpir? " << testListMPIR("../b000045.txt") << std::endl;

	mpfr::mpreal one(1);
	mpfr::mpreal three(3);
	mpfr::mpreal third = one / three;
	mpfr::mpreal prod = three * third;
	bool precise = prod.toString() == "1";
	std::cout << "mpfr exact? " << precise << std::endl;

	return 0;
}