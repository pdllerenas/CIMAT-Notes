#include "string.h"
#include <stdio.h>
#include <stdlib.h>

int longitud_str(const char *str) {
  int len = 0;
  while (*(str++)) {
    len++;
  }
  return len;
}

/*

We assume source is a null ending string. *destiny is any char pointer. dst_len
is the length of destiny, and we assume that the number passed is consistent
with the memory assigned to it

 */

char *copia_str(char *destiny, const char *source, int dst_len) {
  char *init = destiny; // store dst's starting memory address

  if (dst_len < longitud_str(source)) {
    fprintf(stderr, "destiny does not have enough space to copy source.\n");
    exit(1);
  }

  if ((destiny < source + longitud_str(source) && destiny > source) ||
      (destiny < source && source < destiny + dst_len)) {
    fprintf(stderr, "Strings overlap in memory. Exiting program.\n");
    exit(2);
  }
  do {
    *destiny = *source; 
    
    // do this while any of them are not the null char. By this
    // line, we are sure that source fits into destiny.
  } while (*(source++) | *(destiny++));
  return init;
}

int compara_str(const char *str1, const char *str2) {
  do {
    if (*str1 > *str2) {
      return *str1 - *str2;
    }
    if (*str1 < *str2) {
      return *str1 - *str2;
    }
    // loop while both are not '\0'. Note that we do not use &&
    // to prevent short-circuiting the statement, allowing us
    // to always move both pointers
  } while (*++str1 & *++str2);
  // if both have been equal until one (or both) reaches the char
  // '\0', we return  the distance of these two last chars
  return *str1 - *str2;
}

int concatena_str(char *str1, const char *str2, const char *str3) {
  char const *str1_copy = str1; // const pointer to start of str1, we will
                                // return this after all concatenations are done

  // set pointer to null character of str1
  while (*(str1++))
    ;
  str1--; // go back so we can replace null character with space char
  *str1 = ' ';

  str1++;
  do { // set characters on str1 until null char of str2 is reached
    *(str1++) = *(str2++);
  } while (*str2);

  *str1 = ' ';
  str1++;

  do {
    *(str1++) = *(str3++);
  } while (*str3);

  *str1 = '\0'; // end str1 with null character so that we can print it without
                // having memory problems

  // return length using pointer to start of str1
  return longitud_str(str1_copy);
}

int encuentra_str(const char *str1, const char *str2) {
  if (*str1 == '\0' ||
      *str2 == '\0') // If either of the strings are empty, return 0
    return 0;
  int occurrences = 0; // occurrence counter
  char const *cpy_str1 =
      str1; // constant pointer to where str1 starts, so we may reset str1
            // pointer after making a character match in the following nested
            // while loop
  while (*str2) { // while str2 does not point to the null terminating
                  // character, we continue searching for occurrences of str1

    // this initializes a loop once we find that the first
    // character of str1 matches some character of str2
    while (*str1 == *str2) {
      str1++;
      str2++;
      // if we reach the end of str1, we have found one occurrence
      if (*str1 == '\0') {
        occurrences++;
        str2--; // go back a position in str2, as we will add it back when
                // exiting this loop
        str1 = cpy_str1; // reset str1 pointer position for next possible
                         // character match
        break;
      }
    }
    str2++;
  }
  return occurrences;
}

int **frecuencia(const char *str) {
  if (*str == '\0')
    return 0;
  int **freq =
      malloc(26 * sizeof(*freq)); // assuming 26 lower-case letters a-z (no ~n)
  for (int i = 0; i < 26; i++) {
    int *char_freq = malloc(
        2 *
        sizeof(int)); // allocate space for each letter and its repetition value
    *(char_freq) = 'a' + i;
    *(char_freq + 1) = 0;
    *(freq + i) = char_freq;
  }

  while (*str) {
    if (*str - 'a' >= 0 && *str - 'a' < 26)
      *(*(freq + (*str - 'a')) + 1) += 1;
    str++;
  }

  return freq;
}

char *sin_repetir(const char *str) {
  char word[90];    // storage for current word being analyzed.
  char *ptr = word; // pointer to start of array. This will be the object we
                    // will be iterating to set the word array contents

  while (*str) {
    *ptr = *str;
    ptr++;
    str++;
    // We enter when a word is formed (that is, a space
    // character is found, or string str has ended).

    if (*str == ' ' || *str == '\0') {
      *ptr = '\0'; // End string so it can be processed by frecuencia function

      // we then reset pointer to initial position, so that the pointer passed
      // in to frecuencia is properly analyzed from start to finish
      ptr = word;
      int **freq = frecuencia(ptr); // char frequency array
      for (int i = 0; i < 26; i++) {
        if (freq[i][1] > 1) // if one char is repeated, end loop
          break;
        if (i == 25) { // if none have been repeated, print word
          printf("%s\n", word);
        }
      }
    }
  }
  return ptr;
}
