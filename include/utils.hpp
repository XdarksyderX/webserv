/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 23:24:48 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/17 09:54:33 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <fstream>
# include <algorithm> 
# include <sys/stat.h>
# include <sys/types.h>
# include <dirent.h>

# include "Parser.hpp"

namespace Utils
{
	std::string 	getExtensionFromFile(const std::string &filename);
	std::string 	getFileContent(const std::string &filename);
	std::string		joinPaths(const std::string &path1, const std::string &path2);
	std::string		createHTMLDirectoryListing(const std::string &directory);
	std::string		preventFileTraversal(const std::string &path);
	std::string		getNodeName(const std::string &path);
	bool			fileExists(const std::string &path);
	bool			directoryExists(const std::string &path);
	bool			createFile(const std::string &path, const std::string &content);

	template <typename T>
	bool			hasElement(const std::vector<T> &elements,const T &element)
	{
		for (size_t i = 0; i < elements.size(); i++)
		{
			if (elements[i] == element)
				return true;
		}
		return false;
	}
}

#endif
