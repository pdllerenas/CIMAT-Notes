#include <bitset>
#include <cmath>
#include <iostream>

typedef struct Flyte {
  unsigned int sign : 1;
  unsigned int exp : 4;
  unsigned int mantissa : 3;
} Flyte;

void print_flyte(Flyte f) {
  std::cout << "(-1)^" << f.sign << " * 2^(" << f.exp << "-7) * 1."
            << f.mantissa << std::endl;
}

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

  double frac_part = 0.0;
  if (s[index] == '.') {
    ++index;
    unsigned int decimal_place = 10u;
    while (index < s.size() && std::isdigit(s[index])) {
      frac_part =
          frac_part + static_cast<double>(s[index] - '0') / decimal_place;
      decimal_place *= 10;
      ++index;
    }
  }
  // convert fractional part to bits
  // using max possible length (3 bits aka 2^3 = 8)
	
  unsigned int mantissa = 0;
  unsigned int frac_normalized = std::round(frac_part * 8.0);

  unsigned short int_bit_cnt = 0;
  unsigned short frac_bit_cnt = 0;

  unsigned int cpy_int = int_part;
  unsigned int cpy_frac = frac_normalized;
  while (cpy_int) {
    ++int_bit_cnt;
    cpy_int >>= 1;
  }

  while (cpy_frac) {
    ++frac_bit_cnt;
    cpy_frac >>= 1;
  }

  if (int_bit_cnt > 0) {
    result.exp = std::max(int_bit_cnt - 1, 0) + 7;
  }
  // CASE 1: ALL BITS ARE TAKEN BY INTEGER PART
  if (int_bit_cnt >= 4) {
    result.mantissa = (int_part >> (int_bit_cnt - 4));
    result.mantissa &= 0b111;
    // CASE 2: ONLY 1 BIT IS MISSING FOR COMPLETE MANTISSA (TAKING INTEGER PART
    // INTO ACCOUNT)
  } else if (int_bit_cnt == 3) {
    result.mantissa = int_part & 0b11;
    result.mantissa <<= 1;
    result.mantissa |= 0b1 & (frac_normalized >> (frac_bit_cnt - 1));
    // CASE 3: 2 BITS ARE MISSING FROM MANTISSA (TAKING INTEGER PART INTO
    // ACCOUNT)
  } else if (int_bit_cnt == 2) {
    result.mantissa = int_part & 0b1;
    result.mantissa <<= 2;
    result.mantissa |=
        0b11 & (frac_normalized >> std::max(frac_bit_cnt - 2, 0));
    // CASE 4: ALL BITS ARE MISSING, SO WE TAKE THE 3 MSB FROM FRAC PART
  } else if (int_bit_cnt == 1) {
    result.mantissa = (frac_normalized >> (frac_bit_cnt - 3));
    // CASE 5: NO BITS IN INTEGER PART, SO WE MUST NORMALIZE FRAC PART
  } else if (int_bit_cnt == 0) {
    // since min exp = -6, we only need to check up to 6 + 3 = 9 bits
    // representing the fraction part (to cover for the implicit bit and 3
    // aditional for the mantissa)
    std::bitset<9> frac_bits;
    double int_mod;
    frac_bit_cnt = 0;
    for (int i = 0; i < 9; i++) {
      frac_part *= 2;
      // get integer part of product, set frac part to only the frac part
      frac_part = std::modf(frac_part, &int_mod);

      // get significant bits, this will be used for the exponent (equivalently,
      // decimal point shift)
      if (frac_bit_cnt == 0 && int_mod) {
        frac_bit_cnt = 9 - i;
      }
      frac_bits[8 - i] = static_cast<bool>(int_mod);
			int_mod = 0.0;

			// if no more frac_part, break early, the rest of the bits will be 0
			if (frac_part == 0) {
				break;
			}
    }
		std::cout << frac_bits.to_string() << std::endl;
    if (frac_bit_cnt >= 4) {
      // ignore last 3 bits, since these cannot be shifted due to space
      result.exp = 7 - (10 - frac_bit_cnt);
      result.mantissa = (frac_bits[frac_bit_cnt - 2] << 2) |
                        (frac_bits[frac_bit_cnt - 3] << 1) |
                        (frac_bits[frac_bit_cnt - 4]);
    } else {
      result.exp = 0;
      // grab last 3 bits of fraction representation
      result.mantissa =
          (frac_bits[0] << 2) | (frac_bits[1] << 1) | (frac_bits[2]);
    }
  }
  return result;
}

double flyte_to_double(Flyte fly) {
  // handle exp = 0 case

  if (fly.exp == 15) {
    return 0.0;
  }
  double result = (fly.exp) ? 1.0 : 0.0;

  std::bitset<3> mantissa{fly.mantissa};
  result += mantissa[0] * 0.5 + mantissa[1] * 0.25 + mantissa[2] * 0.125;

  if (fly.exp < 7) {
    for (int i = 0; i < 7 - fly.exp; i++) {
      result /= 2.0;
    }
  } else {
    for (int i = 0; i < fly.exp - 7; i++) {
      result *= 2.0;
    }
  }
  if (fly.sign == 1) {
    result *= -1;
  }
  return result;
}

int main() {
  std::string a, b;
	char op;
  std::cin >> a >> op >> b;
  Flyte af = stoflyte(a);
  Flyte bf = stoflyte(b);

  double da = flyte_to_double(af);
  double db = flyte_to_double(bf);
	double result = 0.0;
	switch(op) {
		case '+':
			result = da + db;
			break;
		case '-':
			result = da - db;
			break;
		case '/':
			if (db == 0.0) {
				std::cerr << "division by 0" << std::endl;
			}
			result = da / db;
			break;
		case '*':
			result = da * db;
			break;
		default:
			std::cerr << "invalid operation" << std::endl;
	}
  std::cout << da << " " << op << " " << db << " = " << result << std::endl;
}
