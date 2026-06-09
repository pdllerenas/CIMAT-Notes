#include <vector>
#include <string>
#include "types.h"

std::vector<DataPoint> read_experimental_data(const std::string& filename);
HParams read_parameters_data(const std::string& filename);