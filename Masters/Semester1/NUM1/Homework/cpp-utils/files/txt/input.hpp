#include <string>
#include <vector>
#include <fstream>
#include <iostream>

namespace file_input {
	template<typename T>
	std::vector<T> readFile(const std::string& filename) {
		std::vector<T> items;
		std::ifstream file(filename);

		if (!file.is_open()) {
			std::cerr << "Error: Could not open file " << filename << std::endl;
			return items;
		}

		T value;
		while (file >> value) {
			items.push_back(value);
		}

		file.close();
		return items;
	}
}

