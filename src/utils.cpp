/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 23:27:51 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/20 23:54:37 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.hpp"

std::string Utils::to_string(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

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

bool Utils::fileExists(const std::string &path)
{
	std::ifstream file(path.c_str());
	return file.is_open() && !directoryExists(path);
}

bool Utils::directoryExists(const std::string &path)
{
	struct stat info;
	if (stat(path.c_str(), &info) != 0)
		return false;
	return info.st_mode & S_IFDIR;
}

std::string Utils::getNodeName(const std::string &path)
{
	size_t pos = path.find_last_of('/');
	if (pos == std::string::npos)
		return path;
	return path.substr(pos + 1);
}

std::string Utils::createHTMLDirectoryListing(const std::string &directory, const std::string &path)
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

bool Utils::createFile(const std::string &path, const std::string &content)
{
	std::ofstream file(path.c_str());
	if (!file.is_open())
		return false;
	file << content;
	file.close();
	return true;
}

bool Utils::deleteFile(const std::string &path)
{
	return remove(path.c_str()) == 0;
}

bool Utils::isValidSocket(int socketFD)
{
	int optval;
	socklen_t optlen = sizeof(optval);
	int result = getsockopt(socketFD, SOL_SOCKET, SO_TYPE, &optval, &optlen);
	return result == 0;
}

std::string Utils::convertToBase64(const std::string &input)
{
	const char base64Chars[] =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	std::string result;
	size_t inputSize = input.size();
	for (size_t i = 0; i < inputSize; i += 3)
	{
		uint32_t value = 0;
		int remaining = inputSize - i;
		for (size_t j = 0; j < 3; j++)
		{
			value <<= 8;
			if (i + j < inputSize)
				value |= (unsigned char)input[i + j];
		}
		for (int j = 0; j < 4; j++)
		{
			if (j * 6 < remaining * 8)
			{
				result += base64Chars[(value >> (18 - j * 6)) & 0x3F];
			}
			else
			{
				result += '=';
			}
		}
	}
	return result;
}

int base64CharToValue(char c)
{
	if (c >= 'A' && c <= 'Z')
		return c - 'A';
	if (c >= 'a' && c <= 'z')
		return c - 'a' + 26;
	if (c >= '0' && c <= '9')
		return c - '0' + 52;
	if (c == '+')
		return 62;
	if (c == '/')
		return 63;
	return -1;
}

std::string Utils::convertFromBase64(const std::string &input)
{
	std::string output;
	std::vector<int> temp;

	for (size_t i = 0; i < input.size(); ++i)
	{
		char c = input[i];
		if (c == '=')
			break;
		int val = base64CharToValue(c);
		if (val >= 0)
			temp.push_back(val);
	}

	for (size_t i = 0; i < temp.size(); i += 4)
	{
		int sextet_a = temp[i];
		int sextet_b = temp[i + 1];
		int sextet_c = (i + 2 < temp.size()) ? temp[i + 2] : 0;
		int sextet_d = (i + 3 < temp.size()) ? temp[i + 3] : 0;

		int triple = (sextet_a << 18) + (sextet_b << 12) + (sextet_c << 6) + sextet_d;

		output.push_back((triple >> 16) & 0xFF);
		if (i + 2 < temp.size())
			output.push_back((triple >> 8) & 0xFF);
		if (i + 3 < temp.size())
			output.push_back(triple & 0xFF);
	}

	return output;
}
