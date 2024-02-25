module;
#include <fstream>
#include <system_error>
module obscure.internal.read_file;

std::vector<char> obscure::internal::read_file(const char* file_name)
{
	std::ifstream file(file_name, std::ios::ate | std::ios::binary);

	if (!file.is_open()) throw std::errc::no_such_file_or_directory;

	size_t fileSize = static_cast<size_t>(file.tellg());

	std::vector<char> result(fileSize);

	file.seekg(0);
	file.read(result.data(), fileSize);

	file.close();
	return result;
}

std::vector<char> obscure::internal::read_file(std::filesystem::path file_path)
{
	std::ifstream file(file_path, std::ios::ate | std::ios::binary);

	if (!file.is_open()) throw std::errc::no_such_file_or_directory;

	size_t fileSize = static_cast<size_t>(file.tellg());

	std::vector<char> result(fileSize);

	file.seekg(0);
	file.read(result.data(), fileSize);

	file.close();
	return result;
}