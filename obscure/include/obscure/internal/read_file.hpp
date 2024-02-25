module;
#include <vector>
#include <filesystem>
export module obscure.internal.read_file;

namespace obscure
{
	namespace internal
	{
		export std::vector<char> read_file(const char* file_name);
		export std::vector<char> read_file(std::filesystem::path file_path);
	}
}