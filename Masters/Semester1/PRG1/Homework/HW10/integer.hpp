#include <bitset>
#include <iostream>
#include <string>

static inline bool all_digits(std::string s) {
  for (char c : s) {
    if (!std::isdigit(c)) {
      return false;
    }
  }
  return true;
}

// converts digits 0-15 to their corresponding letter
static inline unsigned char small_dec_to_hex(unsigned char i) {
  if (i <= 9) {
    return i + '0';
  } else {
    return i + 'A' - 10;
  }
}

// convert char (from a hex digit) to the corresponding integer
static inline unsigned short ch_to_b(unsigned char c) {
  if (std::isdigit(c)) {
    return c - '0';
  }
  if ('a' <= c && c <= 'f') {
    return c - 'a' + 10;
  }

  if ('A' <= c && c <= 'F') {
    return c - 'A' + 10;
  }
  return 0;
}

// converts a nibble (4 bits) to the corresponding hex value
static inline unsigned char nibble_to_ch(std::string nib) {
  unsigned char ch = 0;
  for (int i = 0; i < nib.size(); i++) {
    ch += (nib[i] - '0') << (3 - i);
  }
  if (ch <= 9) {
    return ch + '0';
  } else {
    return ch + 'a' - 10;
  }
}

/*
 * Hexadecimal class.
 * The property x is in a normalized notation of '0x' + 8 digits. Constructor
 * from string parses the string and determines if it is in hex notation,
 * binary, or decimal. The first conversion is trivial. The remaining 2 use
 * bitset to determine the values of each nibble.
 *
 * The default constructor sets x to "0x0".
 *
 * The overloaded operations perform sum and difference as one would do by
 * hand, doing carry overs.
 */
class Hexadecimal {
private:
  std::string x;

public:
  Hexadecimal(std::string a) {

    if (a[1] == 'x') {
      x = a;

      // normalize hex string, assuming we already have x = '0x...'.
      // thus, there are 8 + 2 = 10 total chars
      x.insert(x.begin() + 2, 10 - x.length(), '0');
    }

    // binary prefix case
    else if (a[1] == 'b') {
      x = "0x";
      int displacement = 0;
      for (int k = 2; k < a.size(); ++k) {
        x += nibble_to_ch(a.substr(k + 4, 4));
      }
    }

    // decimal case
    else {
      std::bitset<32> b = std::bitset<32>(std::stoi(a));
      for (int k = 0; k < b.size(); k += 4) {
        std::bitset<32> mask(0b1111 << k);
        std::bitset<32> nibble(b & mask);
        std::string s_nibble = nibble.to_string().substr(32 - 4);
        x.insert(x.end(), nibble_to_ch(s_nibble));
      }
    }
  }

  Hexadecimal() { x = "0x00000000"; }

  std::string get_value() { return x; }

  Hexadecimal operator+(Hexadecimal other) {
    std::string result;

    unsigned short carry = 0;
    for (int k = x.length() - 1; k >= 2; --k) {
      unsigned short temp = ch_to_b(other.x[k]) + ch_to_b(x[k]) + carry;
      result.insert(result.begin(), 1, small_dec_to_hex(temp % 16));
      carry = temp / 16;
    }
    result.insert(result.begin(), 1, 'x');
    result.insert(result.begin(), 1, '0');
    return Hexadecimal(result);
  }

  Hexadecimal operator-(Hexadecimal other) {
    std::string result;

    unsigned short borrow = 0;

    for (int k = x.length() - 1; k >= 2; --k) {
      int a = ch_to_b(x[k]);
      int b = ch_to_b(other.x[k]);

      int temp = a - b - borrow;

      if (temp < 0) {
        temp += 16;
        borrow = 1;
      } else {
        borrow = 0;
      }

      result.insert(result.begin(), small_dec_to_hex(temp));
    }

    result.insert(result.begin(), 'x');
    result.insert(result.begin(), '0');

    return Hexadecimal(result);
  }
};

/*
 *
 * The Binary class is essentially a wrapper for bitset.
 * The only novel members are the operator overloads for sum
 * and difference, where the usual binary operations are used
 * to perform them.
 *
 */
class Binary {
private:
  std::bitset<32> bits;

public:
  Binary() { bits = std::bitset<32>(0); }
  Binary(unsigned int i) { bits = std::bitset<32>(i); }
  Binary(int i) { bits = std::bitset<32>(i); }
  Binary(std::bitset<32> i) { bits = i; }

  Binary operator+(Binary other) {
    std::bitset<32> result(0);
    bool carry = 0;
    for (int i = 0; i < 32; i++) {
      result[i] = carry ^ bits[i] ^ other.bits[i];
      carry = (bits[i] & other.bits[i]) | (carry & bits[i]) |
              (carry * other.bits[i]);
    }
    // implicit use of contrusctor Binary(std::bitset)
    return result;
  }

