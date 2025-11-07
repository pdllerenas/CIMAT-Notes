#include <bitset>
#include <cmath>
#include <iostream>

typedef struct Flyte {
  unsigned int sign : 1;
  unsigned int exp : 4;
  unsigned int mantissa : 3;
} Flyte;

/*
 * Given a string s, return a Flyte struct
 *  (see above) with the corresponding encoding values
 */
Flyte stoflyte(std::string s) {
  if (s == "inf") {
    return {0, 0b1111, 0};
  }
  if (s == "nan") {
    return {0, 0b1111, 1};
  }
  size_t index = 0;
  Flyte result{0, 0, 0};

  // handle sign
  bool negative = false;
  if (index < s.size() && (s[index] == '-' || s[index] == '+')) {
    negative = (s[index] == '-');
    result.sign = negative;
    ++index;
  }

  // get integer part
  unsigned int int_part = 0;
  while (index < s.size() && std::isdigit(s[index])) {
    int_part = int_part * 10 + (s[index] - '0');
    ++index;
  }

  // get fractional part
  double frac_part = 0.0;
  if (index < s.size() && s[index] == '.') {
    ++index;
    // keep track of what we should divide by
    double decimal_place = 10.0;
    while (index < s.size() && std::isdigit(s[index])) {
      frac_part =
          frac_part + static_cast<double>(s[index] - '0') / decimal_place;
      decimal_place *= 10.0;
      ++index;
    }
  }
  // since minexp = -6, we only need to check up to 6 + 3 = 9 bits
  // representing the fraction part (to cover for the implicit bit and 3
  // aditional for the mantissa)
  // convert fractional part to bits
  // using max possible length (3 bits aka 2^3 = 8)
  std::bitset<9> frac_bin;
  double int_mod;
  unsigned short shift = 0; // how many times we must shift the decimal point
  unsigned short frac_bit_cnt = 0;
  // only doing 9 operations, as explained above
  for (shift = 0; shift < 9; shift++) {
    if (frac_part == 0) {
      break;
    }
    frac_part *= 2;
    // get integer part of product, set frac part to only the frac part
    frac_part = std::modf(frac_part, &int_mod);

    // get significant bits, this will be used for the exponent (equivalently,
    // decimal point shift)
    if (frac_bit_cnt == 0 && int_mod) {
      frac_bit_cnt = 9 - shift;
    }
    frac_bin[8 - shift] = static_cast<bool>(int_mod);
    int_mod = 0.0;

    // if no more frac_part, break early, the rest of the bits will be 0
  }

  // count number of useful bits for integer part
  unsigned int cpy_int = int_part;
  unsigned short int_bit_cnt = 0;
  while (cpy_int) {
    ++int_bit_cnt;
    cpy_int >>= 1;
  }

  // if integer part is not 0, set exponent to
  // number of times we must shift the decimal point
  //  Note: can maybe merge this if with the following
  if (int_bit_cnt > 0) {
    // if overflow, set exp to 1111 aka inf
    if (int_part > 240) {
      result.exp = 0b1111;
      result.mantissa = 0;
      return result;
    }
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
    result.mantissa |= 0b1 & frac_bin[8];
    // if bit after last one taken into account is active,
    // round the value up
    if (frac_bin[7] == 1) {
      result.mantissa += 1;
    }
    // CASE 3: 2 BITS ARE MISSING FROM MANTISSA (TAKING INTEGER PART INTO
    // ACCOUNT)
  } else if (int_bit_cnt == 2) {
    result.mantissa = int_part & 0b1;
    result.mantissa <<= 2;
    result.mantissa |= (frac_bin[8] << 1) | (frac_bin[7]);
    // if bit after last one taken into account is active,
    // round the value up
    if (frac_bin[6] == 1) {
      result.mantissa += 1;
    }
    // CASE 4: ALL BITS ARE MISSING, SO WE TAKE THE 3 MSB FROM FRAC PART
  } else if (int_bit_cnt == 1) {
    result.mantissa = (frac_bin[8] << 2) | (frac_bin[7] << 1) | (frac_bin[6]);
    // if bit after last one taken into account is active,
    // round the value up
    if (frac_bin[5] == 1) {
      result.mantissa += 1;
    }
    // CASE 5: NO BITS IN INTEGER PART, SO WE MUST NORMALIZE FRAC PART
  } else if (int_bit_cnt == 0) {
    if (frac_bit_cnt >= 4) {
      // exp is 7 - where the first 1 was found in the fraction representation
      result.exp = 7 - (9 - frac_bit_cnt + 1);
      result.mantissa = (frac_bin[frac_bit_cnt - 2] << 2) |
                        (frac_bin[frac_bit_cnt - 3] << 1) |
                        (frac_bin[frac_bit_cnt - 4]);
      // if bit after last one taken into account is active,
      // round the value up
      if (frac_bit_cnt > 4 && frac_bin[frac_bit_cnt - 5] == 1) {
        result.mantissa += 1;
      }
    } else {
      result.exp = 0;
      // grab last 3 bits of fraction representation
      result.mantissa = (frac_bin[2] << 2) | (frac_bin[1] << 1) | (frac_bin[0]);
    }
  }
  return result;
}

float flyte_to_float(Flyte fly) {
  // handle exp = 0 case

  if (fly.exp == 0 && fly.mantissa == 0) {
    return 0.0;
  }
  if (fly.exp == 15) {
    if (fly.mantissa > 0) {
      return nan("");
    }
    return std::numeric_limits<float>::infinity();
  }

  float result = (fly.exp) ? 1.0 : 0.0;

  std::bitset<3> mantissa{fly.mantissa};
  result += mantissa[2] * 0.5 + mantissa[1] * 0.25 + mantissa[0] * 0.125;

  // special case of exp == 0, then we multiply by 2^(-6)
  if (fly.exp == 0) {
    for (int i = 0; i < 6; i++) {
      result /= 2.0;
    }
  } else if (fly.exp < 7) {
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
  while (true) {
    std::cout << "Enter an operation:" << std::endl;
    std::cin >> a >> op >> b;
    Flyte af = stoflyte(a);
    Flyte bf = stoflyte(b);

    float da = flyte_to_float(af);
    float db = flyte_to_float(bf);
    float result = 0.0f;
    float res_float;
    switch (op) {
    case '+':
      result = da + db;
      res_float = std::stof(a) + std::stof(b);
      break;
    case '-':
      result = da - db;
      res_float = std::stof(a) - std::stof(b);
      break;
    case '/':
      if (db == 0.0f) {
        std::cerr << "division by 0" << std::endl;
      }
      result = da / db;
      res_float = std::stof(a) / std::stof(b);
      break;
    case '*':
      result = da * db;
      res_float = std::stof(a) * std::stof(b);
      break;
    default:
      std::cerr << "invalid operation" << std::endl;
    }
    std::cout << "flyte operation: ";
    std::cout << da << " " << op << " " << db << " = " << result << std::endl;

    std::cout << "float operation: ";
    std::cout << a << " " << op << " " << b << " = " << res_float << '\n'
              << std::endl;
  }
}
