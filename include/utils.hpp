/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: migarci2 <migarci2@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/15 23:24:48 by migarci2          #+#    #+#             */
/*   Updated: 2024/04/20 19:57:14 by migarci2         ###   ########.fr       */
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
# include <sys/socket.h>
# include <errno.h>

# include "Parser.hpp"

/**
 * @namespace Utils
 * @brief Utility functions to assist with common file and network operations.
 */
namespace Utils
{
    /**
     * @brief Extracts the file extension from a filename.
     * @param filename The full name of the file.
     * @return The extension of the file as a string.
     */
    std::string getExtensionFromFile(const std::string &filename);

    /**
     * @brief Reads the entire content of a file into a string.
     * @param filename The path to the file.
     * @return A string containing all the data in the file.
     */
    std::string getFileContent(const std::string &filename);

    /**
     * @brief Concatenates two file path segments into a single path.
     * @param path1 The first path segment.
     * @param path2 The second path segment to be joined with the first.
     * @return The combined file path.
     */
    std::string joinPaths(const std::string &path1, const std::string &path2);

    /**
     * @brief Generates an HTML formatted string listing all the files in a directory.
     * @param directory The directory to list.
     * @return A string containing an HTML document listing all directory contents.
     */
    std::string createHTMLDirectoryListing(const std::string &directory, const std::string &path = "");

    /**
     * @brief Prevents directory traversal by sanitizing the path.
     * @param path The potentially unsafe file path.
     * @return A sanitized version of the path.
     */
    std::string preventFileTraversal(const std::string &path);

    /**
     * @brief Gets the node name from a path.
     * @param path The file path.
     * @return The node name as a string.
     */
    std::string getNodeName(const std::string &path);

    /**
     * @brief Checks if a file exists at the specified path.
     * @param path The path to the file.
     * @return True if the file exists, otherwise false.
     * @note If the path is a directory, even if it exists, returns false.
     */
    bool fileExists(const std::string &path);

    /**
     * @brief Checks if a directory exists at the specified path.
     * @param path The path to the directory.
     * @return True if the directory exists, otherwise false.
     */
    bool directoryExists(const std::string &path);

    /**
     * @brief Creates a file with the specified content.
     * @param path The path where the file will be created.
     * @param content The content to write to the file.
     * @return True if the file was successfully created, otherwise false.
     */
    bool createFile(const std::string &path, const std::string &content);

    /**
     * @brief Deletes a file at the specified path.
     * @param path The path to the file.
     * @return True if the file was successfully deleted, otherwise false.
     */
    bool deleteFile(const std::string &path);

    /**
     * @brief Checks if a given socket file descriptor is valid.
     * @param socketFD The file descriptor of the socket.
     * @return True if the socket is valid, otherwise false.
     */
    bool isValidSocket(int socketFD);

    /**
     * @brief Checks if a vector contains a specific element.
     * @param elements The vector of elements.
     * @param element The element to search for in the vector.
     * @return True if the element is found in the vector, otherwise false.
     */
    template <typename T>
    bool hasElement(const std::vector<T> &elements, const T &element)
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
