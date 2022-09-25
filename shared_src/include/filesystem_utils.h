#ifndef FILESYSTEM_UTILS_H
#define FILESYSTEM_UTILS_H

#include <string>
#include <optional>

namespace Filesystem
{
	std::optional<std::string> LoadFile(const std::string &fileName);
}

#endif // FILESYSTEM_UTILS_H 
