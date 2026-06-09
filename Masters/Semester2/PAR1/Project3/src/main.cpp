#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

#include "model.h"
#include "search.h"
#include "read.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        cerr << "Invalid argument count. Usage: " << argv[0] << " <threads> <data_file> [parameters_file]\n";
        return 1;
    }
    HParams hp;
    if (argc == 4)
    {
        hp = read_parameters_data(argv[3]);
    }
    string filename = argv[2];
    vector<DataPoint> experiment_data = read_experimental_data(filename);

    string sse_filename = "sse_output.csv";
    ofstream sse_file(sse_filename);

    Point x = HarmonicSearch([&experiment_data](const Point &p)
                             { return objective_SSE(p, experiment_data); }, hp);
    for (size_t i = 0; i < x.size(); ++i)
    {
        sse_file << x[i] << (i < x.size() - 1 ? ", " : "");
    }

    string sae_filename = "sae_output.csv";
    ofstream sae_file(sae_filename);
    x = HarmonicSearch([&experiment_data](const Point &p)
                             { return objective_SAE(p, experiment_data); }, hp);
    for (size_t i = 0; i < x.size(); ++i)
    {
        sae_file << x[i] << (i < x.size() - 1 ? ", " : "");
    }

    string mae_filename = "mae_output.csv";
    ofstream mae_file(mae_filename);
    x = HarmonicSearch([&experiment_data](const Point &p)
                             { return objective_MAE(p, experiment_data); }, hp);
    for (size_t i = 0; i < x.size(); ++i)
    {
        mae_file << x[i] << (i < x.size() - 1 ? ", " : "");
    }

    return 0;
}