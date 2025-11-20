#include <iostream>

union A {
	int i;
	char c;
};

void solution() {
	A a;
	a.i = 1;
	if (a.c != 0) {
		std::cout << "little endian" << std::endl;
	} else  {
		std::cout << "big endian" << std::endl;
	}
}

int main() {
	solution();
}
