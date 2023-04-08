#ifndef OBSCURE_READ_FILE_DEFINITION
#define OBSCURE_READ_FILE_DEFINITION 1
#include <vector>
#include <filesystem>
namespace obscure
{
	std::vector<char> read_file(const char* file_name);
	std::vector<char> read_file(std::filesystem::path file_path);
}
#endif