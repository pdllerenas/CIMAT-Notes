#include <unistd.h>
#include <iostream>

int factorial(unsigned int N) {
	if (N == 0) {
		return 1;
	} else if (N == 1) {
		return 1;
	}
	pid_t pid;
	pid = fork();
	
	return 0; 
}

int main() {

	return 0;
}
