/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 23:24:48 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/16 18:18:50 by migarci2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <fstream>
# include <sys/stat.h>

# include "Parser.hpp"

namespace Utils
{
	std::string 	getExtensionFromFile(const std::string &filename);
	std::string 	getFileContent(const std::string &filename);
	std::string		joinPaths(const std::string &path1, const std::string &path2);
	bool			fileExists(const std::string &path);
	bool			directoryExists(const std::string &path);
}

#endif
