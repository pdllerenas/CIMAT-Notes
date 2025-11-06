/*
 *
 * TAREA 3
 * PEDRO DAVID LLERENAS GONZALEZ
 *
 */

#include <iostream>
#include <regex>
#include <string>

std::string BigSum(std::string, std::string, unsigned int);
std::string BigDifference(std::string, std::string, unsigned int);
std::string BigProduct(std::string, std::string, unsigned int);
std::string BigDivision(std::string, std::string);

// returns -1 if m > n
// returns 0 if m == n
// returns 1 if n > m
int compare(std::string n, std::string m) {
  if (n.length() < m.length() || (n.length() == m.length() && n < m)) {
    return -1;
  } else if (n == m) {
    return 0;
  } else {
    return 1;
  }
}

/*
 * Performs the integer difference n - m.
 */
std::string BigDifference(std::string n, std::string m, unsigned int base) {
	if (n[0] == '-' && m[0] == '-') {
		n = n.substr(1);
		m = m.substr(1);
		swap(n,m);
	}
	if (n[0] == '+' && m[0] == '-') {
		return BigSum(n.substr(1), m.substr(1), base);
	}

	if (m[0] == '-' && n[0] != '-' && n[0] != '+') {
		return BigSum(n, m.substr(1), base);
	}

	if (m[0] == '+' && n[0] != '-' && n[0] != '+') {
		m = m.substr(1);
	}

	if (n[0] == '-' && m[0] != '-' && m[0] != '+') {
		return '-' + BigSum(n.substr(1), m, base);
	}

	if (n[0] == '+' && m[0] != '-' && m[0] != '+') {
		n = n.substr(1);
	}

  char sign = ' ';
  // this simplifies the computation of the sign, as this
  // calculates the absolute value, then adds the correspoding sign
  if (compare(n, m) == -1) {
    sign = '-';
    swap(n, m);
  }
  std::size_t max_idx = std::max(n.length(), m.length());
  unsigned int carry = 0;
  std::string result;

  // TODO: since m > n, an if statement is redundant
  for (int i = 0; i < max_idx; i++) {
    if (i >= n.length()) {
      int sum = (m[m.length() - 1 - i] - '0') - carry;
      result += char(sum + '0');
      carry = 0;
    } else if (i >= m.length()) {
      int sum = (n[n.length() - 1 - i] - '0') - carry;
      result.push_back(char(sum + '0'));
      carry = 0;
    } else {
      int diff = ((n[n.length() - 1 - i] - '0') -
                  (m[m.length() - 1 - i] - '0') - carry);
      if (diff < 0 && i < n.length() - 1) {
        result.push_back(char(diff + base + '0'));
        carry = 1;
      } else if (diff < 0 && i == n.length() - 1) {
        result.push_back(char(-diff + '0'));
        carry = 0;
      } else {
        result.push_back(char(diff + '0'));
        carry = 0;
      }
    }
  }

  if (carry > 0) {
    result += char(carry + '0');
  }
  // remove leading 0's (before reversing, so the last digits)
  while (result.size() > 1 && result.back() == '0') {
    result.pop_back();
  }

  std::string reversed;
  if (sign == '-') {
    reversed += sign;
  }
  for (int i = result.length() - 1; i >= 0; i--) {
    reversed += result[i];
  }
  // remove leading 0's
  return reversed;
}

std::string BigSum(std::string n, std::string m, unsigned int base) {
	// TODO: refactor so BigSum does not concern itself with
	// deferring these cases
	if (n[0] == '-' && m[0] != '-' && m[0] != '+') {
		return BigDifference(m, n.substr(1), base);
	}

	if (m[0] == '-' && n[0] != '-' && n[0] != '+') {
		return BigDifference(n, m.substr(1), base);
	}

	char sign;
	if (m[0] == '-' && n[0] == '-') {
		sign = '-';
		m = m.substr(1);
		n = n.substr(1);
	}
	if (m[0] == '+' && n[0] == '+') {
		m = m.substr(1);
		n = n.substr(1);
	}

	// remove leading 0's
	while (m.size() > 1 && m[0] == '0') {
		m.erase(m.begin());
	}

	while (n.size() > 1 && n[0] == '0') {
		n.erase(n.begin());
	}

  std::size_t max_idx = std::max(n.length(), m.length());
  unsigned int carry = 0;
  std::string result;
  for (int i = 0; i < max_idx; i++) {
    if (i >= n.length()) {
      int sum = (m[m.length() - 1 - i] - '0') + carry;
      result += char(sum % base + '0');
      carry = sum / base;
    } else if (i >= m.length()) {
      int sum = (n[n.length() - 1 - i] - '0') + carry;
      result += char(sum % base + '0');
      carry = sum / base;
    } else {
      int sum = ((n[n.length() - 1 - i] - '0') + (m[m.length() - 1 - i] - '0') +
                 carry);
      result += char(sum % base + '0');
      carry = sum / base;
    }
  }
  if (carry > 0) {
    result += char(carry + '0');
  }
  std::string reversed;
  for (int i = result.length() - 1; i >= 0; i--) {
    reversed += result[i];
  }
	if (sign == '-') {
		reversed.insert(0, 1, sign);
	}
  return reversed;
}

