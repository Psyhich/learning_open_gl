#include <iostream>

#include "fmt/format.h"

#include "filesystem_utils.h"

std::optional<std::string> Filesystem::LoadFile(const std::string &fileName)
{
	FILE *openedFile = fopen(fileName.c_str(), "r");
	if(!openedFile)
	{
		std::cerr << fmt::format("Failed to open {}, got error: {}\n",
			fileName, std::strerror(errno));
		return std::nullopt;
	}

	fseek(openedFile, 0, SEEK_END);
	const long int fileSize = ftell(openedFile);
	if(fileSize == -1)
	{
		std::cerr << fmt::format("Failed to determine size of file: {}, got error: {}\n",
			fileName, std::strerror(errno));
		return std::nullopt;
	}

	rewind(openedFile);

	std::string fileData;
	fileData.resize(fileSize);

	if(fread(fileData.data(), sizeof(char), fileSize, openedFile) != fileSize)
	{
		std::cerr << fmt::format("Error occured while reading file: {}, got error: {}\n",
			fileName, std::strerror(errno));
		return std::nullopt;
	}

	fclose(openedFile);

	return fileData;
}
