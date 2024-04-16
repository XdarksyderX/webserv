/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 23:27:51 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/16 19:38:05 by migarci2         ###   ########.fr       */
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
	return file.is_open();
}

bool	Utils::directoryExists(const std::string &path)
{
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
		return false;
	return info.st_mode & S_IFDIR;
}