  // these use the overloaded operator defined above
  // using a - b = a + (~b + 1)
  Binary operator-(Binary other) {
    Binary minus = ~other + Binary(1);
    return *this + minus;
  }

  Binary operator~() {
    // this also uses the constructor from std::bitset
    return ~bits;
  }

  Binary operator|=(Binary other) { return bits |= other.bits; }

  Binary operator&(Binary other) { return bits & other.bits; }

  unsigned long to_ulong() { return bits.to_ulong(); }

  std::string to_string() { return bits.to_string(); }

  size_t size() { return bits.size(); }

  std::bitset<32> get_value() { return bits; }
};

/*
 * The UniversalInteger class holds three representations for an integer i. The
 * methods to get each representation are given below. The constructor
 * here parses the string and calls the constructor of each representation
 * with a clean representation.
 *
 * TODO: check redundant sanitizations on string constructor to follow single
 * responsibiliy principle
 *
 */
class UniversalInteger {
private:
  int i;
  Binary b;
  Hexadecimal x;

public:
  // constructor with string, allows binary string initialization, hexadecimal
  // string representation and usual decimal
  UniversalInteger(std::string a) {
    // hexadecimal prefix case
    if (a[1] == 'x') {
      x = a;
      int displacement = 0;
      for (std::string::reverse_iterator rit = a.rbegin(); rit != a.rend() - 2;
           ++rit) {
        b |= ch_to_b(*rit) << displacement;
        displacement += 4;
      }
      i = static_cast<int>(b.to_ulong());
    }

    // binary prefix case
    else if (a[1] == 'b') {
      b = Binary(std::bitset<32>(a.substr(2)));
      i = static_cast<int>(b.to_ulong());
      int k;

      std::string hex;
      // -- converting binary to hex -- //
      // go until we reach msb (the one after the prefix 0b)
      for (k = a.size(); (k -= 4) >= 2;) {
        hex.insert(hex.begin(), 1, nibble_to_ch(a.substr(k, 4)));
      }

      // check for remaining bits
      // k == 1 corresponds to when we clip the 'b',
      // so we just replace it with a '0'
      if (k == 1) {
        hex.insert(hex.begin(), 1, nibble_to_ch('0' + a.substr(k + 1, 3)));
      }

      // when k == 0, we are at position 0, so we replace the prefix "0b" with
      // "00"
      if (k == 0) {
        hex.insert(hex.begin(), 1, nibble_to_ch("00" + a.substr(k + 2, 2)));
      }

      // normalize to simplify operations
      hex.insert(hex.begin(), 8 - hex.length(), '0');

      // insert prefix for hex
      hex.insert(hex.begin(), 1, 'x');
      hex.insert(hex.begin(), 1, '0');
      x = Hexadecimal(hex);
    } else if (!all_digits(a)) { // invalid representation case
      std::cerr << "invalid representation" << std::endl;
      i = 0;
      b = 0;
      x = Hexadecimal("0x00000000");
    }
    // decimal case
    else {
      i = std::stoi(a);
      b = std::bitset<32>(i);
      std::string hex;
      for (int k = 0; k < b.size(); k += 4) {
        Binary mask(0b1111 << k);
        Binary nibble(b & mask);
        std::string s_nibble = nibble.to_string().substr(32 - (k + 4));
        hex.insert(hex.begin(), nibble_to_ch(s_nibble));
      }
      // normalize to simplify operations
      hex.insert(hex.begin(), 8 - hex.length(), '0');

      // insert prefix for hex
      hex.insert(hex.begin(), 1, 'x');
      hex.insert(hex.begin(), 1, '0');
      x = hex;
    }
  }

  // integer initialization
  UniversalInteger(int n) {
    i = n;
    b = std::bitset<32>(n);
    std::string hex;
    for (int k = 0; k < b.size(); k += 4) {
      Binary mask(0b1111 << k);
      Binary nibble(b & mask);
      std::string s_nibble = nibble.to_string().substr(32 - (k + 4));
      hex.insert(hex.begin(), nibble_to_ch(s_nibble));
    }
    // normalize to simplify operations
    hex.insert(hex.begin(), 8 - hex.length(), '0');

    // insert prefix for hex
    hex.insert(hex.begin(), 1, 'x');
    hex.insert(hex.begin(), 1, '0');
    x = hex;
  }

  // default constructor sets everything to 0
  UniversalInteger() {
    i = 0;
    b = std::bitset<32>(0);
    x = Hexadecimal("0x00000000");
  }

  std::string get_hex() { return x.get_value(); }
  std::bitset<32> get_bin() { return b.get_value(); }
  int get_int() { return i; }

  UniversalInteger operator+(UniversalInteger other) {
    UniversalInteger result;
    result.i = i + other.i;
    result.b = b + other.b;
    result.x = x + other.x;
    return result;
  }

  UniversalInteger operator-(UniversalInteger other) {
    UniversalInteger result;
    result.i = i - other.i;
    result.b = b - other.b;
    result.x = x - other.x;
    return result;
  }
};