std::string BigProduct(std::string n, std::string m, unsigned int base) {
	// choose sign
  char sign = (n[0] == '-') ^ (m[0] == '-') ? '-' : ' ';
  if (n[0] == '-' || n[0] == '+') {
    n = n.substr(1);
  }
  if (m[0] == '-' || m[0] == '+') {
    m = m.substr(1);
  }

  // remove leading 0's
  while (n.size() > 1 && n[0] == '0') {
    n.erase(0, 1);
  }
  while (m.size() > 1 && m[0] == '0') {
    m.erase(0, 1);
  }

	// trivial case
  if (n == "0" || m == "0") {
    return "0";
  }
  std::string result = "0";

  unsigned int carry = 0;
  unsigned short int prod;
  // go thru n in reverse order
  for (int i = n.length() - 1; i >= 0; i--) {
    std::string temp = "";

    for (int k = 0; k < n.length() - 1 - i; k++) {
      temp.push_back('0');
    }
    for (int j = m.length() - 1; j >= 0; j--) {
      prod = (n[i] - '0') * (m[j] - '0') + carry;
      if (prod >= base) {
        carry = prod / base;
      } else {
        carry = 0;
      }
      temp.insert(0, 1, char(prod % base + '0'));
    }
    if (carry > 0) {
      temp.insert(0, 1, char(carry + '0'));
    }
    result = BigSum(result, temp, base);
  }
  if (carry > 0) {
    result.insert(0, 1, char(carry + '0'));
  }
  if (sign == '-') {
    result.insert(0, 1, sign);
  }
  return result;
}

/*
 * performs n / m in long division
 */
std::string BigDivision(std::string n, std::string m) {
  if (m == "0") {
    return "Division by zero";
  }
  int c = compare(n, m);
  if (c == -1) {
    return "0";
  }
  if (c == 0) {
    return "1";
  }

  char sign = (n[0] == '-') ^ (m[0] == '-') ? '-' : ' ';
  if (n[0] == '-' || n[0] == '+') {
    n = n.substr(1);
  }
  if (m[0] == '-' || m[0] == '+') {
    m = m.substr(1);
  }
  // result stores the quotient, current stores the dividends
  // digits taken into account, then the residues
  std::string result, current;

  // loop to grab more digits of n
  for (int i = 0; i < n.length(); i++) {
    current.push_back(n[i]);
    // remove leading 0's of n
    if (current.size() > 1 && current[0] == '0') {
      current.erase(current.begin());
    }

    // stores the amount of times m fits in current
    int temp = 0;
    // while current is greater than or equal to m,
    // perform current = current - m
    while (compare(current, m) >= 0) {
      current = BigDifference(current, m, 10);
      temp++;
    }
    // stores the calculated digit in result
    result.push_back(temp + '0');
  }

  // remove leading 0's
  while (result.size() > 1 && result[0] == '0') {
    result.erase(result.begin());
  }
  if (sign == '-') {
    result.insert(0, 1, sign);
  }

  return result;
}

int main(int argc, char *argv[]) {
  std::string n;
  std::string m;
  char op;

  // to ensure n and m are valid integers
  const std::regex int_regex(R"([+-]?\d+)");

  std::string result;

  while (true) {
    std::cout
        << "Insert an operation of the form: 'n (+,-,/,*) m' (make sure to "
           "use spaces).\n";

    std::cin >> n >> op >> m;

    if (std::regex_match(n, int_regex) == 0 ||
        std::regex_match(m, int_regex) == 0) {
      std::cout << "Error: Invalid integer input.\n" << std::endl;
      continue;
    }

		// TODO: actually implement different base handling
    switch (op) {
    case '+':
      result = BigSum(n, m, 10);
      break;
    case '-':
      result = BigDifference(n, m, 10);
      break;
    case '*':
      result = BigProduct(n, m, 10);
      break;
    case '/':
      result = BigDivision(n, m);
      break;
    default:
      std::cout << "Error: Invalid operator.\n" << std::endl;
      continue;
    }

    if (n.length() >= m.length()) {
      std::cout << " " << n << std::endl;
      std::cout << op << std::string(n.length() - m.length(), ' ') << m
                << std::endl;
    } else {
      std::cout << std::string(m.length() - n.length() + 1, ' ') << n
                << std::endl;
      std::cout << op << m << std::endl;
    }
    std::cout << "=====================================================\n";
    std::cout << result << std::endl;
  }
}
