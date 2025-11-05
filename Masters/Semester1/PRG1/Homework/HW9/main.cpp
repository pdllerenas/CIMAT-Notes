#include <bitset>
#include <cmath>
#include <iostream>

typedef struct Flyte {
  unsigned int sign : 1;
  unsigned int exp : 4;
  unsigned int mantissa : 3;
} Flyte;

Flyte stoflyte(std::string s) {
  size_t index = 0;
  Flyte result{0, 0, 0};

  unsigned int int_part = 0u;

  bool negative = false;

  // handle sign
  if (index < s.size() &&
      (s[index] == '-' || s[index] == '-' || s[index] == '+')) {
    negative = (s[index] == '-');
    result.sign = negative;
    ++index;
  }

  while (index < s.size() && std::isdigit(s[index])) {
    int_part = int_part * 10 + (s[index] - '0');
    ++index;
  }

  std::bitset<8> b{int_part};
  std::cout << b.to_string() << '\n';

  double frac_part = 0.0;
  if (s[index] == '.') {
    ++index;
		unsigned int decimal_place = 10u;
    while (index < s.size() && std::isdigit(s[index])) {
      frac_part = frac_part + static_cast<double>(s[index] - '0') / decimal_place; 
			decimal_place *= 10;
			++index;
    }
  }
	std::cout << frac_part << std::endl;
	// convert fractional part to bits
	// using max possible length (3 bits aka 2^3 = 8)
	if (frac_part != 0.0) {
		frac_part *= 8.0;
	}
	unsigned int mantissa = 0u;
	mantissa += std::round(frac_part);
	std::bitset<32> c{int_part};
	std::cout << c.to_string() << std::endl;
	std::bitset<3> d{int_part};
	std::cout << d.to_string() << std::endl;
	std::cout << c.count() << std::endl;
	if (c.count() >= 3) {
		
		int_part >>= 1;
	}
	
	std::cout << mantissa << std::endl;
  return result;
}

int main() {
  std::string a, b;
  std::cin >> a >> b;
  stoflyte(a);
  stoflyte(b);
}
