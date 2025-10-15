#include <chrono>
#include <iostream>

// https://en.cppreference.com/w/cpp/chrono/high_resolution_clock/now.html
volatile bool sink; // prevents optimization

void loop_func(int start, int end, bool (*f)(unsigned int)) {
  for (int i = start; i <= end; i++) {
    sink = f(i);
  }
}

bool gpt_leap_year(unsigned int year) {
  if (year % 4 != 0) {
    return false;
  }
  if (year % 100 != 0) {
    return true;
  }
  if (year % 400 == 0) {
    return true;
  }
  return false;
}

bool microsoft_v1_leap_year(unsigned int year) {
  if (year % 4 != 0)
    return false;
  return (year % 100 != 0 || year % 400 == 0);
}

bool microsoft_v2_leap_year(unsigned int year) {
  if (year % 4 != 0)
    return false;
  return (year % 100 != 0 || year % 400 == 0);
}

bool optimized_400_leap_year(unsigned int year) {
  if (year % 100 != 0) {
    return year % 4 == 0;
  }
  return year % 400 == 0;
}

bool optimized_16_leap_year(unsigned int year) {
  if (year % 100 != 0) {
    return year % 4 == 0;
  }
  // using 400 = 16 * 25, and since year % 100 == 0,
  // year / 100 has to be divisible by 4, or year % 16 == 0
  return year % 16 == 0;
}

bool proposed_leap_year(unsigned int year) {
  return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

int main(int argc, char *argv[]) {
	int iter_s = 0, iter_e = 200000;

  printf("===========================\n");
  auto start = std::chrono::high_resolution_clock::now();
  loop_func(iter_s, iter_e, gpt_leap_year);
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << "gpt_leap_year: " << diff.count() << std::endl;

  printf("===========================\n");
  start = std::chrono::high_resolution_clock::now();
  loop_func(iter_s, iter_e, microsoft_v1_leap_year);
  end = std::chrono::high_resolution_clock::now();
  diff = end - start;
  std::cout << "microsoft v1: " << diff.count() << std::endl;

  printf("===========================\n");
  start = std::chrono::high_resolution_clock::now();
  loop_func(iter_s, iter_e, microsoft_v2_leap_year);
  end = std::chrono::high_resolution_clock::now();
  diff = end - start;
  std::cout << "microsoft v2: " << diff.count() << std::endl;

  printf("===========================\n");
  start = std::chrono::high_resolution_clock::now();
  loop_func(iter_s, iter_e, optimized_400_leap_year);
  end = std::chrono::high_resolution_clock::now();
  diff = end - start;
  std::cout << "optimized 400: " << diff.count() << std::endl;

  printf("===========================\n");
  start = std::chrono::high_resolution_clock::now();
  loop_func(iter_s, iter_e, optimized_16_leap_year);
  end = std::chrono::high_resolution_clock::now();
  diff = end - start;
  std::cout << "optimized 16: " << diff.count() << std::endl;

  printf("===========================\n");
  start = std::chrono::high_resolution_clock::now();
  loop_func(iter_s, iter_e, proposed_leap_year);
  end = std::chrono::high_resolution_clock::now();
  diff = end - start;
  std::cout << "proposed: " << diff.count() << std::endl;
  printf("===========================\n");
}
