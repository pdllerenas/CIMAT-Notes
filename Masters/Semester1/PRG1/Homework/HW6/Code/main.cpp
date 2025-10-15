/*
 * TAREA 6 PRG1
 * PEDRO DAVID LLERENAS GONZÁLEZ
 */



#include <chrono>
#include <iostream>

volatile bool sink; // prevents optimization

bool gpt_leap_year(unsigned int year) {
  if (year % 400 == 0) {
    return true;
  } else if (year % 100 == 0) {
    return false;
  } else if (year % 4 == 0) {
    return true;
  }
  return false;
}

bool microsoft_v1_leap_year(unsigned int year) {
  if (year % 4 != 0) {
    return false;
  }
  if (year % 400 == 0) {
    return true;
  }
  if (year % 100 == 0) {
    return false;
  }
  return true;
}

bool microsoft_v2_leap_year(unsigned int year) {
  return ((year % 4) == 0) && ((year % 100) != 0 || (year % 400) == 0);
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
  return (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0);
}

// wrapper function to loop function calls of f,
// we use the volatile variable here to "prevent optimization",
// as indicated by
// https://en.cppreference.com/w/cpp/chrono/steady_clock/now.html
void loop_func(int start, int end, bool (*f)(unsigned int)) {
  for (int i = start; i <= end; i++) {
    sink = f(i);
  }
}

// prints the average execution time of the function f, over iter iterations
void benchmark(int iter, int index_start, int index_end,
               bool (*f)(unsigned int), std::string fn_name) {
  std::chrono::duration<double> avg;
  for (int i = 0; i < iter; i++) {
    auto start = std::chrono::steady_clock::now();
    loop_func(index_start, index_end, f);
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff = end - start;
    avg += diff;
  }
  avg /= iter;
  // std::fixed prevents cout from printing avg.count in scientific notation
  std::cout << std::fixed << fn_name << ": " << avg.count() * 1000 << " ms"
            << std::endl;
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    fprintf(
        stderr,
        "Invalid argument count: Usage: %s <trials> <year-start> <year-end>\n",
        argv[0]);
    exit(1);
  }
  int iter_s = atoi(argv[2]), iter_e = atoi(argv[3]);
  int trials = atoi(argv[1]);

  printf("===========================\n");
  benchmark(trials, iter_s, iter_e, gpt_leap_year, "gpt");
  printf("===========================\n");
  benchmark(trials, iter_s, iter_e, microsoft_v1_leap_year, "ms v1");
  printf("===========================\n");
  benchmark(trials, iter_s, iter_e, microsoft_v2_leap_year, "ms v2");
  printf("===========================\n");
  benchmark(trials, iter_s, iter_e, optimized_400_leap_year, "optimized 400");
  printf("===========================\n");
  benchmark(trials, iter_s, iter_e, optimized_16_leap_year, "optimized 16");
  printf("===========================\n");
  benchmark(trials, iter_s, iter_e, proposed_leap_year, "proposed");
  printf("===========================\n");
}
