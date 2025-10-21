#include <iostream>
#include <regex>
#include <string>

char q0(char c) {
  if (c == '+' || c == '-' || (c <= '9' && c >= '0')) {
    return c;
  }
  return '\0';
}

char q1(char c) {
  if (c <= '9' && c >= '0') {
    return c;
  }
  return '\0';
}

char q2(char c) {
  if ((c <= '9' && c >= '0') || c == '.') {
    return c;
  }
  return '\0';
}

char q3(char c) {}

std::string fsm(std::string &s) {
  // copy s to corrected
  size_t length = s.size();
  if (length == 0) {
    return NULL;
  }

  std::string corrected = s;
  size_t mantissa_size = 0;

  // traverse string s char by char
  size_t pos = 0;
  char c = s[pos];

  // q0
  if (c <= '9' || c >= '0') {
    // only 1 digit case
    if (pos == length - 1) {
      corrected = s + "e0";
      return corrected;
    }
    // go to next character
    c = s[++pos];
    // q1: loops until no more digits
    while (c <= '9' || c >= '0') {
      mantissa_size++;
      if (pos == length - 1) {
        // since first char is a digit, we add a point after position 1
        corrected = std::string(1, s[0]) + "." + s.substr(1, length) + "e" +
                    std::to_string(mantissa_size);
        return corrected;
      }
      c = s[++pos];
    }
    // q2: checks for point or E/e
    if (c == '.') {
      // if string ends in '.'
      if (pos == length - 1) {
        // remove last decimal, put it in position 1
        corrected = std::string(1, s[0]) + "." + s.substr(1, length - 1) + "e" +
                    std::to_string(mantissa_size);
        return corrected;
      }
      while (c <= '9' || c >= '0') {
        if (pos == length - 1) {
          // place decimal in correct place
          corrected = std::string(1, s[0]) + "." + s.substr(1, length) + "e" +
                      std::to_string(mantissa_size);
          return corrected;
        }
        c = s[++pos];
      }
    } else if (c == 'e' || c == 'E') { // q5
      // if string ends in e, simply normalize notation, as if nothing came
      // after the character e
      if (pos == length - 1) {
        corrected = std::string(1, s[0]) + "." + s.substr(1, length) +
                    std::to_string(mantissa_size);
        return corrected;
      }
      c = s[++pos];
      if (c <= '9' || c >= '0') {
        // 1 digit after e case
        if (pos == length - 1) {
          corrected = std::string(1, s[0]) + "." + s.substr(1, length) +
                      std::to_string((c - '0') + mantissa_size);
          return corrected;
        }
        // keep track of where exponent starts
        int exp_idx_start = pos;
        c = s[++pos];
        // at least 2 digits after e
        while (c <= '9' || c >= '0') {
          if (pos == length - 1) {
            corrected =
                std::string(1, s[0]) + "." + s.substr(1, exp_idx_start - 1) +
                std::to_string(std::stoi(s.substr(exp_idx_start, length)) +
                               mantissa_size);
            return corrected;
          }
          c = s[++pos];
        }
        // if string ends on something that is not a digit, ignore that char
        corrected =
            std::string(1, s[0]) + "." + s.substr(1, exp_idx_start - 1) +
            std::to_string(std::stoi(s.substr(exp_idx_start, length - 1)) +
                           mantissa_size);
        return corrected;
      }
      // q5 -> q6
      if (c == '-' || c == '+') {
        // if string ends in sign, e.g. 12e- ==> 1.2e1 or 123.321e+
        // ==> 1.23321e2
        if (pos == length - 1) {
          corrected = std::string(1, s[0]) + "." + s.substr(1, length - 1) +
                      std::to_string(mantissa_size);
          return corrected;
        }
        c = s[++pos];
        int exp_idx_start = pos;
        while (c <= '9' || c >= '0') { // q6 -> q7
          if (pos == length - 1) {
            corrected =
                std::string(1, s[0]) + "." + s.substr(1, exp_idx_start - 1) +
                std::to_string(std::stoi(s.substr(exp_idx_start, length)) +
                               mantissa_size);
            return corrected;
          }
          c = s[++pos];
        }
        // if it does not end in a digit, ignore char
        corrected =
            std::string(1, s[0]) + "." + s.substr(1, exp_idx_start - 1) +
            std::to_string(std::stoi(s.substr(exp_idx_start, length - 1)) +
                           mantissa_size);
        return corrected;
      } else if (c <= '9' && c >= '0') { // q5 -> q7
        int exp_idx_start = pos;
        while (c <= '9' || c >= '0') {
          if (pos == length - 1) {
            corrected =
                std::string(1, s[0]) + "." + s.substr(1, exp_idx_start - 1) +
                std::to_string(std::stoi(s.substr(exp_idx_start, length)) +
                               mantissa_size);
            return corrected;
          }
          c = s[++pos];
        }
        // if it does not end in a digit, ignore char
        corrected =
            std::string(1, s[0]) + "." + s.substr(1, exp_idx_start - 1) +
            std::to_string(std::stoi(s.substr(exp_idx_start, length - 1)) +
                           mantissa_size);
        return corrected;
      }
    }
  } else if (c == '-' || c == '+') { // q0 -> q1
		// TODO: do everything done above, assuming first char is sign
  } else { // not -, + or digit, ignore everything else, string is no processed
    return NULL;
  }
  return corrected;
}

int main(int argc, char *argv[]) {
  const std::regex pattern(R"([+-]?\d+(\.\d+)?([eE][+-]?\d+)?)");
  std::string s;
  while (true) {
    std::cout << "Enter a number in scientific notation:" << std::endl;
    std::getline(std::cin, s);
    std::cout << ((std::regex_match(s, pattern) == 0) ? "Invalid.\n"
                                                      : "Valid.\n")
              << std::endl;
  }
}
