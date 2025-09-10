#include "io.h"

#include <filesystem>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <ios>

namespace SBF {

std::vector<uint8_t> ReadFileAsBytes(const std::filesystem::path &filepath) {

	if (!std::filesystem::exists(filepath)) throw std::runtime_error("file not found");
	if (!std::filesystem::is_regular_file(filepath)) throw std::runtime_error("path is not a file");

	std::ifstream file(filepath, std::ios::binary | std::ios::ate);

	if (!file.is_open()) throw std::runtime_error("failed to open file");

	return std::vector<uint8_t>(
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>()
	);
}
};
