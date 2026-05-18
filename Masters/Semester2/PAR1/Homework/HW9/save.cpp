#include <stdio.h>
#include <string.h>


int main()
{
	const char* name = "spa";
	int name_size = strlen(name) + 1;
	double rows = 3;
	double cols = 2;
	double I[4] = {1, 1, 2, 3};
	double J[4] = {1, 2, 1, 2};
	double V[4] = {-1, 1.1, 2, 3};

	FILE* file = fopen("sparse.mat", "wb");

	int header[5];
	header[0] = 2 + 0; // 2 = sparse matrix, +0 = double
	header[1] = 4 + 1; // Number of non-zero + 1
	header[2] = 3; // 3 = real
	header[3] = 0; // fixed value 0
	header[4] = name_size; // name length + 1
	fwrite(header, sizeof(int), 5, file);

	fwrite(name, sizeof(char), name_size, file);

	fwrite(I, sizeof(double), 4, file);
	fwrite(&rows, sizeof(double), 1, file);

	fwrite(J, sizeof(double), 4, file);
	fwrite(&cols, sizeof(double), 1, file);

	fwrite(V, sizeof(double), 4, file);

	double zero = 0;
	fwrite(&zero, sizeof(double), 1, file);

	fclose(file);
	return 0;
}
