#pragma once

unsigned char **pgmRead(char *fileName, int *rows, int *cols);
int pgmWrite(char *filename, int rows, int cols, unsigned char **image,
             char *comment_string);
int get_freq_table(char *filename, int **frequencies);
