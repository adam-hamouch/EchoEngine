#include "Utils/FileUtils.h"

#include <iostream>
#include <fstream>

#ifdef CreateFile
#undef CreateFile
#endif

std::vector<std::string> FileUtils::ListFiles(const std::string& directoryPath, bool useRelativePath, const std::string& targetExtension)
{
	std::vector<std::string> files;

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
	{
		if (!std::filesystem::is_regular_file(entry.status()))
			continue;

		std::string extension = entry.path().extension().string();

		if (!targetExtension.empty() && extension != targetExtension)
			continue;

		files.push_back(useRelativePath ? entry.path().filename().string()
			: entry.path().string());
	}

	return files;
}

std::vector<std::string> FileUtils::ListSubdirectories(const std::string& directoryPath)
{
	std::vector<std::string> directories;

	for (const auto& entry : std::filesystem::directory_iterator(directoryPath))
	{
		if (entry.is_directory())
		{
			directories.push_back(entry.path().filename().string());
		}
	}

	return directories;
}

std::string FileUtils::FileName(const std::string& path)
{
	return std::filesystem::path(path).filename().string();
}

bool FileUtils::DeleteFile(const std::string& path)
{
	if (std::filesystem::exists(path)) {
		std::filesystem::remove(path);
		std::cout << "[Info] Deleted file: " << path << std::endl;
		return true;
	}
	else {
		std::cerr << "[Warning] File not found: " << path << std::endl;
		return false;
	}
}

bool FileUtils::CreateFile(const std::string& baseDir, const std::string& baseName, const std::string& extension, std::string& outFilename)
{
	if (!std::filesystem::exists(baseDir) || !std::filesystem::is_directory(baseDir)) {
		std::cerr << "[Error] Directory does not exist: " << baseDir << std::endl;
		return false;
	}

	int id = 1;
	do {
		outFilename = baseDir + "/" + baseName + "_" + std::to_string(id) + extension;
		++id;
	} while (std::filesystem::exists(outFilename));

	std::ofstream file(outFilename);
	if (!file.is_open()) {
		std::cerr << "[Error] Unable to create file: " << outFilename << std::endl;
		return false;
	}

	try {
		file.close();
		std::cout << "[Info] File created: " << outFilename << std::endl;
		return true;
	}
	catch (const std::exception& e) {
		std::cerr << "[Error] Writing file: " << e.what() << std::endl;
		return false;
	}
}

std::string FileUtils::GetExtension(const std::string& filePath)
{
	size_t dotPos = filePath.find_last_of('.');
	if (dotPos != std::string::npos)
	{
		return filePath.substr(dotPos); // Inclut le point, ex: ".png"
	}
	return "";
}

bool FileUtils::RenameFile(const std::string& oldPath, const std::string& newPath)
{
	try
	{
		if (!std::filesystem::exists(oldPath))
			return false;

		std::filesystem::rename(oldPath, newPath);
		return true;
	}
	catch (const std::filesystem::filesystem_error& e)
	{
		// Optional: log or print e.what()
		return false;
	}
}
