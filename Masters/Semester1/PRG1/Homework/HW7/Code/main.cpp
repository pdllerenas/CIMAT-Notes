#include <iostream>
#include <regex>
#include <string>

std::string fsm(std::string &s) {
  if (s.empty()) {
    return "Empty string.\n";
  }
  // copy s to corrected
  size_t length = s.size();
  if (length == 0) {
    return "Empty string\n";
  }

  std::string corrected = s;
  size_t mantissa_size = 0;

  // traverse string s char by char
  size_t pos = 0;
  char c = s[pos];

  // describes where the digit starts, so we can parse the sign and
  // use substrings correctly
  size_t digit_idx_start = 0;
  size_t decimal_idx = 0;
  size_t exp_idx_start = 0;

  // q0
  if (c <= '9' && c >= '0') {
    digit_idx_start = 0;
  } else if (c == '+' || c == '-') { // q1
    digit_idx_start = 1;
    mantissa_size = -1;
  } else { // not digit or pm, invalidate string
    return "Invalid format.\n";
  }
  // only 1 digit case
  if (digit_idx_start == 0 && pos == length - 1) {
    corrected = s + "e0";
    return corrected;
  } else if (digit_idx_start == 1 &&
             pos == length - 1) { // only sign, no other char
    return "Invalid format.\n";
  }

  // go to next character
  c = s[++pos];

  // q2: loops until no more digits
  while (c <= '9' && c >= '0') {
    mantissa_size++;
    if (pos == length - 1) {
      // insert a decimal, depending on the existence of a sign
      if (mantissa_size == 0) {
        corrected = s.substr(0, digit_idx_start + 1) + ".0e" +
                    std::to_string(mantissa_size);
      } else {
        corrected = s.substr(0, digit_idx_start + 1) + "." +
                    s.substr(digit_idx_start + 1) + "e" +
                    std::to_string(mantissa_size);
      }
      return corrected;
    }
    c = s[++pos];
  }
  // q2: checks for point or E/e
  if (c == '.') {
    decimal_idx = pos;
    // if string ends in '.'
    if (pos == length - 1) {
      // remove last decimal, put it in position after first digit
      if (mantissa_size == 0) {
        corrected = s.substr(0, digit_idx_start + 1) + ".0e" +
                    std::to_string(mantissa_size);
      } else {
        corrected = s.substr(0, digit_idx_start + 1) + "." +
                    s.substr(digit_idx_start + 1, mantissa_size) + "e" +
                    std::to_string(mantissa_size);
      }
      return corrected;
    }
    c = s[++pos];
    while (c <= '9' && c >= '0') {
      if (pos == length - 1) {
        // place decimal in correct place
        corrected = s.substr(0, digit_idx_start + 1) + "." +
                    s.substr(digit_idx_start + 1, mantissa_size) +
                    s.substr(decimal_idx + 1) + "e" +
                    std::to_string(mantissa_size);
        return corrected;
      }
      c = s[++pos];
    }
    if (c == 'e' || c == 'E') {
      if (pos == length - 1) {
        // if it ends in e, normalize it assuming nothing was after e
        corrected = s.substr(0, digit_idx_start + 1) + "." +
                    s.substr(digit_idx_start + 1, mantissa_size) + "e" +
                    std::to_string(mantissa_size);
        return corrected;
      }
      c = s[++pos];
      bool exp_sign = false;
      if (c == '+' || c == '-') {
        c = s[++pos];
        exp_sign = true;
      }
      exp_idx_start = pos;
      while (c <= '9' && c >= '0') {
        if (pos == length - 1) {
          // place decimal in correct place
          corrected =
              s.substr(0, digit_idx_start + 1) + "." +
              s.substr(digit_idx_start + 1, mantissa_size) +
              s.substr(decimal_idx + 1,
                       exp_idx_start - decimal_idx - 2 - exp_sign) +
              "e" +
              std::to_string(std::stoi(s.substr(exp_idx_start - exp_sign)) +
                             (int)mantissa_size);
          return corrected;
        }
        c = s[++pos];
      }
      // if it does not end on a digit, ignore last char
      corrected = s.substr(0, digit_idx_start + 1) + "." +
                  s.substr(digit_idx_start + 1, mantissa_size) +
                  s.substr(decimal_idx + 1) + "e" +
                  std::to_string(mantissa_size);
      return corrected;
    }
  } else if (c == 'e' || c == 'E') { // q5
    // if string ends in e, simply normalize notation, as if nothing came
    // after the character e
    if (pos == length - 1) {
      corrected = s.substr(0, digit_idx_start + 1) + "." + s.substr(1, length) +
                  std::to_string(mantissa_size);
      return corrected;
    }
    c = s[++pos];
    if (c <= '9' && c >= '0') {
      // 1 digit after e case
      if (pos == length - 1) {
        corrected = s.substr(0, digit_idx_start + 1) + "." +
                    s.substr(1, length) +
                    std::to_string((c - '0') + mantissa_size);
        return corrected;
      }
      // keep track of where exponent starts
      exp_idx_start = pos;
      c = s[++pos];
      // at least 2 digits after e
      while (c <= '9' && c >= '0') {
        if (pos == length - 1) {
          corrected =
              s.substr(0, digit_idx_start + 1) + "." +
              s.substr(1, exp_idx_start - 1) +
              std::to_string(std::stoi(s.substr(exp_idx_start, length)) +
                             mantissa_size);
          return corrected;
        }
        c = s[++pos];
      }
      // if string ends on something that is not a digit, ignore that char
      corrected =
          s.substr(0, digit_idx_start + 1) + "." +
          s.substr(1, exp_idx_start - 1) +
          std::to_string(std::stoi(s.substr(exp_idx_start, length - 1)) +
                         mantissa_size);
      return corrected;
    } else if (c == '-' || c == '+') {
      // if string ends in sign, e.g. 12e- ==> 1.2e1 or 123.321e+
      // ==> 1.23321e2
      if (pos == length - 1) {
        corrected = s.substr(0, digit_idx_start + 1) + "." +
                    s.substr(1, length - 1) + std::to_string(mantissa_size);
        return corrected;
      }
      c = s[++pos];
      exp_idx_start = pos;
      while (c <= '9' && c >= '0') { // q6 -> q7
        if (pos == length - 1) {
          corrected = s.substr(0, digit_idx_start + 1) + "." +
                      s.substr(1, mantissa_size) +
                      std::to_string(std::stoi(s.substr(exp_idx_start)) +
                                     mantissa_size);
          return corrected;
        }
        c = s[++pos];
      }
      // if it does not end in a digit, ignore char
      corrected =
          s.substr(0, digit_idx_start + 1) + "." +
          s.substr(1, exp_idx_start - 1) +
          std::to_string(std::stoi(s.substr(exp_idx_start, length - 1)) +
                         mantissa_size);
      return corrected;
    } else if (c <= '9' && c >= '0') { // q5 -> q7
      exp_idx_start = pos;
      while (c <= '9' && c >= '0') {
        if (pos == length - 1) {
          corrected =
              s.substr(0, digit_idx_start + 1) + "." +
              s.substr(1, exp_idx_start - 1) +
              std::to_string(std::stoi(s.substr(exp_idx_start, length)) +
                             mantissa_size);
          return corrected;
        }
        c = s[++pos];
      }
      // if it does not end in a digit, ignore char
      corrected =
          s.substr(0, digit_idx_start + 1) + "." +
          s.substr(1, exp_idx_start - 1) +
          std::to_string(std::stoi(s.substr(exp_idx_start)) + mantissa_size);
      return corrected;
    }
  }
  corrected =
      s.substr(0, digit_idx_start + 1) + "." +
      s.substr(digit_idx_start + 1, mantissa_size) + s.substr(decimal_idx + 1) +
      std::to_string(std::stoi(s.substr(exp_idx_start)) + mantissa_size);
  return corrected;
}

int main(int argc, char *argv[]) {

  const std::regex pattern(R"([+-]?\d+(\.\d+)?([eE][+-]?\d+))");
  std::string s;
  while (true) {
    std::cout << "=============================================";
    std::cout << "\nEnter a number in scientific notation:" << std::endl;
    std::getline(std::cin, s);
    std::cout << "Regex says: "
              << ((std::regex_match(s, pattern) == 0) ? "Invalid."
                                                      : "Valid.")
              << std::endl;
    std::cout << "Finite state machine says: " << fsm(s) << std::endl;
    std::cout << "=============================================" << std::endl;
  }
}
