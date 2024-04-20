/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 23:27:51 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/20 19:56:43 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string Utils::getExtensionFromFile(const std::string &filename)
{
	size_t dotPos = filename.find_last_of('.');
	if (dotPos == std::string::npos)
		return "";
	return filename.substr(dotPos + 1);
}

std::string Utils::getFileContent(const std::string &filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
		return "";
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

std::string Utils::joinPaths(const std::string &path1, const std::string &path2)
{
    if (path1.empty())
        return path2;
    if (path2.empty())
        return path1;

    char lastCharPath1 = path1[path1.length() - 1];
    char firstCharPath2 = path2[0];

    if (lastCharPath1 == '/' && firstCharPath2 == '/')
        return path1 + path2.substr(1);
    if (lastCharPath1 != '/' && firstCharPath2 != '/')
        return path1 + '/' + path2;

    return path1 + path2;
}

bool	Utils::fileExists(const std::string &path)
{
	std::ifstream file(path.c_str());
	return file.is_open() && !directoryExists(path);
}

bool	Utils::directoryExists(const std::string &path)
{
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
		return false;
	return info.st_mode & S_IFDIR;
}

std::string		Utils::getNodeName(const std::string &path)
{
	size_t pos = path.find_last_of('/');
	if (pos == std::string::npos)
		return path;
	return path.substr(pos + 1);
}


std::string		Utils::createHTMLDirectoryListing(const std::string &directory, const std::string &path)
{
	std::string html;
	html += "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of ";
	html += Utils::joinPaths("", directory.substr(directory.find('/') + 1));
	html += "</title>\n</head>\n<body>\n<h1>Index of ";
	html += directory.substr(directory.find('/') + 1);
	html += "</h1>\n<ul>\n";
	DIR *dir = opendir(directory.c_str());
	if (dir == NULL)
		return "";
	std::string dirName = Utils::getNodeName(directory);
	struct dirent *entry;
	html += "<li><a href=\".\">.</a></li>\n";
	html += "<li><a href=\"..\">..</a></li>\n";
	while ((entry = readdir(dir)) != NULL)
	{
		if (entry->d_name[0] == '.')
			continue;
		html += "<li><a href=\"";
		html += joinPaths(path, entry->d_name);
		html += "\">";
		html += entry->d_name;
		html += "</a></li>\n";
	}
	html += "</ul>\n</body>\n</html>\n";
	closedir(dir);
	return html;
}

std::string Utils::preventFileTraversal(const std::string &path)
{
	std::string result = path;
	size_t pos = result.find("..");
	while (pos != std::string::npos)
	{
		size_t slashPos = result.rfind('/', pos);
		if (slashPos == std::string::npos)
			return "";
		result.erase(slashPos, pos + 2 - slashPos);
		pos = result.find("..");
	}
	return result;
}

bool	Utils::createFile(const std::string &path, const std::string &content)
{
	std::ofstream file(path.c_str());
	if (!file.is_open())
		return false;
	file << content;
	file.close();
	return true;
}

bool	Utils::deleteFile(const std::string &path)
{
	return remove(path.c_str()) == 0;
}

bool	Utils::isValidSocket(int socketFD)
{
	int optval;
    socklen_t optlen = sizeof(optval);
    int result = getsockopt(socketFD, SOL_SOCKET, SO_TYPE, &optval, &optlen);
    return result == 0;
}
