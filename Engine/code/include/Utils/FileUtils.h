#pragma once
#include <filesystem>
#include <string>
#include <vector>

class FileUtils
{
public:
	//Return files&Directories 
	static std::vector<std::string> ListFiles(const std::string& directoryPath, bool useRelativePath, const std::string& targetExtension = ""); 
	static std::vector<std::string> ListSubdirectories(const std::string& directoryPath);

	//Return fileName of a path
	static std::string FileName(const std::string& path);

	//Delete&Create
	static bool DeleteFile(const std::string& path);
	static bool CreateFile(const std::string& baseDir, const std::string& baseName, const std::string& extension, std::string& outFilename);

	static std::string GetExtension(const std::string& filePath);

	//Rename a file (returns true if successful)
	static bool RenameFile(const std::string& oldPath, const std::string& newPath);
};
