#include <stdio.h>


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Use: %s <file_name.mat>\n", argv[0]);
		return 1;
	}

	FILE* file = fopen(argv[1], "rb");

	int header[5];
	fread(header, sizeof(int), 5, file);

	int format = header[0] % 10;
	int type = header[0] - format;

	printf("format: %i (%s)\n", format, (format == 0) ? "full matrix" : (format == 1) ? "text" : (format == 2) ? "Sparse matrix" : "INVALID");
	printf("type:   %i (%s)\n", type, (type == 0) ? "double" : (type == 10) ? "float" : (type == 20) ? "int" : (type == 30) ? "short" : (type == 40) ? "unsigned short" : (type == 50) ? "unsigned char" : "INVALID");

	if (format == 0) // Full matrix
	{
		int rows = header[1];
		int cols = header[2];
		int field = header[3];
		int name_size = header[4];
		char name[128];
		fread(name, name_size, 1, file);

		printf("rows:  %i\n", rows);
		printf("cols:  %i\n", cols);
		printf("field: %i (%s)\n", field, (field == 0) ? "real" : (field == 1) ? "complex" : "INVALID");
		printf("name:  %s\n", name);

		double* tmp = new double[rows*cols];
		fread(tmp, sizeof(double), rows*cols, file);

		double* matrix = new double[rows*cols];
		for (int i = 0; i < rows; ++i)
		{
			for (int j = 0; j < cols; ++j)
			{
				matrix[i*cols + j] = tmp[j*rows + i];
				printf("%7.2f", matrix[i*cols + j]);
			}
			printf("\n");
		}

		delete [] matrix;
		delete [] tmp;
	}
	else if (format == 1)
	{
		// Text matrix (Not used)
	}
	else if (format == 2)
	{
		// Sparse matrix
		// ...
	}

	fclose(file);
	return 0;
}
